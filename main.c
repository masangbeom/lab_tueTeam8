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

char tb_st[7][512] = { {'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'} };

char showString[2048] = { '\0' };

int new_flag[7];

int bufBLE, tableNUM;
int sizeBLE[7];

int commandUI = 0;
int printTableNUM = 0;

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

//Occured When detect Rx of USART2
void USART2_IRQHandler(void) {
    if ((USART2 -> SR & USART_FLAG_RXNE) != (u16) RESET) {
        bufBLE = USART_ReceiveData(USART2);
        if (bufBLE == '@') {
            tableNUM = 1;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
        }
        else if (bufBLE == '#') {
            tableNUM = 2;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
        }
        else if (bufBLE == '$') {
            tableNUM = 3;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
        }
        else if (bufBLE == '%') {
            tableNUM = 4;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
        }
        else if (bufBLE == '^') {
            tableNUM = 5;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
        }
        else if (bufBLE == '&') {
            tableNUM = 6;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
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
int main() {
	GPIO_InitTypeDef AAA;
	SystemInit();
	UsartInit();
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
			printOrderList(&tb_st[printTableNUM], &commandUI, &pos_x, &pos_y);
			break;
		}

		}
	}
}
