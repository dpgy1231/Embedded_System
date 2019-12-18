/*
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "header/pinout.h"

#include "drivers/mx66l51235f.h"
#define  STX    0x02
#define  ETX    0x03
#define  ESC    0x1B
#define  ACK	0x06
#define  NACK	0x15

//*****************************************************************************
//
// The system clock frequency in Hz.
//
//*****************************************************************************
uint32_t g_ui32SysClock;

int save_addr;
int image_size;

char flag= 0;

void res_func(char tx);
void data_check(char *data, int size);

int main(void) {
	
	unsigned char serial_buffer[600];
	unsigned char cs = 0;
	unsigned int  rx_cnt = 0;

	char rsv_ch;
	char start_flag = 0;
	char esc_flag = 0;

	//
	// Run from the PLL at 120 MHz.
	//
	g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
	                                     SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
	                                     SYSCTL_CFG_VCO_480), 120000000);

	PinoutSet();

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
	SysCtlPeripheralReset(SYSCTL_PERIPH_SSI3);
	MX66L51235FInit(g_ui32SysClock);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

	while(1){
		rsv_ch = uartgetchar(UART0_BASE);

		switch(rsv_ch){
		case STX:
			if(start_flag) res_func(NACK);
			else start_flag = 1;

			rx_cnt = 0;
			cs     = 0;
			break;

		case ETX:
			if(start_flag) {
				start_flag = 0;
				if(cs) res_func(NACK);
				else {
					data_check(serial_buffer, rx_cnt-1);
					rx_cnt = 0;
				}
			}
			else res_func(NACK);

			break;

		case ESC:
			esc_flag = 1;
			cs ^= ESC;
			break;

		default:
			if(esc_flag){
				esc_flag = 0;
				serial_buffer[rx_cnt++] = rsv_ch ^ ESC;
			}
			else{
				serial_buffer[rx_cnt++] = rsv_ch;
			}
			cs ^= rsv_ch;

		}
	}

	return 0;
}

void res_func(char tx){
	char res_data[6] = {STX, 0, 0, 0, 0, ETX};
	char cs = 0;
	char esc_flag = 0;

	res_data[1] = tx;
	cs ^= tx;
	res_data[4] = cs;

	UARTSend((uint8_t *)res_data, 6);
}

void data_check(char *data, int size){
	int i, data_cnt, data_size;

	char cmd;
	int erase_addr = 0;
	int max_addr = 0;
	unsigned long ulClockMS;

	ulClockMS = g_ui32SysClock / (3 * 1000);

	if(size < 0){
		res_func(NACK);
		return;
	}
	else{
		/*
		for(i = 0, data_cnt = 0; i < size; ++i){
			if(esc_flag){
				esc_flag = 0;
				data[data_cnt++] = data[i] ^ ESC;
			}
			else if(ESC == data[i]) esc_flag = 1;
			else {
				data[data_cnt++] = data[i];
			}
		}
		*/

		cmd = data[0];

		data_size = (int)data[1]<<8 | data[2];
		if(data_size + 3 != size)
		{
			res_func(NACK);
			return;
		}

		//cmd_func
		switch(cmd){
		case 0x10: //image_addr
			save_addr  = (((int)data[3]&0xFF)<<24) | (((int)data[4]&0xFF)<<16) | (((int)data[5]&0xFF)<<8) | data[6];
			image_size = (((int)data[7]&0xFF)<<24) | (((int)data[8]&0xFF)<<16) | (((int)data[9]&0xFF)<<8) | data[10];

			erase_addr = save_addr & 0xFFFFE000;
			max_addr   = save_addr + image_size;

			for(erase_addr = save_addr & 0xFFFFE000; erase_addr < max_addr; erase_addr += 0x1000){
				MX66L51235FSectorErase(erase_addr);
				SysCtlDelay(ulClockMS *10);
			}

			break;

		case 0x20: //data
//			if(save_addr == 0x4500){
//				flag = 1;
//			}
			MX66L51235FPageProgram(save_addr, (unsigned char*)&data[3], data_size);
			SysCtlDelay(ulClockMS *3);
			save_addr += data_size;
			break;
		default:
			res_func(NACK);
			return;
		}
		res_func(ACK);
	}

}
