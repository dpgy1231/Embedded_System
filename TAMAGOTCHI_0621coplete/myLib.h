/*
 * myLib.h
 *
 *  Created on: 2015. 2. 25.
 *      Author: ESL-LKH
 */

#ifndef MYLIB_H_
#define MYLIB_H_

#pragma once
#include "cortex_m4.h"

#define 	HDP_S 	479
#define		VDP_S	271
#define LCD_HEIGHT	272
#define LCD_WIDTH	480
#define SCROLL	10
#define DIV_MS	17


#define LCD_ENTRY_MODE_REG        0x36
#define LCD_RAM_DATA_REG          0x2C
#define LCD_X_RAM_ADDR_REG        0x2A
#define LCD_Y_RAM_ADDR_REG        0x2B

#define DLY_4 	16
#define DLY_8	8
#define DLY_16	4
#define A0		63162
#define C1 		61068
#define D1 		54421
#define E1 		48484
#define F1 		45844
#define G1 		40815
#define A1 		36363
#define A_1 		34375
#define B1 		32388
#define C2 		30592
#define D2 		27211
#define D_2		25726
#define E2 		24242
#define F2 		22922
#define G2 		20408
#define A2 		18182
#define B2 		16194
#define C3 		15296
#define D3 		13606



#define COLOR_VIOLET	 0x480F
#define COLOR_ORANGE	 0xFBE0
#define COLOR_BLUE       0x001F
#define COLOR_GREEN      0x07E0
#define COLOR_RED      	 0xF800
#define COLOR_YELLOW	 0xFFE0
#define COLOR_WHITE      0xFFFF
#define COLOR_BLACK      0x0000
#define COLOR_NAVY		 0x000F

#define image0			0x000000	//¾Ë(½ÃÀÛ)
#define image1			0x040000	//±âº»2
#define image2			0x080000	//Àá1
#define image3			0x0C0000	//¿À¸¥ÂÊ
#define image4			0x100000	//¹ä
#define image5			0x140000	//²¥ÇÏ
#define image6			0x180000	//Àá2
#define image7			0x1C0000	//¿ÞÂÊ
#define image8			0x200000	//¾Ä±â1
#define image9			0x240000	//¾Ä±â2
#define image10			0x280000	//¾Ä±â3
#define image11			0x2C0000	//¾Ä±â4
#define image12			0x300000	//¾Ä±â5

#define image13			0x340000	//¾ÆÇÄ
#define image14			0x380000	//È¸º¹1
#define image15			0x3C0000	//È¸º¹2
#define image16			0x400000	//°¡À§¹ÙÀ§º¸
#define image17			0x440000	//¹¬¹¬
#define image18			0x480000	//¹¬Âî
#define image19			0x4C0000	//¹¬ºü
#define image20			0x500000	//Âî¹¬
#define image21			0x540000	//ÂîÂî
#define image22			0x580000	//Âîºü
#define image23			0x5C0000	//ºü¹¬
#define image24			0x600000	//ºüÂî
#define image25			0x640000	//ºüºü
#define image26			0x680000	//WIN
#define image27			0x6C0000	//DRAW
#define image28			0x700000	//LOSE
#define image29			0x740000	//±âº»1
#define image30			0x780000	//±âº»3
#define image31			0x7C0000	//Á×À½




static unsigned char fnd_num[18] = {
//	0		1		2		3		4		5		6		7
	0x3f,	0x06,	0x5b,	0x4f,	0x66,	0x6d,	0x7d,	0x27,
//	8		9		a		b		c		d		e		f
	0x7f,	0x67,	0x5f,	0x7c,	0x39,	0x5e,	0x79,	0x71,
//	off		neg
	0x00, 	0x40
};

static unsigned char fnd_pos[6] = {
//	D1		D2		D3		D4		D5		D6
	0x01,	0x02,	0x04,	0x08,	0x10,	0x20
};

void Interrupt_Init(void);
void PWM_Init(uint32_t ui32SysClock);
void RTC_Init(void);
void PWM0_Interrupt_Enable(void);
void PWM0_Interrupt_Disable(void);
void BUZZER_init();
void Bluetooth_Init(float BRD, int BRDI, int BRDF);
void TIMER_init();

void FND_clear();
void LED_clear();
void BUZZER_clear();
void LED_init();
//void DIP_init();
void PUSH_init();
void FND_init();
void LCD_Init(unsigned long ulClockMS);
void BUZZER_init();
void Play(int freq);
void DelayForPlay(int DLY);
inline uint16_t ReadData(void);
inline void WriteData(uint16_t ui16Data);
inline void WriteCommand(uint16_t ui8Data);

void Port_Init(void);
void Uart_Init(float BRD, int BRDI, int BRDF);
char Uart_GetCh(void);
char Uart_GetKey(void);
void Uart_PutCh(uint8_t data);
void Uart_PutStr(char* pt);
void Uart_Printf(char *fmt,...);

void SetFullFrame();
void Scroll_up(unsigned char *buffer, int x1, int y1, int x2, int y2, int scroll);
void Scroll_down(unsigned char *buffer, int x1, int y1, int x2, int y2, int scroll);
void Scroll_right(unsigned char *buffer, int x1, int y1, int x2, int y2, int scroll);
void Scroll_left(unsigned char *buffer, int x1, int y1, int x2, int y2, int scroll);
void DrawLine(unsigned char *buffer, int x1, int y1, int x2, int y2, int color);
void PutPixel(unsigned char *buffer, int x, int y, int color);

void RestoreBackground(unsigned char *buffer, int x1, int y1, int x2, int y2, int image);
void DrawImage(unsigned char *buffer, int x1, int y1, int x2, int y2, int image);
void DrawRect_fill(unsigned char *buffer, int x1, int y1, int x2, int y2, int color);
void DrawCircle(unsigned char *buffer, int nCenterX,int nCenterY,int nRadius,int color);
void DrawCircle_fill(unsigned char *buffer, int nCenterX,int nCenterY,int nRadius,int color);
void DrawCircleimage_fill(unsigned char *buffer, int nCenterX,int nCenterY,int nRadius,int image);

#endif /* MYLIB_H_ */
