#include "lcd.h"
#include "misc.h"
#include "touch.h"

void printOrderList(char * tb_st, int * commandUI, uint16_t *pos_x, uint16_t *pos_y) {
	uint16_t pix_x, pix_y;
	LCD_Clear(WHITE);
	while (*commandUI == 1) {
		LCD_ShowString(10, 30, tb_st, BLACK, WHITE);
		LCD_DrawLine(0, 240, 240, 240);
		Touch_GetXY(pos_x, pos_y, 1);
		Convert_Pos(*pos_x, *pos_y, &pix_x, &pix_y);
		if (pix_x >= 0 && pix_x <= 240 && pix_y >= 240 && pix_y <= 320) {
			*commandUI = 0;
		}
	}
	LCD_Clear(WHITE);
}

int printWaitUI(int * commandUI, uint16_t *pos_x, uint16_t *pos_y, int * new_flag) {
	uint16_t pix_x, pix_y;
	int tableNumber = 0;
	while (*commandUI == 0) {
		LCD_DrawLine(0, 103, 240, 103);
		LCD_DrawLine(0, 206, 240, 206);
		LCD_DrawLine(120, 0, 120, 320);
		LCD_ShowString(30, 53, "TABLE 1" , BLACK, WHITE);
		if(new_flag[1]) LCD_ShowString(38, 53, "[NEW]" , BLACK, WHITE);

		LCD_ShowString(150, 53, "TABLE 2", BLACK, WHITE);
		if(new_flag[2]) LCD_ShowString(158, 53, "[NEW]" , BLACK, WHITE);

		LCD_ShowString(30, 153, "TABLE 3", BLACK, WHITE);
		if(new_flag[3]) LCD_ShowString(38, 153, "[NEW]" , BLACK, WHITE);

		LCD_ShowString(150, 153, "TABLE 4", BLACK, WHITE);
		if(new_flag[4]) LCD_ShowString(158, 153, "[NEW]" , BLACK, WHITE);

		LCD_ShowString(30, 253, "TABLE 5", BLACK, WHITE);
		if(new_flag[5]) LCD_ShowString(38, 253, "[NEW]" , BLACK, WHITE);

		LCD_ShowString(150, 253, "TABLE 6", BLACK, WHITE);
		if(new_flag[6]) LCD_ShowString(158, 253, "[NEW]" , BLACK, WHITE);

		Touch_GetXY(pos_x, pos_y, 1);
		Convert_Pos(*pos_x, *pos_y, &pix_x, &pix_y);
		if (pix_x >= 0 && pix_x <= 120 && pix_y >= 0 && pix_y <= 103)
		{
			*commandUI = 1;
			tableNumber = 1;
			new_flag[tableNumber] = 0;
		}
		else if (pix_x >= 120 && pix_x <= 240 && pix_y >= 0 && pix_y <= 103)
		{
			*commandUI = 1;
			tableNumber = 2;
			new_flag[tableNumber] = 0;
		}
		else if (pix_x >= 0 && pix_x <= 120 && pix_y >= 103 && pix_y <= 206)
		{
			*commandUI = 1;
			tableNumber = 3;
			new_flag[tableNumber] = 0;
		}
		else if (pix_x >= 120 && pix_x <= 240 && pix_y >= 103 && pix_y <= 206)
		{
			*commandUI = 1;
			tableNumber = 4;
			new_flag[tableNumber] = 0;
		}
		else if (pix_x >= 0 && pix_x <= 120 && pix_y >= 206 && pix_y <= 320)
		{
			*commandUI = 1;
			tableNumber = 5;
			new_flag[tableNumber] = 0;
		}
		else if (pix_x >= 120 && pix_x <= 240 && pix_y >= 206 && pix_y <= 320)
		{
			*commandUI = 1;
			tableNumber = 6;
			new_flag[tableNumber] = 0;
		}
	}
	LCD_Clear(WHITE);
	return tableNumber;
}
