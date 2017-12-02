//#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_usart.h"
//#include "stm32f10x_tim.h"
//#include "stdlib.h"
//#include "misc.h"
////UsartInit
//void UsartInit(void) {
//	USART_InitTypeDef USART_InitStructure2;
//
//	NVIC_InitTypeDef NVIC_InitStruct_UART2;
//	// RCC Configuration
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//
//	/* UART2 Port Configuration */
//	USART_InitStructure2.USART_BaudRate = 9600;
//	USART_InitStructure2.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure2.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure2.USART_Parity = USART_Parity_No;
//	USART_InitStructure2.USART_HardwareFlowControl =
//	USART_HardwareFlowControl_None;
//	USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//
//	USART_Init(USART2, &USART_InitStructure2);
//
//	/* Enable NVIC for UART Interrupt */
//	NVIC_InitStruct_UART2.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStruct_UART2.NVIC_IRQChannelPreemptionPriority = 0x0;
//	NVIC_InitStruct_UART2.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStruct_UART2.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStruct_UART2);
//
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//
//	USART_Cmd(USART2, ENABLE);
//}
//
//
//void init_Timer() {
//   NVIC_InitTypeDef NVIC_InitStructure;             // for interreupt
//   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;       // timerbase...
//
//   /* TIM2 Clock Enable */
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//
//   /* Enable TIM2 Global Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
//
//   /* TIM2 Initialize */
//   TIM_TimeBaseStructure.TIM_Period = 2000;
//   TIM_TimeBaseStructure.TIM_Prescaler = 35999;
//   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//
//   /* TIM2 Enale */
//   TIM_ARRPreloadConfig(TIM2, ENABLE);
//   TIM_Cmd(TIM2, ENABLE);
//
//}
