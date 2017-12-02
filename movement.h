#ifndef __MOVEMENT_H
#define __MOVEMENT_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
/*
#include "delay.h"
*/
#define FORWARDS  0
#define BACKWARDS 1
#define TURNLEFT  2
#define TURNRIGHT 3

#define LEFT1_TIRE_GPIO   GPIOD
#define LEFT2_TIRE_GPIO   GPIOD
#define RIGHT1_TIRE_GPIO  GPIOD
#define RIGHT2_TIRE_GPIO  GPIOD

#define LEFT1_TIRE_CLK_GPIO   RCC_APB2Periph_GPIOD
#define LEFT2_TIRE_CLK_GPIO   RCC_APB2Periph_GPIOD
#define RIGHT1_TIRE_CLK_GPIO  RCC_APB2Periph_GPIOD
#define RIGHT2_TIRE_CLK_GPIO  RCC_APB2Periph_GPIOD

#define LEFT1_TIRE_ENA    8
#define LEFT1_TIRE_IN1    GPIO_Pin_0
#define LEFT1_TIRE_IN2    GPIO_Pin_1

#define LEFT2_TIRE_ENA    9
#define LEFT2_TIRE_IN1    GPIO_Pin_2
#define LEFT2_TIRE_IN2    GPIO_Pin_3

#define RIGHT1_TIRE_ENA   10
#define RIGHT1_TIRE_IN1   GPIO_Pin_4
#define RIGHT1_TIRE_IN2	  GPIO_Pin_5

#define RIGHT2_TIRE_ENA   11
#define RIGHT2_TIRE_IN1   GPIO_Pin_6
#define RIGHT2_TIRE_IN2	  GPIO_Pin_7

void Tire_Config(void);
void setForwards(void);
void setBackwards(void);
void setTurnLeft(void);
void setTurnRight(void);
void setSpeed(int towards, int PWM_Setl, int PWM_Setr);
void stopTheCar(void);
void carGo(uint8_t direction);
void carGoWithStick(uint8_t lx, uint8_t ly);

#endif /* __MOVEMENT_H */

