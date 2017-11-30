#include "lcd.h"
#include "misc.h"
#include "touch.h"
#include "stm32f10x_tim.h"

void printOrderList(char * tb_st, int * commandUI, uint16_t *pos_x,
		uint16_t *pos_y) {
	uint16_t pix_x, pix_y;
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); // interrupt enable
	LCD_Clear(WHITE);
	while (*commandUI == 1) {
		LCD_ShowString(10, 30, tb_st, BLACK, WHITE);
		LCD_DrawLine(100, 240, 100, 320);
		LCD_ShowString(20, 275, "Back", BLACK, WHITE);
		LCD_ShowString(105, 275, "Start Delivery", BLACK, WHITE);
		LCD_DrawLine(0, 240, 240, 240);
		Touch_GetXY(pos_x, pos_y, 1);
		Convert_Pos(*pos_x, *pos_y, &pix_x, &pix_y);
		if (pix_x >= 0 && pix_x <= 100 && pix_y >= 240 && pix_y <= 320) {
			*commandUI = 0;
		} else if (pix_x > 100 && pix_x <= 240 && pix_y >= 240
				&& pix_y <= 320) {
			*commandUI = 2; // Start Delivery
		}
	}
	LCD_Clear(WHITE);
}

int printWaitUI(int * commandUI, uint16_t *pos_x, uint16_t *pos_y,
		int * new_flag) {
	uint16_t pix_x, pix_y;
	int tableNumber = 0;
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // interrupt enable
	while (*commandUI == 0) {
		LCD_DrawLine(0, 103, 240, 103);
		LCD_DrawLine(0, 206, 240, 206);
		LCD_DrawLine(120, 0, 120, 320);

		LCD_ShowString(30, 53, "TABLE 1", BLACK, WHITE);

		LCD_ShowString(150, 53, "TABLE 2", BLACK, WHITE);

		LCD_ShowString(30, 153, "TABLE 3", BLACK, WHITE);

		LCD_ShowString(150, 153, "TABLE 4", BLACK, WHITE);

		LCD_ShowString(30, 253, "TABLE 5", BLACK, WHITE);

		LCD_ShowString(150, 253, "TABLE 6", BLACK, WHITE);

		Touch_GetXY(pos_x, pos_y, 1);
		Convert_Pos(*pos_x, *pos_y, &pix_x, &pix_y);
		if (pix_x >= 0 && pix_x <= 120 && pix_y >= 0 && pix_y <= 103) {
			*commandUI = 1;
			tableNumber = 1;
			new_flag[tableNumber] = 0;
		} else if (pix_x >= 120 && pix_x <= 240 && pix_y >= 0 && pix_y <= 103) {
			*commandUI = 1;
			tableNumber = 2;
			new_flag[tableNumber] = 0;
		} else if (pix_x >= 0 && pix_x <= 120 && pix_y >= 103 && pix_y <= 206) {
			*commandUI = 1;
			tableNumber = 3;
			new_flag[tableNumber] = 0;
		} else if (pix_x >= 120 && pix_x <= 240 && pix_y >= 103
				&& pix_y <= 206) {
			*commandUI = 1;
			tableNumber = 4;
			new_flag[tableNumber] = 0;
		} else if (pix_x >= 0 && pix_x <= 120 && pix_y >= 206 && pix_y <= 320) {
			*commandUI = 1;
			tableNumber = 5;
			new_flag[tableNumber] = 0;
		} else if (pix_x >= 120 && pix_x <= 240 && pix_y >= 206
				&& pix_y <= 320) {
			*commandUI = 1;
			tableNumber = 6;
			new_flag[tableNumber] = 0;
		}
	}
	LCD_Clear(WHITE);
	return tableNumber;
}

void startDelivery(int * commandUI, uint16_t *pos_x, uint16_t *pos_y) {
	uint16_t pix_x, pix_y;
	while (*commandUI == 2) {
		LCD_DrawLine(0, 240, 240, 240);
		LCD_ShowString(30, 153, "FoodVery is delivering !", BLACK, WHITE);
		LCD_ShowString(30, 275, "Press Button - GO BACK", BLACK, WHITE);
		if (pix_x >= 0 && pix_x <= 240 && pix_y >= 240 && pix_y <= 320) {
			*commandUI = 3;
		}
	}
	LCD_Clear(WHITE);
}

void printConfirm(int *commandUI, int * countConfirm) {
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	LCD_ShowString(30, 153, "Have a nice meal. \n Thank you for your order.", BLACK, WHITE);
	while (*commandUI == 3) {
		if (*countConfirm == 3) {
			*commandUI = 1;
		}
	}
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	LCD_Clear(WHITE);
}
