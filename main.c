#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "delay.h"
#include "movement.h"
#include "lcd.h"
#include "misc.h"
#include "printFunc.h"
#include "touch.h"
#include "init.h"

//UI 화면
#define WAIT_ORDER 0
#define NEW_ORDER 1
#define DELI_START 2
#define CONFIRM 3

//Order List String
char tb_st[7][512] = { { '\0' }, { '\0' }, { '\0' }, { '\0' }, { '\0' },
		{ '\0' }, { '\0' } };

//Temp Array for X, Y Location Value
char XY_Value[7][50] = { { '\0' }, { '\0' }, { '\0' }, { '\0' }, { '\0' }, {
		'\0' }, { '\0' } };

//X, Y Location Value
int valueXY[2];

//블루투스 RX를 통해 받는 값이 OrderList인지 X,Y Location인지
int isString = 0;

//주문이 있는지 없는지 1초마다 갱신.
int new_flag[7];

int bufBLE, tableNUM, XorY;
int sizeBLE[7];
int sizeBLEXY[7];

int commandUI = 0;
int countConfirm = 0;
int printTableNUM = 0;

uint16_t pos_x, pos_y;
vu16 ADCConvertedValue;
vu16 ADCTEMP;

//Occured When detect Rx of USART2
void USART2_IRQHandler(void) {
	if ((USART2->SR & USART_FLAG_RXNE) != (u16) RESET) {
		bufBLE = USART_ReceiveData(USART2);
		switch (bufBLE) {
		case '@': {
			tableNUM = 1;
			isString = 1;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
			break;
		}
		case '#': {
			tableNUM = 2;
			isString = 1;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
			break;
		}
		case '$': {
			tableNUM = 3;
			isString = 1;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
			break;
		}
		case '%': {
			tableNUM = 4;
			isString = 1;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
			break;
		}
		case '^': {
			tableNUM = 5;
			isString = 1;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
			break;
		}
		case '&': {
			tableNUM = 6;
			isString = 1;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
			break;
		}
		case '*': {
			bufBLE = ' ';
			isString = 0;
			sizeBLEXY[tableNUM] = 0;
			XY_Value[tableNUM][0] = '\0';
			break;
		}
		}

		// String Order List 이면
		if (isString == 1) {
			if (sizeBLE[tableNUM] == 512) {
				tb_st[tableNUM][sizeBLE[tableNUM]] = bufBLE;
				sizeBLE[tableNUM] = 0;

			} else {
				tb_st[tableNUM][sizeBLE[tableNUM]++] = bufBLE;
			}
			tb_st[tableNUM][sizeBLE[tableNUM]] = '\0';
		} // XY 좌표값이면
		else if (isString == 0) {
			if (sizeBLEXY[tableNUM] == 512) {
				XY_Value[tableNUM][sizeBLEXY[tableNUM]] = bufBLE;
				sizeBLEXY[tableNUM] = 0;
			} else {
				XY_Value[tableNUM][sizeBLEXY[tableNUM]++] = bufBLE;
			}
			XY_Value[tableNUM][sizeBLEXY[tableNUM]] = '\0';
		}
	}

}

//TIM2
void TIM2_IRQHandler(void) {
	if (commandUI == 0) {
		if (new_flag[1])
			LCD_ShowString(38, 75, "[NEW]", WHITE, GREEN);
		if (new_flag[2])
			LCD_ShowString(158, 75, "[NEW]", WHITE, GREEN);
		if (new_flag[3])
			LCD_ShowString(38, 175, "[NEW]", WHITE, GREEN);
		if (new_flag[4])
			LCD_ShowString(158, 175, "[NEW]", WHITE, GREEN);
		if (new_flag[5])
			LCD_ShowString(38, 290, "[NEW]", WHITE, GREEN);
		if (new_flag[6])
			LCD_ShowString(158, 290, "[NEW]", WHITE, GREEN);

		// commandUI == 2 로 가야함.
		ADCTEMP = ADCConvertedValue;
		//적외선 거리 감지센서에 인식이 되면
		if (ADCTEMP <= 10) {
			stopTheCar();
		}
	} else if (commandUI == 2) {

	} else if (commandUI == 3) {
		countConfirm++;
		if (countConfirm == 4) {
			commandUI = 0;
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	//Clears the TIMx's interrupt pending bits.
}

int main() {
	SystemInit();
	Tire_Config();
	GPIO_Configuration();
	ADC1_Configuration();
	DMA_Configuration(&ADCConvertedValue);
	delay_init(72);
	UsartInit();
	init_Timer();
	LCD_Init();
	Touch_Configuration();
	Touch_Adjust();
	LCD_Clear(WHITE);

	while (1) {
		switch (commandUI) {
		case WAIT_ORDER: {
			printTableNUM = printWaitUI(&commandUI, &pos_x, &pos_y, new_flag);
			break;
		}
		case NEW_ORDER: {
			printOrderList(tb_st[printTableNUM], &commandUI, &pos_x, &pos_y);
			break;
		}
		case DELI_START: {
			startDelivery(&commandUI, &pos_x, &pos_y, XY_Value[printTableNUM],
					sizeBLEXY[tableNUM], valueXY);
			break;
		}
		case CONFIRM: {
			printConfirm(tb_st[printTableNUM], &sizeBLE[printTableNUM],
					&commandUI, &countConfirm);
			break;
		}

		}
	}
}
