#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "lcd.h"
#include "misc.h"
#include "printFunc.h"
#include "touch.h"
#include "stdlib.h"

#define WAIT_ORDER 0
#define NEW_ORDER 1


char st[2048] = { '\0' };

char showString[2048] = { '\0' };

int bufBLE, sizeBLE, tableNUM;

int commandUI = 0;

uint16_t pos_x, pos_y;

//UsartInit
void UsartInit(void) {
	USART_InitTypeDef USART_InitStructure2;

	NVIC_InitTypeDef NVIC_InitStruct_UART2;
	// RCC Configuration
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* UART2 Port Configuration */
	USART_InitStructure2.USART_BaudRate = 9600;
	USART_InitStructure2.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure2.USART_StopBits = USART_StopBits_1;
	USART_InitStructure2.USART_Parity = USART_Parity_No;
	USART_InitStructure2.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure2);

	/* Enable NVIC for UART Interrupt */
	NVIC_InitStruct_UART2.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct_UART2.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStruct_UART2.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct_UART2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct_UART2);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);
}

////Occured When detect Rx of USART1
//void USART1_IRQHandler(void) {
//	char c;
//
//	if (GetPCKey(&c) == 1) { //Get Data Using RX's Signal of USART1
//		Lb_BT_printf("%c", c); //Send Data Using TX's Signal of USART2
//	}
//}

//Occured When detect Rx of USART2
void USART2_IRQHandler(void) {
	if ((USART2->SR & USART_FLAG_RXNE) != (u16) RESET) {
		bufBLE = USART_ReceiveData(USART2);
		if (sizeBLE == 512) {
			st[sizeBLE] = bufBLE;
			sizeBLE = 0;
		} else {
			st[sizeBLE++] = bufBLE;
		}
		st[sizeBLE] = '\0';
	}
	if (st[1] == 'B' && st[2] == 'T') {
		st[0] = '\0';
	} else if (st[1] == 'C' && st[2] == 'O') {
		st[0] = '\0';
	}
}

int main() {
	SystemInit();
	UsartInit();
	LCD_Init();
	Touch_Configuration();
	Touch_Adjust();
	LCD_Clear(WHITE);

	while (1) {
		switch (commandUI) {
		case WAIT_ORDER: {
			printWaitUI(&commandUI, &pos_x, &pos_y);
			break;
		}
		case NEW_ORDER: {
			printOrderList(st, &tableNUM , &commandUI, &pos_x, &pos_y);
			break;
		}

		}

	}
}
