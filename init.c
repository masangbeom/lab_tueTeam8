#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stdlib.h"
#include "misc.h"
#include "init.h"

#define ADC1_DR_Address    ((u32)0x4001244C)

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

void init_Timer() {
	NVIC_InitTypeDef NVIC_InitStructure1;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure1;

	NVIC_InitTypeDef NVIC_InitStructure;             // for interreupt
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;       // timerbase...

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 |RCC_APB1Periph_TIM2, ENABLE);

	/* Enable TIM2 Global Interrupt */
	NVIC_InitStructure1.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);

	/* TIM2 Initialize *//* ≈∏¿Ã∏” ¡÷±‚ = (1/FCLK)*TIM_Prescaler*TIM_Period */
	TIM_TimeBaseStructure1.TIM_Period = 3600 - 1;
	TIM_TimeBaseStructure1.TIM_Prescaler = 2000 - 1; // (1/72MHz)*2000*3600 = 100ms
	TIM_TimeBaseStructure1.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure1.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure1);

	/* TIM2 Enable */
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // interrupt enable

	/* TIM4 Clock Enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* Enable TIM4 Global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM4 Initialize */
	TIM_TimeBaseStructure.TIM_Period = 2000;
	TIM_TimeBaseStructure.TIM_Prescaler = 35999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* TIM4 Enale */
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

}

//ADC 설정 : 조도센서 값 출력 Channel 8번 사용.
void ADC1_Configuration(void) {
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */
	ADC_ResetCalibration(ADC1);

	/* Check the end of ADC1 reset calibration register */
	while (ADC_GetResetCalibrationStatus(ADC1))
		;

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC1))
		;

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

void GPIO_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure1;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure1);
	//PB1 적외선거리감지센서
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DMA_Configuration(vu16 * ADCConvertedValue) {
	DMA_InitTypeDef DMA_InitStructure;
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) ADCConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
