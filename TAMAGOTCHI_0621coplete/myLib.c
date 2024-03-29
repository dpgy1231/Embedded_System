#include "myLib.h"
#include "cortex_m4.h"

#define DMA_EN
#define QSPI_EN
#define QSPI_IMAGE_PAGE  1020
#define QSPI_PAGE_SIZE    256


extern uint32_t g_ui32SysClock;


void RTC_Init(void) {
	//Hibernation Control(p.590)
	HIBCTL = HIBCTL | 0x40;
	while (!(HIBCTL & 0x80000000));
	//Hibernation Peripheral Present(p.343)
	PPHIB = 1;

	//Hibernation Control(p.590)
	HIBCTL = HIBCTL | 0x141;
	while (!(HIBCTL & 0x80000000));
	HIBCALCTL = 0x5;

	//(p.619)
	HIBLOCK = 0xA3359554;
	while (!(HIBCTL & 0x80000000));
	HIBCALLD0 = (12 << 16) | (0 << 8) | 0;
	while (!(HIBCTL & 0x80000000));
	HIBCALLD1 = (0x0 << 24) | (15 << 16) | (3 << 8) | 2;
	while (!(HIBCTL & 0x80000000));

	HIBLOCK = 0;
	while (!(HIBCTL & 0x80000000));


	RCGCUART = RCGCUART | UART0;
	RCGCHIB = 1;
	RCGCGPIO = RCGCGPIO | GPIO_RUN_A | GPIO_RUN_P;

	GPIO_PORTP_DIR = GPIO_PORTP_DIR & ~(PIN1);
	GPIO_PORTA_AFSEL = GPIO_PORTA_AFSEL | PIN0 | PIN1;
	GPIO_PORTP_AFSEL = GPIO_PORTP_AFSEL & (~PIN1);
	GPIO_PORTA_PC = GPIO_PORTA_PC | 0xF;

	GPIO_PORTA_DR2R = GPIO_PORTA_DR2R & (~(PIN0 | PIN1));
	GPIO_PORTA_DR4R = GPIO_PORTA_DR4R | PIN0 | PIN1;
	GPIO_PORTA_DR8R = GPIO_PORTA_DR8R & (~(PIN0 | PIN1));
	GPIO_PORTA_DR12R = GPIO_PORTA_DR12R & (~(PIN0 | PIN1));
	GPIO_PORTA_SLR = GPIO_PORTA_SLR & (~(PIN0 | PIN1));

	GPIO_PORTA_PCTL = GPIO_PORTA_PCTL | 0x11;
	GPIO_PORTA_DEN = GPIO_PORTA_DEN | PIN0 | PIN1;
	GPIO_PORTP_DEN = GPIO_PORTP_DEN | PIN1;
}

void BUZZER_clear(){
	GPTMTAILR = 0;
	GPTMTMATCHR = 0;
}
void Play(int freq){
	GPTMTAILR = freq;
	GPTMTMATCHR = freq/2;
}
void DelayForPlay(int DLY){
	//Watchdog Load(p.1185)
	WDT1LOAD = DLY * 1000000;
	while(!(WDT1CTL & 0x80000000));
	//Watchdog Control(p.1187)
	WDT1CTL = 0x1;

	while(WDT1VALUE > 20);
	WDT1CTL = 0x0;
	WDT1ICR = 0x1;
}

void LED_clear(){
	GPIO_WRITE(GPIO_PORTL, 0xf, 0x0);
	GPIO_WRITE(GPIO_PORTM, 0xf, 0x0);
	delay(2500000);
}



void FND_clear(){
	GPIO_WRITE(GPIO_PORTC, 0x10 | 0x20 | 0x40 | 0x80, 0);
	GPIO_WRITE(GPIO_PORTG, 0x01 | 0x02 | 0x04 | 0x08, 0);

	GPIO_WRITE(GPIO_PORTH, 0x01 | 0x02 | 0x04 | 0x08, 0);
	GPIO_WRITE(GPIO_PORTN, 0x10 | 0x20, 0);
}

void LED_init(){
	//GPIO Run Mode Clock Gating control(p. 400)
	RCGCGPIO = RCGCGPIO | 0xC00;

	//direction (p.795)
	GPIO_PORTL_DIR = GPIO_PORTL_DIR | 0xF;
	GPIO_PORTM_DIR = GPIO_PORTM_DIR |0xF;

	//Digital enable (p.817)
	GPIO_PORTL_DEN = GPIO_PORTL_DEN | 0xF;
	GPIO_PORTM_DEN = GPIO_PORTM_DEN | 0xF;

	//Alternate Function Select(p.806)
	GPIO_PORTL_AFSEL = GPIO_PORTL_AFSEL & (~0xf);
	GPIO_PORTM_AFSEL = GPIO_PORTM_AFSEL & (~0xf);

	//port control register (p.823)
	GPIO_PORTL_PCTL = GPIO_PORTL_PCTL | 0x00005555;
	GPIO_PORTM_PCTL = GPIO_PORTM_PCTL | 0x00005555;

	//peripheral configuration (p.836)
	GPIO_PORTL_PC = GPIO_PORTL_PC | 0x00FF;
	GPIO_PORTM_PC = GPIO_PORTM_PC | 0x00FF;

	//drive control register setting(slew rate control) (p.808~810, p.816, p.828)
	GPIO_PORTL_DR2R = GPIO_PORTL_DR2R & (~0xf);
	GPIO_PORTM_DR2R = GPIO_PORTM_DR2R & (~0xf);

	GPIO_PORTL_DR4R = GPIO_PORTL_DR4R | 0xf;
	GPIO_PORTM_DR4R = GPIO_PORTM_DR4R | 0xf;

	GPIO_PORTL_DR8R = GPIO_PORTL_DR8R | 0xf;
	GPIO_PORTM_DR8R = GPIO_PORTM_DR8R | 0xf;

	GPIO_PORTL_SLR = GPIO_PORTL_SLR & (~0xf);
	GPIO_PORTM_SLR = GPIO_PORTM_SLR & (~0xf);

	GPIO_PORTL_DR12R = GPIO_PORTL_DR12R & (~0xf);
	GPIO_PORTM_DR12R = GPIO_PORTM_DR12R & (~0xf);
}
/*
void DIP_init(){
	RCGCGPIO = RCGCGPIO | 0x4043 ;

	GPIO_PORTA_DIR = GPIO_PORTA_DIR & (~0xf);
	GPIO_PORTB_DIR = GPIO_PORTB_DIR & (~0xf);
	GPIO_PORTQ_DIR = GPIO_PORTQ_DIR & (~0xf);
	GPIO_PORTG_DIR = GPIO_PORTG_DIR & (~0xf);


	GPIO_PORTA_DEN = (0x01<<3) | (0x01<<6) | (0x01<<7);
	GPIO_PORTB_DEN = 0x01<<3;
	GPIO_PORTQ_DEN = (0x01<<6) | (0x01<<5) | (0x01<<4);
	GPIO_PORTG_DEN = 0x01<<6;

	GPIO_PORTA_AFSEL = GPIO_PORTA_AFSEL & (~ ((0x01<<3) | (0x01<<6) | (0x01<<7)));
	GPIO_PORTB_AFSEL = GPIO_PORTB_AFSEL & (~ (0x01<<3));
	GPIO_PORTQ_AFSEL = GPIO_PORTQ_AFSEL & (~ ((0x01<<6) | (0x01<<5) | (0x01<<4)));
	GPIO_PORTG_AFSEL = GPIO_PORTG_AFSEL & (~ (0x01<<6));
}
*/
void PUSH_init(){

	RCGCGPIO = RCGCGPIO | GPIO_RUN_P | GPIO_RUN_N | GPIO_RUN_E | GPIO_RUN_K;

	GPIO_PORTP_DIR = GPIO_PORTP_DIR & ~(0x01<<1);
	GPIO_PORTN_DIR = GPIO_PORTN_DIR & ~(0x01<<3);
	GPIO_PORTE_DIR = GPIO_PORTE_DIR & ~(0x01<<5);
	GPIO_PORTK_DIR = GPIO_PORTK_DIR & ~(0x01<<7);

	GPIO_PORTP_AFSEL = GPIO_PORTP_AFSEL & (~ (0x01<<1));
	GPIO_PORTN_AFSEL = GPIO_PORTN_AFSEL & (~ (0x01<<3));
	GPIO_PORTE_AFSEL = GPIO_PORTE_AFSEL & (~ (0x01<<5));
	GPIO_PORTK_AFSEL = GPIO_PORTK_AFSEL & (~ (0x01<<7));

	GPIO_PORTP_DEN = GPIO_PORTP_DEN | (0x01<<1);
	GPIO_PORTN_DEN = GPIO_PORTN_DEN | (0x01<<3);
	GPIO_PORTE_DEN = GPIO_PORTE_DEN | (0x01<<5);
	GPIO_PORTK_DEN = GPIO_PORTK_DEN | (0x01<<7);
}

void FND_init(){

	RCGCGPIO = RCGCGPIO | 0x10C4;

	GPIO_PORTH_DIR = GPIO_PORTH_DIR | 0x0f;
	GPIO_PORTN_DIR = GPIO_PORTN_DIR | 0x30;
	GPIO_PORTC_DIR = GPIO_PORTC_DIR | 0xf0;
	GPIO_PORTG_DIR = GPIO_PORTG_DIR | 0x0f;

	GPIO_PORTH_DEN = GPIO_PORTH_DEN | 0x0f;
	GPIO_PORTN_DEN = GPIO_PORTN_DEN | 0x30;
	GPIO_PORTC_DEN = GPIO_PORTC_DEN | 0xf0;
	GPIO_PORTG_DEN = GPIO_PORTG_DEN | 0x0f;

	GPIO_PORTH_AFSEL = GPIO_PORTH_AFSEL & (~ (0x0F));
	GPIO_PORTN_AFSEL = GPIO_PORTN_AFSEL & (~ (0x30));
	GPIO_PORTC_AFSEL = GPIO_PORTC_AFSEL & (~ (0xF0));
	GPIO_PORTG_AFSEL = GPIO_PORTG_AFSEL & (~ (0x0F));

	GPIO_PORTH_PC = GPIO_PORTH_PC | 0x00FF;
	GPIO_PORTN_PC = GPIO_PORTN_PC | 0x0F00;
	GPIO_PORTC_PC = GPIO_PORTC_PC | 0xFF00;
	GPIO_PORTG_PC = GPIO_PORTG_PC | 0x00FF;

	GPIO_PORTH_DR2R = GPIO_PORTH_DR2R & (~(0x0F));
	GPIO_PORTN_DR2R = GPIO_PORTN_DR2R & (~(0x30));
	GPIO_PORTC_DR2R = GPIO_PORTC_DR2R & (~(0xF0));
	GPIO_PORTG_DR2R = GPIO_PORTG_DR2R & (~(0x0F));

	GPIO_PORTH_DR4R = GPIO_PORTH_DR4R | 0x0F;
	GPIO_PORTN_DR4R = GPIO_PORTN_DR4R | 0x30;
	GPIO_PORTC_DR4R = GPIO_PORTC_DR4R | 0xF0;
	GPIO_PORTG_DR4R = GPIO_PORTG_DR4R | 0x0F;

	GPIO_PORTH_DR8R = GPIO_PORTH_DR8R | 0x0F;
	GPIO_PORTN_DR8R = GPIO_PORTN_DR8R | 0x30;
	GPIO_PORTC_DR8R = GPIO_PORTC_DR8R | 0xF0;
	GPIO_PORTG_DR8R = GPIO_PORTG_DR8R | 0x0F;

	GPIO_PORTH_SLR = GPIO_PORTH_SLR & (~(0x0F));
	GPIO_PORTN_SLR = GPIO_PORTN_SLR & (~(0x30));
	GPIO_PORTC_SLR = GPIO_PORTC_SLR & (~(0xF0));
	GPIO_PORTG_SLR = GPIO_PORTG_SLR & (~(0x0F));

	GPIO_PORTH_DR12R = GPIO_PORTH_DR12R & (~(0x0F));
	GPIO_PORTN_DR12R = GPIO_PORTN_DR12R & (~(0x30));
	GPIO_PORTC_DR12R = GPIO_PORTC_DR12R & (~(0xF0));
	GPIO_PORTG_DR12R = GPIO_PORTG_DR12R & (~(0x0F));
}

void LCD_Init(unsigned long g_ui32SysClock) {
	static uint8_t pui8DMAControlTable[1024];
	tLCDIDDTiming sTimings;
	unsigned long ulClockMS;

	ulClockMS = g_ui32SysClock / (3 * 1000);

	PinoutSet();
	SysCtlPeripheralEnable(SYSCTL_PERIPH_LCD0);

#ifdef DMA_EN
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
#endif

	SysCtlDelay(50 * ulClockMS);
	LCDModeSet(LCD0_BASE, LCD_MODE_LIDD, g_ui32SysClock, g_ui32SysClock);
	LCDDMAConfigSet(LCD0_BASE, LCD_DMA_BURST_4);
	LCDIDDConfigSet(LCD0_BASE, LIDD_CONFIG_ASYNC_MPU80);
	sTimings.ui8WSSetup = CYCLES_FROM_TIME_NS(g_ui32SysClock, 4);
	sTimings.ui8WSDuration = CYCLES_FROM_TIME_NS(g_ui32SysClock, 10);
	sTimings.ui8WSHold = CYCLES_FROM_TIME_NS(g_ui32SysClock, 1);
	sTimings.ui8RSSetup = CYCLES_FROM_TIME_NS(g_ui32SysClock, 0);
	sTimings.ui8RSDuration = CYCLES_FROM_TIME_NS(g_ui32SysClock, 45);
	sTimings.ui8RSHold = CYCLES_FROM_TIME_NS(g_ui32SysClock, 1);
	sTimings.ui8DelayCycles = CYCLES_FROM_TIME_NS(g_ui32SysClock, 35);
	LCDIDDTimingSet(LCD0_BASE, 0, &sTimings);

	//
	// Enter sleep mode (if we are not already there).
	//
	WriteCommand(0x0001);	// software reset

	SysCtlDelay(ulClockMS);

	WriteCommand(0x00E2);	//PLL multiplier, set PLL clock to 120M
	WriteData(0x0014);	//29
	WriteData(0x0003);
	WriteData(0x0004);

	WriteCommand(0x00E0);  // PLL enable
	WriteData(0x0001);

	SysCtlDelay(ulClockMS / 10);

	WriteCommand(0x00E0);
	WriteData(0x0003);

	SysCtlDelay(ulClockMS / 5);

	WriteCommand(0x00E6);	//PLL setting for PCLK, depends on resolution
	WriteData(0x0003);	//04
	WriteData(0x00A0);	//B9
	WriteData(0x0000);	//38

	WriteCommand(0x00B0);	//LCD SPECIFICATION
	WriteData(0x0020);	// 24-bit
	WriteData(0x0000);	// TFT-mode
	WriteData(0x01); 	//Set HDP
	WriteData(0xDF);
	WriteData(0x01);
	WriteData(0x0F);
	WriteData(0x00);	// RGB

	WriteCommand(0x00B4);	//HSYNC
	WriteData(0x02);
	WriteData(0x0D);
	WriteData(0x00);
	WriteData(0x25);
	WriteData(0x02);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);

	WriteCommand(0x00B6);	//VSYNC
	WriteData(0x01);
	WriteData(0x1E);
	WriteData(0x00);
	WriteData(0x08);
	WriteData(0x01);
	WriteData(0x00);
	WriteData(0x00);

	WriteCommand(0xB8);
	WriteData(0x0007);	//GPIO3=input, GPIO[2:0]=output
	WriteData(0x0001);	//GPIO0 normal

	WriteCommand(0xBA);
	WriteData(0x000F);	//GPIO[3:0] out 1

	WriteCommand(0x36);	//rotation
	WriteData(0x0001);

	WriteCommand(0x00F0); //pixel data interface
	WriteData(0x0003);

	SysCtlDelay(ulClockMS / 2);

	WriteCommand(0x00BE); //set PWM for B/L
	WriteData(0x0001); //06 frequency
	WriteData(0x00FF); //C0 duty
	WriteData(0x0009); //bit[3]DBC enable,bit[0]PWM enable
	WriteData(0x00FF); //C0 DBC manual brightness
	WriteData(0x0088); //10 DBC minimum brightness
	WriteData(0x0000); //08 Brightness prescaler

	WriteCommand(0x00d4);
	WriteData(0x0000);
	WriteData(0x001C);
	WriteData(0x0020);
	WriteData(0x0000);
	WriteData(0x0046);
	WriteData(0x0050);
	WriteData(0x0000);
	WriteData(0x00A8);
	WriteData(0x00C0);

	WriteCommand(0x00d0);
	WriteData(0x0001); //DBC disable

	//
	// Set the X address of the display cursor.
	//
	WriteCommand(LCD_X_RAM_ADDR_REG);
	WriteData(0);
	WriteData(0);
	WriteData(HDP_S >> 8);
	WriteData(HDP_S & 0xff);
	//
	// Set the Y address of the display cursor.
	//
	WriteCommand(LCD_Y_RAM_ADDR_REG);
	WriteData(0);
	WriteData(0);
	WriteData(VDP_S >> 8);
	WriteData(VDP_S & 0xFF);

	//
	// Write the pixel value.
	//
	WriteCommand(LCD_RAM_DATA_REG);
	for (ulClockMS = 0; ulClockMS < (LCD_HEIGHT * LCD_WIDTH); ++ulClockMS) {
		WriteData(0x07E0);
	}
	WriteCommand(0x0029); //display on


	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_5);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_5, GPIO_DIR_MODE_OUT);

#ifdef DMA_EN
    uDMAEnable();
    uDMAControlBaseSet(&pui8DMAControlTable[0]);
    uDMAChannelAttributeDisable(UDMA_CHANNEL_SW, UDMA_ATTR_ALL);
#endif

#ifdef QSPI_EN
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
	SysCtlPeripheralReset(SYSCTL_PERIPH_SSI3);
	MX66L51235FInit(g_ui32SysClock);

#endif
}

inline uint16_t ReadData(void) {
	//
	// Split the write into two bytes and pass them to the LCD controller.
	//
	//LCDIDDDataWrite(LCD0_BASE, 0, ui16Data);
	return LCDIDDDataRead(LCD0_BASE, 0);
	//LCDIDDDataWrite(LCD0_BASE, 0, ui16Data & 0xff);
}

inline void WriteData(uint16_t ui16Data) {
	//
	// Split the write into two bytes and pass them to the LCD controller.
	//
	LCDIDDDataWrite(LCD0_BASE, 0, ui16Data);
	//LCDIDDDataWrite(LCD0_BASE, 0, ui16Data & 0xff);
}

inline void WriteCommand(uint16_t ui8Data) {
	//
	// Pass the write on to the controller.
	//
	LCDIDDCommandWrite(LCD0_BASE, 0, ui8Data);
}
void Port_Init(void) {
	//GPIO Run Mode Clock Gating control(p.400)
	RCGCGPIO = RCGCGPIO | GPIO_RUN_F | GPIO_RUN_J | GPIO_RUN_N | GPIO_RUN_R
			| GPIO_RUN_S | GPIO_RUN_T;

	//LCD Run Mode Clock Gating control(p.420)
	RCGCLCD = 1;

	//direction (p.795)
	GPIO_PORTF_DIR = GPIO_PORTF_DIR & (~(PIN7));
	GPIO_PORTJ_DIR = GPIO_PORTJ_DIR & (~(PIN2 | PIN3 | PIN6));
	GPIO_PORTN_DIR = GPIO_PORTN_DIR & (~(PIN6 | PIN7));
	GPIO_PORTR_DIR = GPIO_PORTR_DIR
			& (~(PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6 | PIN7));
	GPIO_PORTS_DIR = GPIO_PORTS_DIR & (~(PIN4 | PIN5 | PIN6 | PIN7));
	GPIO_PORTT_DIR = GPIO_PORTT_DIR & (~(PIN0 | PIN1));

	//Alternate Function Select(p.806)
	GPIO_PORTF_AFSEL = GPIO_PORTF_AFSEL | PIN7;
	GPIO_PORTJ_AFSEL = GPIO_PORTJ_AFSEL | PIN2 | PIN3 | PIN6;
	GPIO_PORTN_AFSEL = GPIO_PORTN_AFSEL | PIN6 | PIN7;
	GPIO_PORTR_AFSEL = GPIO_PORTR_AFSEL | PIN0 | PIN1 | PIN2 | PIN3 | PIN4
			| PIN5 | PIN6 | PIN7;
	GPIO_PORTS_AFSEL = GPIO_PORTS_AFSEL | PIN4 | PIN5 | PIN6 | PIN7;
	GPIO_PORTT_AFSEL = GPIO_PORTT_AFSEL | PIN0 | PIN1;

	//port control register (p.823)
	GPIO_PORTF_PCTL = 0xF0000000;
	GPIO_PORTJ_PCTL = 0x0F00FF00;
	GPIO_PORTN_PCTL = 0xFF000000;
	GPIO_PORTR_PCTL = 0xFFFFFFFF;
	GPIO_PORTS_PCTL = 0xFFFF0000;
	GPIO_PORTT_PCTL = 0x000000FF;

	//peripheral configuration (p.836)
	GPIO_PORTF_PC = GPIO_PORTF_PC | 0xC000;
	GPIO_PORTJ_PC = GPIO_PORTJ_PC | 0x30F0;
	GPIO_PORTN_PC = GPIO_PORTN_PC | 0xF000;
	GPIO_PORTR_PC = GPIO_PORTR_PC | 0xFFFF;
	GPIO_PORTS_PC = GPIO_PORTS_PC | 0xFF00;
	GPIO_PORTT_PC = GPIO_PORTT_PC | 0x000F;

	//drive control register setting(slew rate control) (p.808~810, p.816, p.828)
	GPIO_PORTF_DR2R = GPIO_PORTF_DR2R & (~(PIN7));
	GPIO_PORTJ_DR2R = GPIO_PORTJ_DR2R & (~(PIN2 | PIN3 | PIN6));
	GPIO_PORTN_DR2R = GPIO_PORTN_DR2R & (~(PIN6 | PIN7));
	GPIO_PORTR_DR2R = GPIO_PORTR_DR2R
			& (~(PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6 | PIN7));
	GPIO_PORTS_DR2R = GPIO_PORTS_DR2R & (~(PIN4 | PIN5 | PIN6 | PIN7));
	GPIO_PORTT_DR2R = GPIO_PORTT_DR2R & (~(PIN0 | PIN1));

	GPIO_PORTF_DR4R = GPIO_PORTF_DR4R | PIN7;
	GPIO_PORTJ_DR4R = GPIO_PORTJ_DR4R | PIN2 | PIN3 | PIN6;
	GPIO_PORTN_DR4R = GPIO_PORTN_DR4R | PIN6 | PIN7;
	GPIO_PORTR_DR4R = GPIO_PORTR_DR4R | PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN5
			| PIN6 | PIN7;
	GPIO_PORTS_DR4R = GPIO_PORTS_DR4R | PIN4 | PIN5 | PIN6 | PIN7;
	GPIO_PORTT_DR4R = GPIO_PORTT_DR4R | PIN0 | PIN1;

	GPIO_PORTF_DR8R = GPIO_PORTF_DR8R | PIN7;
	GPIO_PORTJ_DR8R = GPIO_PORTJ_DR8R | PIN2 | PIN3 | PIN6;
	GPIO_PORTN_DR8R = GPIO_PORTN_DR8R | PIN6 | PIN7;
	GPIO_PORTR_DR8R = GPIO_PORTR_DR8R | PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN5
			| PIN6 | PIN7;
	GPIO_PORTS_DR8R = GPIO_PORTS_DR8R | PIN4 | PIN5 | PIN6 | PIN7;
	GPIO_PORTT_DR8R = GPIO_PORTT_DR8R | PIN0 | PIN1;

	GPIO_PORTF_SLR = GPIO_PORTF_SLR & (~(PIN7));
	GPIO_PORTJ_SLR = GPIO_PORTJ_SLR & (~(PIN2 | PIN3 | PIN6));
	GPIO_PORTN_SLR = GPIO_PORTN_SLR & (~(PIN6 | PIN7));
	GPIO_PORTR_SLR = GPIO_PORTR_SLR
			& (~(PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6 | PIN7));
	GPIO_PORTS_SLR = GPIO_PORTS_SLR & (~(PIN4 | PIN5 | PIN6 | PIN7));
	GPIO_PORTT_SLR = GPIO_PORTT_SLR & (~(PIN0 | PIN1));

	GPIO_PORTF_DR12R = GPIO_PORTF_DR12R & (~(PIN7));
	GPIO_PORTJ_DR12R = GPIO_PORTJ_DR12R & (~(PIN2 | PIN3 | PIN6));
	GPIO_PORTN_DR12R = GPIO_PORTN_DR12R & (~(PIN6 | PIN7));
	GPIO_PORTR_DR12R = GPIO_PORTR_DR12R
			& (~(PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6 | PIN7));
	GPIO_PORTS_DR12R = GPIO_PORTS_DR12R & (~(PIN4 | PIN5 | PIN6 | PIN7));
	GPIO_PORTT_DR12R = GPIO_PORTT_DR12R & (~(PIN0 | PIN1));

	//Digital enable (p.817)
	GPIO_PORTF_DEN = PIN7;
	GPIO_PORTJ_DEN = PIN2 | PIN3 | PIN6;
	GPIO_PORTN_DEN = PIN6 | PIN7;
	GPIO_PORTR_DEN = PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6 | PIN7;
	GPIO_PORTS_DEN = PIN4 | PIN5 | PIN6 | PIN7;
	GPIO_PORTT_DEN = PIN0 | PIN1;
	///////////////////////////////////////////////////////////////////////////////


}

void BUZZER_init(){
	RCGCGPIO = RCGCGPIO | 0xa;

	GPIO_PORTD_DIR = GPIO_PORTD_DIR | 0x10;

	//(p.806)
	GPIO_PORTB_AFSEL = GPIO_PORTB_AFSEL | 0x4;
	GPIO_PORTD_AFSEL = GPIO_PORTD_AFSEL & (~ (0x10));

	//peripheral configuration (p.836)
	GPIO_PORTB_PC = GPIO_PORTB_PC | 0x30;
	GPIO_PORTD_PC = GPIO_PORTD_PC | 0x300;

	//drive control register setting(slew rate control) (p.808~810, p.816, p.828)
	GPIO_PORTB_DR2R = GPIO_PORTB_DR2R & (~(0x4));
	GPIO_PORTD_DR2R = GPIO_PORTD_DR2R & (~(0x10));

	GPIO_PORTB_DR4R = GPIO_PORTB_DR4R | 0x4;
	GPIO_PORTD_DR4R = GPIO_PORTD_DR4R | 0x10;

	GPIO_PORTB_DR8R = GPIO_PORTB_DR8R | 0x4;
	GPIO_PORTD_DR8R = GPIO_PORTD_DR8R & (~(0x10));

	GPIO_PORTB_SLR = GPIO_PORTB_SLR & (~(0x4));
	GPIO_PORTD_SLR = GPIO_PORTD_SLR & (~(0x10));;

	GPIO_PORTB_DR12R = GPIO_PORTB_DR12R & (~(0x4));
	GPIO_PORTD_DR12R = GPIO_PORTD_DR12R & (~(0x10));

	//port control register (p.823)
	GPIO_PORTB_PCTL = 0x300;
	GPIO_PORTD_PCTL = GPIO_PORTD_PCTL & (~(0xF0000));

	//Digital enable (p.817)
	GPIO_PORTB_DEN = 0x4;
	GPIO_PORTD_DEN = 0x10;
}
void TIMER_init(){
	// PWM
	//Timer Run Mode Clock Gating control(p.398)
	RCGCTIMER = RCGCTIMER | 0x20;
	//Timer Clock Configuration(p.1180)
	GPTMCC = 0x1;

	//Timer Configuration(p.1129)
	GPTMCFG = 0x4;

	//Timer Mode(p.1130)
	GPTMTAMR = 0xA;

	//TIMER Control(p.1139)
	GPTMCTL = 0x40; // 0x41 . timer enable

	//Timer Prescale(p.1161)
	GPTMTAPR = 0x0;

	// WDT
	//Watchdog Timer Run Mode Clock Gating control(p.397)
	RCGCWD = RCGCWD | 0x2;

	//interrupt set enable(p.166, p.127)
	INTEN0 = 0x1<<18;

	//Alternate Clock Configuration(p.293)
	ALTCLKCFG = 0x0;

	RCGCGPIO = RCGCGPIO | 0x2020;

	//Alternate Function Select(p.806)
	GPIO_PORTF_AFSEL = GPIO_PORTF_AFSEL & (~ 0x2);
	GPIO_PORTP_AFSEL = GPIO_PORTP_AFSEL & (~ 0x2);

	//direction (p.795)
	GPIO_PORTF_DIR = GPIO_PORTF_DIR | 0x2;
	GPIO_PORTP_DIR = GPIO_PORTP_DIR & ~(0x2);

	GPIO_PORTF_PC = GPIO_PORTF_PC | 0xC;
	GPIO_PORTF_DR2R = GPIO_PORTF_DR2R & (~PIN1);
	GPIO_PORTF_DR4R = GPIO_PORTF_DR4R | 0x2;

	GPIO_PORTF_DR8R = GPIO_PORTF_DR8R | 0x2;
	GPIO_PORTF_SLR = GPIO_PORTF_SLR & (~0x2);
	GPIO_PORTF_DR12R = GPIO_PORTF_DR12R & (~0x2);

	GPIO_PORTF_DEN = GPIO_PORTF_DEN | 0x2;
	GPIO_PORTP_DEN = GPIO_PORTP_DEN | 0x2;

}


void Uart_Init(float BRD, int BRDI, int BRDF) {
	//UART Control(p.1343)

	RCGCUART = RCGCUART | (0x01 << 0) | (0x01 << 3)/*(Bluetooth)*/;
	RCGCGPIO = RCGCGPIO | (0x01 << 0) | (0x01 << 8);
	GPIO_PORTA_AFSEL = GPIO_PORTA_AFSEL | (0x01 << 0) | (0x01 << 1);
	GPIO_PORTA_PC = GPIO_PORTA_PC | 0xF;
	GPIO_PORTA_DR2R = GPIO_PORTA_DR2R & (~((0x01 << 0) | (0x01 << 1)));
	GPIO_PORTA_DR4R = GPIO_PORTA_DR4R | (0x01 << 0) | (0x01 << 1);
	GPIO_PORTA_DR8R = GPIO_PORTA_DR8R & (~((0x01 << 0) | (0x01 << 1)));
	GPIO_PORTA_SLR = GPIO_PORTA_SLR & (~((0x01 << 0) | (0x01 << 1)));
	GPIO_PORTA_DR12R = GPIO_PORTA_DR12R & (~((0x01 << 0) | (0x01 << 1)));
	GPIO_PORTA_PCTL = 0x11;
	GPIO_PORTA_DEN = (0x01 << 0) | (0x01 << 1);

	UARTCTL_UART0 = 0x300; // [9:8] RX,TX Enable [0] UART Disable
	UARTIBRD_UART0 = BRDI; // Baud Rate Setting
	UARTFBRD_UART0 = BRDF; // Baud Rate Setting

	//Uart Line Control(p.1341)
	UARTLCRH_UART0 = 0x60; // [6:5] 8bit Data

	//UART clock source(p.1368)
	UARTCC_UART0 = 0;

	//UART Control
	UARTCTL_UART0 = 0x301; // UART Enable

	//UARTCTL_UART3 = 0x300;
	//UARTIBRD_UART3 = BRDI;
	//UARTFBRD_UART3 = BRDF;
	//UARTLCRH_UART3 = 0x60;
	//UARTCC_UART3 = 0;
	//UARTCTL_UART3 = 0x301;
}

char Uart_GetCh(void) {
	while (!(UART0FR & 0x40));
	return UART0_DATA;
}

char Uart_GetKey(void) {
	if (!(UART0FR & 0x40)) {
		return 0;
	} else
		return UART0_DATA;
}

void Uart_PutCh(uint8_t data) {
	UART0_DATA = data;
	while (!(UART0FR & 0x80));
}


void Uart_PutStr(char* pt) {
	while (*pt)
		Uart_PutCh(*pt++);
}

void Uart_Printf(char *fmt, ...) {
	va_list ap;
	char ch_buffer[256];

	va_start(ap, fmt);
	vsprintf(ch_buffer, (const char*) fmt, ap);

	Uart_PutStr(ch_buffer);
	va_end(ap);
}

void DrawImage(unsigned char *buffer, int x1, int y1, int x2, int y2, int image){
	int i;
	unsigned long ulClockMS;

	ulClockMS = g_ui32SysClock / (3 * 1000);

	WriteCommand(LCD_X_RAM_ADDR_REG);		// Set X
	WriteData(x1 >> 8);
	WriteData(x1 & 0xFF);
	WriteData(x2-1 >> 8);
	WriteData(x2-1 & 0xff);

	WriteCommand(LCD_Y_RAM_ADDR_REG);		// Set Y
	WriteData(y1 >> 8);
	WriteData(y1 & 0xFF);
	WriteData(y2-1 >> 8);
	WriteData(y2-1 & 0xff);

	WriteCommand(LCD_RAM_DATA_REG);

	for(i = y1; i <= y2; ++i){
		MX66L51235FRead(image + ((i-y1)*(x2-x1)*2), buffer, (x2-x1)*2);

		uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
		uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[0], (void *)0x44050018, (x2-x1)/2);
		uDMAChannelEnable(UDMA_CHANNEL_SW);
		uDMAChannelRequest(UDMA_CHANNEL_SW);
		SysCtlDelay(ulClockMS /DIV_MS);

		uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
		uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(x2-x1)], (void *)0x44050018, (x2-x1)/2);
		uDMAChannelEnable(UDMA_CHANNEL_SW);
		uDMAChannelRequest(UDMA_CHANNEL_SW);
		SysCtlDelay(ulClockMS /DIV_MS);
	}

	SetFullFrame();
}
void RestoreBackground(unsigned char *buffer, int x1, int y1, int x2, int y2, int image){
	int i;
	unsigned long ulClockMS;

	ulClockMS = g_ui32SysClock / (3 * 1000);

	WriteCommand(LCD_X_RAM_ADDR_REG);		// Set X
	WriteData(x1 >> 8);
	WriteData(x1 & 0xFF);
	WriteData(x2-1 >> 8);
	WriteData(x2-1 & 0xff);

	WriteCommand(LCD_Y_RAM_ADDR_REG);		// Set Y
	WriteData(y1 >> 8);
	WriteData(y1 & 0xFF);
	WriteData(y2-1 >> 8);
	WriteData(y2-1 & 0xff);

	WriteCommand(LCD_RAM_DATA_REG);

	for(i = y1; i <= y2; ++i){
		MX66L51235FRead(image + (i*480*2 + x1*2), buffer, (x2-x1)*2);

		uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
		uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[0], (void *)0x44050018, (x2-x1)/2);
		uDMAChannelEnable(UDMA_CHANNEL_SW);
		uDMAChannelRequest(UDMA_CHANNEL_SW);
		SysCtlDelay(ulClockMS /DIV_MS);

		uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
		uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(x2-x1)], (void *)0x44050018, (x2-x1)/2);
		uDMAChannelEnable(UDMA_CHANNEL_SW);
		uDMAChannelRequest(UDMA_CHANNEL_SW);
		SysCtlDelay(ulClockMS /DIV_MS);
	}

	SetFullFrame();
}

void PutPixel(unsigned char *buffer, int x, int y, int color){
	WriteCommand(LCD_X_RAM_ADDR_REG);		// Set X
	WriteData(x >> 8);
	WriteData(x & 0xFF);
	WriteData(x >> 8);
	WriteData(x & 0xff);

	WriteCommand(LCD_Y_RAM_ADDR_REG);		// Set Y
	WriteData(y >> 8);
	WriteData(y & 0xFF);
	WriteData(y >> 8);
	WriteData(y & 0xff);

	WriteCommand(LCD_RAM_DATA_REG);
	WriteData(color);
}

void DrawLine(unsigned char *buffer, int x1, int y1, int x2, int y2, int color){
	int dx,dy,e;
	dx=x2-x1;
	dy=y2-y1;

	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(buffer, x1, y1, color);

					if(e>0){y1+=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(buffer, x1, y1, color);

					if(e>0){x1+=1;e-=dy;}
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(buffer, x1, y1, color);

					if(e>0){y1-=1;e-=dx;}
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(buffer, x1, y1, color);

					if(e>0){x1+=1;e-=dy;}
					y1-=1;
					e+=dx;
				}
			}
		}
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(buffer, x1, y1, color);

					if(e>0){y1+=1;e-=dx;}
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(buffer, x1, y1, color);

					if(e>0){x1-=1;e-=dy;}
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(buffer, x1, y1, color);

					if(e>0){y1-=1;e-=dx;}
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(buffer, x1, y1, color);

					if(e>0){x1-=1;e-=dy;}
					y1-=1;
					e+=dx;
				}
			}
		}
	}

	SetFullFrame();
}

void DrawCircle(unsigned char *buffer, int nCenterX,int nCenterY,int nRadius,int color){
   int x,y;

   for(x = nCenterX - nRadius; x < nCenterX + nRadius ; x ++ )
      for( y = nCenterY - nRadius; y < nCenterY + nRadius; y++)
         if( ((nCenterX - x ) * (nCenterX-x)) +
            ((nCenterY - y ) * (nCenterY-y)) <=
               nRadius*nRadius &&
            ((nCenterX - x ) * (nCenterX-x)) +
            ((nCenterY - y ) * (nCenterY-y)) > ((nRadius-2) * (nRadius-2)))
                  PutPixel(buffer,x,y,color);

   SetFullFrame();
}

void DrawCircle_fill(unsigned char *buffer, int nCenterX,int nCenterY,int nRadius,int color){
   int x,y;

   for(x = nCenterX - nRadius; x < nCenterX + nRadius ; x ++ )
      for( y = nCenterY - nRadius; y < nCenterY + nRadius; y++)
         if( ((nCenterX - x ) * (nCenterX-x)) +
            ((nCenterY - y ) * (nCenterY-y)) <=
               nRadius*nRadius &&
            ((nCenterX - x ) * (nCenterX-x)) +
            ((nCenterY - y ) * (nCenterY-y)) > ((nRadius-nRadius) * (nRadius-nRadius)))
                  PutPixel(buffer,x,y,color);

   PutPixel(buffer,nCenterX,nCenterY,color);

   SetFullFrame();
}
/*
void DrawCircleimage_fill(unsigned char *buffer, int nCenterX,int nCenterY,int nRadius,int image){
   int x,y;
   int i;
   int x1,y1,x2,y2;
   x1=nCenterX-nRadius;
   x2=nCenterX+nRadius;
   y1=nCenterY-nRadius;
   y2=nCenterY+nRadius;

   unsigned long ulClockMS;

   	ulClockMS = g_ui32SysClock / (3 * 1000);

   	WriteCommand(LCD_X_RAM_ADDR_REG);		// Set X
   	WriteData(x1 >> 8);
   	WriteData(x1 & 0xFF);
   	WriteData(x2-1 >> 8);
   	WriteData(x2-1 & 0xff);

   	WriteCommand(LCD_Y_RAM_ADDR_REG);		// Set Y
   	WriteData(y1 >> 8);
   	WriteData(y1 & 0xFF);
   	WriteData(y2-1 >> 8);
   	WriteData(y2-1 & 0xff);

   	WriteCommand(LCD_RAM_DATA_REG);

   	for(i = y1; i <= y2; ++i){
   for(x = nCenterX - nRadius; x < nCenterX + nRadius ; x ++ ){
      for( y = nCenterY - nRadius; y < nCenterY + nRadius; y++){
         if( ((nCenterX - x ) * (nCenterX-x)) +
            ((nCenterY - y ) * (nCenterY-y)) <=
               nRadius*nRadius &&
            ((nCenterX - x ) * (nCenterX-x)) +
            ((nCenterY - y ) * (nCenterY-y)) > ((nRadius-nRadius) * (nRadius-nRadius))){
        	 MX66L51235FRead(image + ((i-y1)*(x2-x1)*2), buffer, (x2-x1)*2);

        	 		uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
        	 		uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[0], (void *)0x44050018, (x2-x1)/2);
        	 		uDMAChannelEnable(UDMA_CHANNEL_SW);
        	 		uDMAChannelRequest(UDMA_CHANNEL_SW);
        	 		SysCtlDelay(ulClockMS /DIV_MS);

        	 		uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
        	 		uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(x2-x1)], (void *)0x44050018, (x2-x1)/2);
        	 		uDMAChannelEnable(UDMA_CHANNEL_SW);
        	 		uDMAChannelRequest(UDMA_CHANNEL_SW);
        	 		SysCtlDelay(ulClockMS /DIV_MS);
         }}}}
  // PutPixel(buffer,nCenterX,nCenterY,image1);

   SetFullFrame();
}
/*
void DrawImage(unsigned char *buffer, int x1, int y1, int x2, int y2, int image){
	int i;
	unsigned long ulClockMS;

	ulClockMS = g_ui32SysClock / (3 * 1000);

	WriteCommand(LCD_X_RAM_ADDR_REG);		// Set X
	WriteData(x1 >> 8);
	WriteData(x1 & 0xFF);
	WriteData(x2-1 >> 8);
	WriteData(x2-1 & 0xff);

	WriteCommand(LCD_Y_RAM_ADDR_REG);		// Set Y
	WriteData(y1 >> 8);
	WriteData(y1 & 0xFF);
	WriteData(y2-1 >> 8);
	WriteData(y2-1 & 0xff);

	WriteCommand(LCD_RAM_DATA_REG);

	for(i = y1; i <= y2; ++i){
		MX66L51235FRead(image + ((i-y1)*(x2-x1)*2), buffer, (x2-x1)*2);

		uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
		uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[0], (void *)0x44050018, (x2-x1)/2);
		uDMAChannelEnable(UDMA_CHANNEL_SW);
		uDMAChannelRequest(UDMA_CHANNEL_SW);
		SysCtlDelay(ulClockMS /DIV_MS);

		uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
		uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(x2-x1)], (void *)0x44050018, (x2-x1)/2);
		uDMAChannelEnable(UDMA_CHANNEL_SW);
		uDMAChannelRequest(UDMA_CHANNEL_SW);
		SysCtlDelay(ulClockMS /DIV_MS);
	}

	SetFullFrame();
}
*/
void DrawRect_fill(unsigned char *buffer, int x1, int y1, int x2, int y2, int color){
   int i, j;

   WriteCommand(LCD_X_RAM_ADDR_REG);      // Set X
   WriteData(x1 >> 8);
   WriteData(x1 & 0xFF);
   WriteData(x2-1 >> 8);
   WriteData(x2-1 & 0xff);

   WriteCommand(LCD_Y_RAM_ADDR_REG);      // Set Y
   WriteData(y1 >> 8);
   WriteData(y1 & 0xFF);
   WriteData(y2-1 >> 8);
   WriteData(y2-1 & 0xff);

   WriteCommand(LCD_RAM_DATA_REG);

   for(i = y1; i < y2; ++i){
      for(j = x1; j < x2; ++j) {
         WriteData(color);
      }
   }

   SetFullFrame();
};

void Scroll_left(unsigned char *buffer, int x1, int y1, int x2, int y2, int scroll){
	int i, j;
	static unsigned char temp_buffer[LCD_HEIGHT * 20];
	unsigned long ulClockMS = g_ui32SysClock / (3 * 1000);

	WriteCommand(LCD_Y_RAM_ADDR_REG);
	WriteData(y1 >> 8);
	WriteData(y1 & 0xFF);
	WriteData(y2-1 >> 8);
	WriteData(y2-1 & 0xff);

	for(j = x1; j <= x2; j+=scroll){
		if(j != x2){
			WriteCommand(LCD_X_RAM_ADDR_REG);
			WriteData(j >> 8);
			WriteData(j & 0xFF);
			WriteData((j + scroll - 1) >> 8);
			WriteData((j + scroll - 1) & 0xff);

			WriteCommand(0x2E);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE |  UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, (void *)0x44050018,  &buffer[i*(y2-y1)*2], (y2-y1));
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}

		WriteCommand(LCD_X_RAM_ADDR_REG);
		if(j >= x2){
			WriteData((j - scroll)>>8);
			WriteData((j - scroll) & 0xFF);
			WriteData((j - 1) >> 8);
			WriteData((j - 1) & 0xff);

			WriteCommand(LCD_RAM_DATA_REG);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &temp_buffer[i*(y2-y1)*2], (void *)0x44050018, (y2-y1));
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}

		else if(j > x1){
			WriteData((j - scroll)>>8);
			WriteData((j - scroll) & 0xFF);
			WriteData((j - 1) >> 8);
			WriteData((j - 1) & 0xff);

			WriteCommand(LCD_RAM_DATA_REG);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[i*(y2-y1)*2], (void *)0x44050018, (y2-y1));
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}

		}

		else if(j == x1){
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[i*(y2-y1)*2], &temp_buffer[i*(y2-y1)*2], (y2-y1));
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}
	}

	SetFullFrame();
}

void Scroll_right(unsigned char *buffer, int x1, int y1, int x2, int y2, int scroll){
	int i, j;
	static unsigned char temp_buffer[LCD_HEIGHT * 20];
	unsigned long ulClockMS = g_ui32SysClock / (3 * 1000);

	WriteCommand(LCD_Y_RAM_ADDR_REG);
	WriteData(y1 >> 8);
	WriteData(y1 & 0xFF);
	WriteData(y2-1 >> 8);
	WriteData(y2-1 & 0xff);

	for(j = x2; j >= x1; j-=scroll){
		if(j != x1){
			WriteCommand(LCD_X_RAM_ADDR_REG);
			WriteData((j - scroll) >> 8);
			WriteData((j - scroll) & 0xFF);
			WriteData((j - 1) >> 8);
			WriteData((j - 1) & 0xff);

			WriteCommand(0x2E);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE |  UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, (void *)0x44050018,  &buffer[i*(y2-y1)*2], (y2-y1));
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}

		if(j == x2){
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[i*(y2-y1)*2], &temp_buffer[i*(y2-y1)*2], (y2-y1));
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}
		else if(j == x1){
			WriteCommand(LCD_X_RAM_ADDR_REG);
			WriteData(j >> 8);
			WriteData(j & 0xFF);
			WriteData((j + scroll - 1) >> 8);
			WriteData((j + scroll - 1) & 0xff);

			WriteCommand(LCD_RAM_DATA_REG);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &temp_buffer[i*(y2-y1)*2], (void *)0x44050018, (y2-y1));
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}
		else if(j < x2){
			WriteCommand(LCD_X_RAM_ADDR_REG);
			WriteData(j >> 8);
			WriteData(j & 0xFF);
			WriteData((j + scroll - 1) >> 8);
			WriteData((j + scroll - 1) & 0xff);

			WriteCommand(LCD_RAM_DATA_REG);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[i*(y2-y1)*2], (void *)0x44050018, (y2-y1));
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}
	}

	SetFullFrame();
}
void Scroll_down(unsigned char *buffer, int x1, int y1, int x2, int y2, int scroll){
	int i, j;
	static unsigned char temp_buffer[LCD_WIDTH * 20];
	unsigned long ulClockMS = g_ui32SysClock / (3 * 1000);

	WriteCommand(LCD_X_RAM_ADDR_REG);
	WriteData(x1 >> 8);
	WriteData(x1 & 0xFF);
	WriteData(x2-1 >> 8);
	WriteData(x2-1 & 0xff);

	for(j = y1; j <= y2; j+=scroll){
		if(j != y2){
			WriteCommand(LCD_Y_RAM_ADDR_REG);
			WriteData(j >> 8);
			WriteData(j & 0xFF);
			WriteData((j + scroll - 1) >> 8);
			WriteData((j + scroll - 1) & 0xff);

			WriteCommand(0x2E);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE |  UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, (void *)0x44050018,  &buffer[(i*2)*(x2-x1)], (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);

				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE |  UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, (void *)0x44050018,  &buffer[(i*2+1)*(x2-x1)], (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}

		WriteCommand(LCD_Y_RAM_ADDR_REG);
		if(j >= y2){
			WriteData((j - scroll) >> 8);
			WriteData((j - scroll) & 0xFF);
			WriteData((j - 1) >> 8);
			WriteData((j - 1) & 0xff);

			WriteCommand(LCD_RAM_DATA_REG);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &temp_buffer[(i*2)*(x2-x1)], (void *)0x44050018, (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);

				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &temp_buffer[(i*2+1)*(x2-x1)], (void *)0x44050018, (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}

		else if(j>y1){
			WriteData((j - scroll) >> 8);
			WriteData((j - scroll) & 0xFF);
			WriteData((j - 1) >> 8);
			WriteData((j - 1) & 0xff);

			WriteCommand(LCD_RAM_DATA_REG);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(i*2)*(x2-x1)], (void *)0x44050018, (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);

				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(i*2+1)*(x2-x1)], (void *)0x44050018, (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}

		}

		else if(j == y1){
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(i*2)*(x2-x1)], &temp_buffer[(i*2)*(x2-x1)], (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);

				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(i*2+1)*(x2-x1)], &temp_buffer[(i*2+1)*(x2-x1)], (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}
	}

	SetFullFrame();
}

void Scroll_up(unsigned char *buffer, int x1, int y1, int x2, int y2, int scroll){
	int i, j;
	static unsigned char temp_buffer[LCD_WIDTH * 20];
	unsigned long ulClockMS = g_ui32SysClock / (3 * 1000);

	WriteCommand(LCD_X_RAM_ADDR_REG);
	WriteData(x1 >> 8);
	WriteData(x1 & 0xFF);
	WriteData(x2-1 >> 8);
	WriteData(x2-1 & 0xff);

	for(j = y2; j >= y1; j-=scroll){
		if(j != y1){
			WriteCommand(LCD_Y_RAM_ADDR_REG);
			WriteData((j - scroll) >> 8);
			WriteData((j - scroll) & 0xFF);
			WriteData((j - 1) >> 8);
			WriteData((j - 1) & 0xff);

			WriteCommand(0x2E);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE |  UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, (void *)0x44050018,  &buffer[(i*2)*(x2-x1)], (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);

				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_NONE |  UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, (void *)0x44050018,  &buffer[(i*2+1)*(x2-x1)], (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}

		WriteCommand(LCD_Y_RAM_ADDR_REG);
		if(j == y2){
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(i*2)*(x2-x1)], &temp_buffer[(i*2)*(x2-x1)], (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);

				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_16 | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(i*2+1)*(x2-x1)], &temp_buffer[(i*2+1)*(x2-x1)], (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}
		else if(j <= y1){
			WriteData(j >> 8);
			WriteData(j & 0xFF);
			WriteData((j + scroll - 1) >> 8);
			WriteData((j + scroll - 1) & 0xff);

			WriteCommand(LCD_RAM_DATA_REG);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &temp_buffer[(i*2)*(x2-x1)], (void *)0x44050018, (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);

				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &temp_buffer[(i*2+1)*(x2-x1)], (void *)0x44050018, (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}
		}
		else if(j){
			WriteData(j >> 8);
			WriteData(j & 0xFF);
			WriteData((j + scroll - 1) >> 8);
			WriteData((j + scroll - 1) & 0xff);

			WriteCommand(LCD_RAM_DATA_REG);
			for(i = 0; i < scroll; i++){
				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(i*2)*(x2-x1)], (void *)0x44050018, (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);

				uDMAChannelControlSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8);
				uDMAChannelTransferSet(UDMA_CHANNEL_SW | UDMA_PRI_SELECT, UDMA_MODE_AUTO, &buffer[(i*2+1)*(x2-x1)], (void *)0x44050018, (x2-x1)/2);
				uDMAChannelEnable(UDMA_CHANNEL_SW);
				uDMAChannelRequest(UDMA_CHANNEL_SW);
				SysCtlDelay(ulClockMS /DIV_MS);
			}

		}
	}

	SetFullFrame();
}


void SetFullFrame(){
	WriteCommand(LCD_X_RAM_ADDR_REG);
	WriteData(0);
	WriteData(0);
	WriteData(479 >> 8);
	WriteData(479 & 0xff);

	WriteCommand(LCD_Y_RAM_ADDR_REG);
	WriteData(0 >> 8);
	WriteData(0 & 0xFF);
	WriteData(271 >> 8);
	WriteData(271 & 0xff);
}

