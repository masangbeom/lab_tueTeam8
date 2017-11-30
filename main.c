#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_tim.h" // timer�� �����ϱ� ���� �Լ��� ������
#include "lcd.h"
#include "misc.h"
#include "printFunc.h"
#include "touch.h"
#include "stdlib.h"
#include "init.h"

#define WAIT_ORDER 0
#define NEW_ORDER 1

char tb_st[7][512] = { {'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'},{'\0'} };

int new_flag[7];

int bufBLE, tableNUM;
int sizeBLE[7];

int commandUI = 0;
int printTableNUM = 0;

uint16_t pos_x, pos_y;


//Occured When detect Rx of USART2
void USART2_IRQHandler(void) {
    if ((USART2 -> SR & USART_FLAG_RXNE) != (u16) RESET) {
        bufBLE = USART_ReceiveData(USART2);
        if (bufBLE == '@') {
            tableNUM = 1;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
            sizeBLE[tableNUM] = 0;
            bufBLE = ' ';
        }
        else if (bufBLE == '#') {
            tableNUM = 2;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
            sizeBLE[tableNUM] = 0;
            bufBLE = ' ';
        }
        else if (bufBLE == '$') {
            tableNUM = 3;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
            sizeBLE[tableNUM] = 0;
            bufBLE = ' ';
        }
        else if (bufBLE == '%') {
            tableNUM = 4;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
            sizeBLE[tableNUM] = 0;
            bufBLE = ' ';
        }
        else if (bufBLE == '^') {
            tableNUM = 5;
            new_flag[tableNUM] = 1;
            tb_st[tableNUM][0] = '\0';
            sizeBLE[tableNUM] = 0;
            bufBLE = ' ';
        }
        else if (bufBLE == '&') {
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
   if(new_flag[1]) LCD_ShowString(38, 69, "[NEW]" , GREEN, WHITE);
   if(new_flag[2]) LCD_ShowString(158, 69, "[NEW]" , GREEN, WHITE);
   if(new_flag[3]) LCD_ShowString(38, 169, "[NEW]" , GREEN, WHITE);
   if(new_flag[4]) LCD_ShowString(158, 169, "[NEW]" , GREEN, WHITE);
   if(new_flag[6]) LCD_ShowString(158, 269, "[NEW]" , GREEN, WHITE);
   TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
   //Clears the TIMx's interrupt pending bits.
}



int main() {
	SystemInit();
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

		}
	}
}
