#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "movement.h"
#include "lcd.h"
#include "misc.h"
#include "printFunc.h"
#include "touch.h"
#include "init.h"
#include "pca9685.h"

#define WAIT_ORDER 0
#define NEW_ORDER 1
#define DELI_START 2
#define CONFIRM 3

char tb_st[7][512] = { { '\0' }, { '\0' }, { '\0' }, { '\0' }, { '\0' },
		{ '\0' }, { '\0' } };

int new_flag[7];

int bufBLE, tableNUM;
int sizeBLE[7];

int commandUI = 0;
int countConfirm = 0;
int printTableNUM = 0;

uint16_t pos_x, pos_y;

//Occured When detect Rx of USART2
void USART2_IRQHandler(void) {
	if ((USART2->SR & USART_FLAG_RXNE) != (u16) RESET) {
		bufBLE = USART_ReceiveData(USART2);
		if (bufBLE == '@') {
			tableNUM = 1;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
		} else if (bufBLE == '#') {
			tableNUM = 2;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
		} else if (bufBLE == '$') {
			tableNUM = 3;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
		} else if (bufBLE == '%') {
			tableNUM = 4;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
		} else if (bufBLE == '^') {
			tableNUM = 5;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
		} else if (bufBLE == '&') {
			tableNUM = 6;
			new_flag[tableNUM] = 1;
			tb_st[tableNUM][0] = '\0';
			sizeBLE[tableNUM] = 0;
			bufBLE = ' ';
		}

		if (sizeBLE[tableNUM] == 512) {
			tb_st[tableNUM][sizeBLE[tableNUM]] = bufBLE;
			sizeBLE[tableNUM] = 0;
		} else {
			tb_st[tableNUM][sizeBLE[tableNUM]++] = bufBLE;
		}
		tb_st[tableNUM][sizeBLE[tableNUM]] = '\0';
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
	}

	if (commandUI == 3) {
		countConfirm++;
		if (countConfirm == 4) {
			commandUI = 0;
		}
	}

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	//Clears the TIMx's interrupt pending bits.
}

void soundConfig(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11)) {
		GPIO_SetBits(GPIOD, GPIO_Pin_2);
	}
}

int main() {
	int i = 0 ;
	SystemInit();
	Tire_Config();
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
			startDelivery(&commandUI, &pos_x, &pos_y);
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
