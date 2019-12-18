/*
 * pinout.c
 *
 *  Created on: 2014. 12. 8.
 *      Author: Administrator
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "header/pinout.h"

void PinoutSet(void)
{
	//
	// Enable all the GPIO peripherals.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//Buzzer2 //I2C6,7 //USB0,1
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//segment
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//bluetooth //Buzzer4 //USB6,7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//SW
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//SD0,4,5 //LCD7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);//segment
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);//segment//SD4
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);//bluetooth //LCD2,3,6
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);//USB6,7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);//segment
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);//LCD6,7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);//SW //segment 3
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);//SD0,1,2
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOR);//LCD0,1,2,3,4,5,6,7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOS);//LCD4,5,6,7
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOT);//LCD0,1

    //
    // PA0-1 are used for UART0.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    //
    // PJ0-1 are used for UART3.
    //
    ROM_GPIOPinConfigure(GPIO_PJ0_U3RX);
    ROM_GPIOPinConfigure(GPIO_PJ1_U3TX);
    ROM_GPIOPinTypeUART(GPIO_PORTJ_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    //ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);

    //LED
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);


    //SW
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_3);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_1);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_7);

    //DIP
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_6);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTQ_BASE, GPIO_PIN_4);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTQ_BASE, GPIO_PIN_5);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTQ_BASE, GPIO_PIN_6);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3);

    //7 segment
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //Buzzer
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_4);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2);
    ROM_GPIOPinConfigure(GPIO_PB2_T5CCP0);

    //I2C
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_7);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_6);
    ROM_GPIOPinConfigure(GPIO_PB6_I2C6SCL);
    ROM_GPIOPinConfigure(GPIO_PB7_I2C6SDA);

    //SD Card
    ROM_GPIOPinConfigure(GPIO_PF0_SSI3XDAT1);
    ROM_GPIOPinConfigure(GPIO_PF4_SSI3XDAT2);
    ROM_GPIOPinConfigure(GPIO_PF5_SSI3XDAT3);
    ROM_GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
    ROM_GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
    ROM_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4 | GPIO_PIN_5);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_4);
    ROM_GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_4, GPIO_PIN_4);
    ROM_GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_2);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_1);
    ROM_GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, GPIO_PIN_1);

    //USB
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0xff;
    ROM_GPIOPinConfigure(GPIO_PD6_USB0EPEN);
    ROM_GPIOPinConfigure(GPIO_PD7_USB0PFLT);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ROM_GPIOPinTypeUSBDigital(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTL_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    //
    // PF6-7/PJ6/PS4-5/PR0-7 are used for the LCD.
    //
    ROM_GPIOPinConfigure(GPIO_PF7_LCDDATA02);
    ROM_GPIOPinConfigure(GPIO_PJ2_LCDDATA14);
    ROM_GPIOPinConfigure(GPIO_PJ3_LCDDATA15);
    ROM_GPIOPinConfigure(GPIO_PJ6_LCDAC);
    ROM_GPIOPinConfigure(GPIO_PN6_LCDDATA13);
    ROM_GPIOPinConfigure(GPIO_PN7_LCDDATA12);
    ROM_GPIOPinConfigure(GPIO_PR0_LCDCP);
    ROM_GPIOPinConfigure(GPIO_PR1_LCDFP);
    ROM_GPIOPinConfigure(GPIO_PR2_LCDLP);
    ROM_GPIOPinConfigure(GPIO_PR3_LCDDATA03);
    ROM_GPIOPinConfigure(GPIO_PR4_LCDDATA00);
    ROM_GPIOPinConfigure(GPIO_PR5_LCDDATA01);
    ROM_GPIOPinConfigure(GPIO_PR6_LCDDATA04);
    ROM_GPIOPinConfigure(GPIO_PR7_LCDDATA05);
    ROM_GPIOPinConfigure(GPIO_PS4_LCDDATA06);
    ROM_GPIOPinConfigure(GPIO_PS5_LCDDATA07);
    ROM_GPIOPinConfigure(GPIO_PS6_LCDDATA08);
    ROM_GPIOPinConfigure(GPIO_PS7_LCDDATA09);
    ROM_GPIOPinConfigure(GPIO_PT0_LCDDATA10);
    ROM_GPIOPinConfigure(GPIO_PT1_LCDDATA11);

    GPIOPinTypeLCD(GPIO_PORTF_BASE, GPIO_PIN_7);
    GPIOPinTypeLCD(GPIO_PORTJ_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6);
    GPIOPinTypeLCD(GPIO_PORTN_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeLCD(GPIO_PORTR_BASE,
                       (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7));
    GPIOPinTypeLCD(GPIO_PORTS_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeLCD(GPIO_PORTT_BASE, GPIO_PIN_0 | GPIO_PIN_1);

}
