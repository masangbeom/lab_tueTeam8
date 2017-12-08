#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "movement.h"

void Tire_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LEFT1_TIRE_CLK_GPIO | LEFT2_TIRE_CLK_GPIO |
	RIGHT1_TIRE_CLK_GPIO | RIGHT1_TIRE_CLK_GPIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LEFT1_TIRE_IN1 | LEFT1_TIRE_IN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LEFT1_TIRE_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LEFT2_TIRE_IN1 | LEFT2_TIRE_IN2;
	GPIO_Init(LEFT2_TIRE_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RIGHT1_TIRE_IN1 | RIGHT1_TIRE_IN2;
	GPIO_Init(RIGHT1_TIRE_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RIGHT2_TIRE_IN1 | RIGHT2_TIRE_IN2;
	GPIO_Init(RIGHT2_TIRE_GPIO, &GPIO_InitStructure);

	GPIO_ResetBits(LEFT1_TIRE_GPIO, LEFT1_TIRE_IN2);
	GPIO_ResetBits(LEFT2_TIRE_GPIO, LEFT2_TIRE_IN2);
	GPIO_ResetBits(RIGHT1_TIRE_GPIO, RIGHT1_TIRE_IN2);
	GPIO_ResetBits(RIGHT2_TIRE_GPIO, RIGHT2_TIRE_IN2);

	stopTheCar();
}

void setTowardAndTime(int towards, int time) {
	switch (towards) {
	case FORWARDS:
		setForwards();
		delay_ms(time);
		break;
	case BACKWARDS:
		setBackwards();
		delay_ms(time);
		break;
	case TURNLEFT:
		setTurnLeft();
		delay_ms(time);
		break;
	case TURNRIGHT:
		setTurnRight();
		delay_ms(time);
		break;
	default:
		stopTheCar();
	}
}

//void goToXY(int *valueXY){
//	setTowardAndTime(0);
//	setTowardAndTime();
//	stopTheCar();
//}

void setForwards(void) {
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) & 0xff55);
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) | 0x0055);
	// GPIO_SetBits(LEFT1_TIRE_GPIO, LEFT1_TIRE_IN1);
	// GPIO_SetBits(LEFT2_TIRE_GPIO, LEFT2_TIRE_IN1);
	// GPIO_SetBits(RIGHT1_TIRE_GPIO, RIGHT1_TIRE_IN1);
	// GPIO_SetBits(RIGHT2_TIRE_GPIO, RIGHT2_TIRE_IN1);
}

void setBackwards(void) {
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) & 0xffaa);
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) | 0x00aa);
}

void setTurnLeft(void) {
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) & 0xff5a);
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) | 0x005a);
}

void setTurnRight(void) {
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) & 0xffa5);
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) | 0x00a5);
}

void stopTheCar(void) {
	GPIO_Write(GPIOD, GPIO_ReadOutputData(GPIOD) & 0xff00);
}
