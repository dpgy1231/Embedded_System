#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

//volatile unsigned int variable0 = 0;


unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y);
extern int32_t user_X, user_Y;
uint32_t g_ui32SysClock;

void WRITE_FND(int digit, int seg_out);
void WRITE_FND_DOT(int digit, int seg_out);
void condition(int hrt_cnt);
void gauge(int HG, int EG, int CG);
void RSP_game(char alph);
void delay(int count);

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];

int tone[15] = { E2,D_2,C2,D2,D2,A_1,B1,B1,E2,C2, C2,C1,C1,A0,A0};
				//E2,D_2,C2,D2,D2, A_1,B1,B1,E2,C2,C2,E2,E2,D2,D2
int tone2[16] = {C1,D1,E1,F1,G1,A1,B1,C2,D2,E2,F2,G2,A2,B2,C3,D3};
int flag=0;
int i=0;
int touchcnt=0;
int bk_flag=0;
int hrt_cnt=50;
int eat_cnt=50;
int cln_cnt=50;
int out = 0;
int rsp_uart = 0;
int rsp=0;
int push_data=0;
int nondie=0;
int win_cnt=0;
int lose_cnt=0;

int main(void) {
	uint8_t code;
	float BRD;
	int   BRDI;
	int   BRDF;

	int rtc_time0;

	g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
										SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
										SYSCTL_CFG_VCO_480), 120000000);

	BRD = (float)g_ui32SysClock / (16 * 115200);
	BRDI = (int)BRD;
	BRDF = (int)(((BRD - (int)BRD)*64) + 0.5);

	LED_init();
	PUSH_init();
	FND_init();
	RTC_Init();
	TIMER_init();
	BUZZER_init();

	//Uart
	Uart_Init(BRD, BRDI, BRDF);
	//LCD
	LCD_Init(g_ui32SysClock);
	//Touch
	TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(PointerMessage);

    LED_clear();
	FND_clear();
	BUZZER_clear();
	delay(10000000);

		//********** Set Real Clock ************//
		HIBLOCK = 0xA3359554;

		while (!(HIBCTL & 0x80000000));
		HIBCALLD0 = (0 << 16) | (0 << 8) | 0;
		// (Hour << 16) | (Min << 8) | Sec
		while (!(HIBCTL & 0x80000000));

		HIBLOCK = 0;
		while (!(HIBCTL & 0x80000000));

		rtc_time0 = HIBCAL0;	// hour, min, sec
		//**************************************//

	GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer ON
	GPTMCTL = GPTMCTL | 0x41;

	DrawImage(buffer, 0, 0, 480, 272, image0);

	while(1){

		push_data = ~(( GPIO_READ(GPIO_PORTP, 0x02 ) >> 1 ) | ( GPIO_READ(GPIO_PORTN, 0x08) >> 2 ) |
					   ( GPIO_READ(GPIO_PORTE, 0x20 ) >> 3 ) | ( GPIO_READ(GPIO_PORTK, 0x80) >> 4));

		rtc_time0 = HIBCAL0;
		HIBCTL = HIBCTL | 0x01;
		if(nondie==0x1){ HIBCTL = HIBCTL & ~0x01; } //TIME STOP
		else{ HIBCTL = HIBCTL | 0x01; }	//TIME START
		if(push_data & 0x2)	flag=1;	//시계
	  	if(flag==1){
	  		if(nondie){
	  		WRITE_FND(1,((rtc_time0 >> 16) & 0x1f) / 10);
	  		WRITE_FND_DOT(2,((rtc_time0 >> 16) & 0x1f) % 10);
	  		WRITE_FND(3,((rtc_time0 >> 8) & 0x3f) / 10);
	  		WRITE_FND_DOT(4,((rtc_time0 >> 8) & 0x3f) % 10);
	  		WRITE_FND(5,((rtc_time0) & 0x3f) / 10);
	  		WRITE_FND_DOT(6,((rtc_time0) & 0x3f) % 10);
	  		}
	  	}
	  	if(push_data & 0x4)	flag=2;	//카운트3개
	  	if(flag==2){
	  		WRITE_FND(1,hrt_cnt/10);
	  		WRITE_FND_DOT(2,hrt_cnt%10);
	  		WRITE_FND(3,eat_cnt/10);
	  		WRITE_FND_DOT(4,eat_cnt%10);
	  		WRITE_FND(5,cln_cnt/10);
	  		WRITE_FND_DOT(6,cln_cnt%10);
	  	}

	  	if(bk_flag!=2){	//임의로 하트 다운카운팅
	  		if(nondie){
				code = Uart_GetKey();
				if(code=='k'){
					gauge(-5, -2, -2);
					condition(hrt_cnt);
				}
	  		}
	  	}
		if(push_data & 0x8){	//reset
			DrawImage(buffer, 0, 0, 480, 272, image0);
			i=0;	flag=0;	touchcnt=0;
			bk_flag=0;		hrt_cnt=50;
			eat_cnt=50;		cln_cnt=50;
			rsp_uart = 0;	rsp=0;
			push_data=0;	nondie=1;
			win_cnt=0;		lose_cnt=0;

			//********** Set Real Clock ************//
			HIBLOCK = 0xA3359554;

			while (!(HIBCTL & 0x80000000));
			HIBCALLD0 = (0 << 16) | (0 << 8) | 0;
			// (Hour << 16) | (Min << 8) | Sec
			while (!(HIBCTL & 0x80000000));

			HIBLOCK = 0;
			while (!(HIBCTL & 0x80000000));

			rtc_time0 = HIBCAL0;	// hour, min, sec
			//**************************************//
		}
		if(hrt_cnt==0){	//죽음
			nondie=0;
			condition(hrt_cnt);

			for(i=0; i<15; i++){
				Play(tone[i]);
				delay(2000000);
			}
			BUZZER_clear();
			for(i=1; i<=0xff; i=i<<1){
				GPIO_WRITE(GPIO_PORTM, 0xf, i>>4);
				GPIO_WRITE(GPIO_PORTL, 0xf, i);
				delay(100000);
			}

		}
		if(bk_flag == 2) {	//게임 ->유아트위해 while문에 작성
			code = Uart_GetKey();
			//srand();
			rsp = (rand() % 3) + 1;
			RSP_game(code);
		}
	}
	return 0;
}

int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y){
	if(user_X >= 0 && user_X < 480 && user_Y >= 0 && user_Y < 272){
		if(bk_flag==0){
			if(user_X >= 160 && user_X < 260 && user_Y >= 42 && user_Y < 172){	//broken egg
				touchcnt++;
				if(touchcnt==1)	DrawLine(buffer, 150, 95, 174, 90, COLOR_BLACK);
				if(touchcnt==2)	DrawLine(buffer, 174, 90, 198, 105, COLOR_BLACK);
				if(touchcnt==3)	DrawLine(buffer, 198, 105, 222, 92, COLOR_BLACK);
				if(touchcnt==4)	DrawLine(buffer, 222, 92, 246, 115, COLOR_BLACK);
				if(touchcnt==5){
					DrawLine(buffer, 246, 115, 270, 88, COLOR_BLACK);
					bk_flag=1;	//기본->부화
					nondie=1;
					condition(hrt_cnt);
					for(i=0; i<16; i++){	//효과음
						Play(tone2[i]);
						delay(600000);
					}
					BUZZER_clear();
				}
			}
			delay(5000000);
		}
		if(nondie){
			if(bk_flag==1){	//기본
				if(user_X >= 230 && user_X < 350 && user_Y >= 115 && user_Y < 200){	//캐릭터 터치
					gauge(2, 0, 0);
					DrawImage(buffer, 0, 0, 480, 272, image5);
					delay(5000000);
					condition(hrt_cnt);
				}
				if(user_X >= 0 && user_X < 90 && user_Y >= 0 && user_Y < 50){	//잠메뉴 터치
					gauge(50, -15, -10);
					DrawImage(buffer, 0, 0, 480, 272, image7);
					delay(1000000);
					DrawImage(buffer, 0, 0, 480, 272, image2);
					delay(1000000);
					DrawImage(buffer, 0, 0, 480, 272, image6);
					delay(9900000);
					condition(hrt_cnt);
				}
				if(user_X >= 95 && user_X < 190 && user_Y >= 0 && user_Y < 50){	//밥메뉴터치
					gauge(5, 10, -5);
					DrawImage(buffer, 0, 0, 480, 272, image3);
					DrawImage(buffer, 0, 0, 480, 272, image4);
					delay(9900000);
					DrawImage(buffer, 0, 0, 480, 272, image5);
					delay(5000000);
					condition(hrt_cnt);
				}
				if(user_X >= 190 && user_X < 280 && user_Y >= 0 && user_Y < 50){	//씻기메뉴터치
					gauge(5, -3, 15);
					DrawImage(buffer, 0, 0, 480, 272, image3);
					delay(1000000);
					DrawImage(buffer, 0, 0, 480, 272, image8);
					delay(1000000);
					DrawImage(buffer, 0, 0, 480, 272, image9);
					delay(1000000);
					DrawImage(buffer, 0, 0, 480, 272, image10);
					delay(1000000);
					DrawImage(buffer, 0, 0, 480, 272, image11);
					delay(1000000);
					DrawImage(buffer, 0, 0, 480, 272, image12);
					delay(1000000);
					DrawImage(buffer, 0, 0, 480, 272, image7);
					delay(1000000);
					condition(hrt_cnt);
				}
				if(user_X >= 290 && user_X < 380 && user_Y >= 0 && user_Y < 50){	//회복메뉴터치
					gauge(20, 0, 25);
					DrawImage(buffer, 0, 0, 480, 272, image14);
					delay(3000000);
					DrawImage(buffer, 0, 0, 480, 272, image15);
					delay(9000000);
					condition(hrt_cnt);
				}
				if(user_X >= 385 && user_X < 480 && user_Y >= 0 && user_Y < 50){	//게임메뉴터치
					gauge(7, -10, -15);
					DrawImage(buffer, 0, 0, 480, 272, image16);
					win_cnt=0; lose_cnt=0;
					bk_flag = 2;
				}
				delay(5000000);
			}//bk_flag=2 -> 게임화면 코드while문에 작성
		}
	}
	return 0;
}

void RSP_game(char alph){
	switch(alph){
	case 'a':
		rsp_uart=1;	//묵
		break;
	case 's':
		rsp_uart=2;	//찌
		break;
	case 'd':
		rsp_uart=3;	//빠
		break;
	default:
		rsp_uart=0;
		break;
	}
	if(rsp_uart==1){	//묵
		if (rsp == 1) { //묵묵
			DrawImage(buffer, 0, 0, 480, 272, image17);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image27);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
		else if (rsp == 2) { //찌묵
			win_cnt++;
			DrawImage(buffer, 0, 0, 480, 272, image20);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image26);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
		else if (rsp == 3) { //빠묵
			lose_cnt++;
			DrawImage(buffer, 0, 0, 480, 272, image23);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image28);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
	}
	if(rsp_uart==2){	//찌
		if (rsp == 1) { //묵찌
			lose_cnt++;
			DrawImage(buffer, 0, 0, 480, 272, image18);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image28);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
		else if (rsp == 2) { //찌찌
			DrawImage(buffer, 0, 0, 480, 272, image21);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image27);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
		else if (rsp == 3) { //빠찌
			win_cnt++;
			DrawImage(buffer, 0, 0, 480, 272, image24);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image26);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
	}
	if(rsp_uart==3){	//빠
		if (rsp == 1) { //묵빠
			win_cnt++;
			DrawImage(buffer, 0, 0, 480, 272, image19);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image26);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
		else if (rsp == 2) { //찌빠
			lose_cnt++;
			DrawImage(buffer, 0, 0, 480, 272, image22);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image28);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
		else if (rsp == 3) { //빠빠
			DrawImage(buffer, 0, 0, 480, 272, image25);
			delay(2000000);
			DrawImage(buffer, 150, 82, 330, 190, image27);
			delay(9000000);
			DrawImage(buffer, 0, 0, 480, 272, image16);
		}
	}
	for(i=0; i<win_cnt; i++){	//win일때 파란포인트블럭쌓임
		DrawRect_fill(buffer, 460, (5*i)+2, 475, 5+(5*i), COLOR_BLUE);

	}
	for(i=0; i<lose_cnt; i++){	//lose일때 빨간포인트블럭쌓임
		DrawRect_fill(buffer, 5, (5*i)+2, 20, 5+(5*i), COLOR_RED);
	}

	if ( push_data & 0x1) {
		Uart_Printf("score [%d : %d]\n\r", lose_cnt, win_cnt);	//나갈때 스코어 터미널에 출력
		bk_flag = 1;//PUSH1 되돌아가기
		rsp_uart = 0;
		rsp=0;
		condition(hrt_cnt);
	}

}
void condition(int h_cnt){
	if(h_cnt>=40)
		DrawImage(buffer, 0, 0, 480, 272, image29); //기본1
	else if(h_cnt>=20)
		DrawImage(buffer, 0, 0, 480, 272, image1); //기본2
	else if(h_cnt>=10)
		DrawImage(buffer, 0, 0, 480, 272, image30);	//기본4
	else if(h_cnt>0)
		DrawImage(buffer, 0, 0, 480, 272, image13);	//아픔
	else if(h_cnt==0)
		DrawImage(buffer, 0, 0, 480, 272, image31);	//죽음
}
void gauge(int HG, int EG, int CG){
	hrt_cnt+=HG;
	eat_cnt+=EG;
	cln_cnt+=CG;

	if(hrt_cnt>50)	hrt_cnt=50;
	if(eat_cnt>50)	eat_cnt=50;
	if(cln_cnt>50)	cln_cnt=50;

	if(hrt_cnt<0)	hrt_cnt=0;
	if(eat_cnt<=0){
		eat_cnt=0;
		hrt_cnt--;
		if(hrt_cnt<0)	hrt_cnt=0;
	}
	if(cln_cnt<=0){
		cln_cnt=0;
		hrt_cnt--;
		if(hrt_cnt<0)	hrt_cnt=0;
	}
}
void WRITE_FND(int digit, int seg_out){
	int i;

	GPIO_WRITE(GPIO_PORTC, 0x10 | 0x20 | 0x40 | 0x80, (fnd_num[seg_out] << 4) & 0xf0);
	GPIO_WRITE(GPIO_PORTG, 0x01 | 0x02 | 0x04 | 0x08, (fnd_num[seg_out] >> 4) & 0x0f);

	GPIO_WRITE(GPIO_PORTH, 0x01 | 0x02 | 0x04 | 0x08, fnd_pos[digit-1] & 0x0f);
	GPIO_WRITE(GPIO_PORTN, 0x10 | 0x20, fnd_pos[digit-1] & 0xf0);

	for(i=0; i<9999;i++);
}
void WRITE_FND_DOT(int digit, int seg_out){
	int i;

	GPIO_WRITE(GPIO_PORTC, 0x10 | 0x20 | 0x40 | 0x80,  (fnd_num[seg_out] << 4) & 0xf0);
	GPIO_WRITE(GPIO_PORTG, 0x01 | 0x02 | 0x04 | 0x08, ((fnd_num[seg_out] >> 4) & 0x0f) | 0x08);

	GPIO_WRITE(GPIO_PORTH, 0x01 | 0x02 | 0x04 | 0x08, fnd_pos[digit-1] & 0x0f);
	GPIO_WRITE(GPIO_PORTN, 0x10 | 0x20, fnd_pos[digit-1] & 0xf0);

	for(i=0; i<9999;i++);
}
void delay(int count){
	while(count != 0){
		count--;
	}
}

//	int dip_data;
//if(start){}
//GPIO_WRITE(GPIO_PORTD, 0x10, ~0x10);  // buzzer OFF
//int start=0;
//	DIP_init();
//		dip_data = 	( GPIO_READ(GPIO_PORTA, 0x08) >> 3 ) | ( GPIO_READ(GPIO_PORTA, 0x40) >> 5 ) |
//			( GPIO_READ(GPIO_PORTA, 0x80) >> 5 ) | ( GPIO_READ(GPIO_PORTB, 0x08) >> 0 ) |
//			( GPIO_READ(GPIO_PORTQ, 0x40) >> 2 ) | ( GPIO_READ(GPIO_PORTQ, 0x20) >> 0 ) |
//			( GPIO_READ(GPIO_PORTQ, 0x10) << 2 ) | ( GPIO_READ(GPIO_PORTG, 0x40) << 1 );
//		if(dip_data==0x01)	start=1;
/*
			Play(C1);
			DelayForPlay(DLY_8);
			Play(D1);
			DelayForPlay(DLY_8);
			Play(E1);
			DelayForPlay(DLY_8);
			Play(C1);
			DelayForPlay(DLY_4);
			Play(D1);
			DelayForPlay(DLY_8);
			Play(E1);
			DelayForPlay(DLY_8);
			Play(C1);
			DelayForPlay(DLY_8);
			Play(E1);
			DelayForPlay(DLY_8);
			Play(F1);
			DelayForPlay(DLY_8);
			Play(G1);
			DelayForPlay(DLY_4);
			Play(E1);
			DelayForPlay(DLY_8);
			Play(F1);
			DelayForPlay(DLY_8);
			Play(G1);
			DelayForPlay(DLY_4);
			Play(G1);
			DelayForPlay(DLY_16);
			Play(A1);
			DelayForPlay(DLY_16);
			Play(G1);
			DelayForPlay(DLY_16);
			Play(F1);
			DelayForPlay(DLY_8);
			Play(E1);
			DelayForPlay(DLY_16);
			Play(C1);
			DelayForPlay(DLY_8);
			Play(G1);
			DelayForPlay(DLY_16);
			Play(A1);
			DelayForPlay(DLY_16);
			Play(G1);
			DelayForPlay(DLY_16);
			Play(F1);
			DelayForPlay(DLY_8);
			Play(E1);
			DelayForPlay(DLY_16);
			Play(C1);
			DelayForPlay(DLY_8);
			Play(C1);
			DelayForPlay(DLY_8);
			Play(C1);
			DelayForPlay(DLY_4);
			Play(C1);
			DelayForPlay(DLY_8);
			Play(C1);
			DelayForPlay(DLY_4);
			GPIO_WRITE(GPIO_PORTD, 0x10, ~0x10);
			BUZZER_clear();
*/
