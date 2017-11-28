#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "lcd.h"
#include "misc.h"
#include "touch.h"
#include "Lb_Printf.h"
#include "Lb_BT_Printf.h"
#include "stdlib.h"
#define WAIT_ORDER 0
#define NEW_ORDER 1
#define FLAG_BLE_CONNECT '@'
char st[2048] = {'\0'};
char showString[2048] = {'\0'};
int bufBLE, sizeBLE;
int countStart = 1;
int commandUI = 0;
uint16_t pos_x, pos_y;
//Send Data Using TX of USART1
void print_byte(unsigned short c) {
   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
      ;
   USART_SendData(USART1, c);
}

//Send Data Using TX of USART2
void print_byte_to_Bluetooth(unsigned short c) {
   while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
      ;
   USART_SendData(USART2, c);
}

//Detect RX Data of USART1 - PC
int GetPCKey(char *pkey) {
   int ret = 0;

   if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) {
      *pkey = (u8) USART_ReceiveData(USART1);
      ret = 1;
   }
   return ret;
}
//Detect RX Data of USART2 - Bluetooth
int GetBluetoothKey(char *pkey) {
   int ret = 0;

   if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET) {
      *pkey = (u8) USART_ReceiveData(USART2);
      ret = 1;
   }
   return ret;
}
//UsartInit
void UsartInit(void) {
   GPIO_InitTypeDef GPIO_InitStructure;
   USART_InitTypeDef USART_InitStructure1;

   USART_InitTypeDef USART_InitStructure2;
   NVIC_InitTypeDef NVIC_InitStruct_UART1;
   NVIC_InitTypeDef NVIC_InitStruct_UART2;
   // RCC Configuration
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,
         ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

   /* PA9 */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* PA10 */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* PA2 */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* PA3 */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* UART1 Port Configuration */
   USART_InitStructure1.USART_BaudRate = 9600;
   USART_InitStructure1.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure1.USART_StopBits = USART_StopBits_1;
   USART_InitStructure1.USART_Parity = USART_Parity_No;
   USART_InitStructure1.USART_HardwareFlowControl =
         USART_HardwareFlowControl_None;
   USART_InitStructure1.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(USART1, &USART_InitStructure1);

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
   NVIC_InitStruct_UART1.NVIC_IRQChannel = USART1_IRQn;
   NVIC_InitStruct_UART1.NVIC_IRQChannelPreemptionPriority = 0x0;
   NVIC_InitStruct_UART1.NVIC_IRQChannelSubPriority = 0x00;
   NVIC_InitStruct_UART1.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStruct_UART1);
   /* Enable NVIC for UART Interrupt */
   NVIC_InitStruct_UART2.NVIC_IRQChannel = USART2_IRQn;
   NVIC_InitStruct_UART2.NVIC_IRQChannelPreemptionPriority = 0x0;
   NVIC_InitStruct_UART2.NVIC_IRQChannelSubPriority = 0x00;
   NVIC_InitStruct_UART2.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStruct_UART2);

   /* Enable Receive interrupts */
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

   USART_Cmd(USART1, ENABLE);
   USART_Cmd(USART2, ENABLE);
}

//Occured When detect Rx of USART1
void USART1_IRQHandler(void) {
   char c;

   if (GetPCKey(&c) == 1) { //Get Data Using RX's Signal of USART1
      Lb_BT_printf("%c", c); //Send Data Using TX's Signal of USART2
   }
}

//Occured When detect Rx of USART2
void USART2_IRQHandler(void) {
		if ((USART2->SR & USART_FLAG_RXNE) != (u16)RESET)
	    {
	        bufBLE = USART_ReceiveData(USART2);

	         if(countStart != 1){
	            if(sizeBLE == 512)
	            {
	                st[sizeBLE] = bufBLE;
	                sizeBLE = 0;
	            }
	            else
	            {
	                st[sizeBLE++] = bufBLE;
	            }
	            st[sizeBLE] = '\0';
			}
	    }
	countStart = 0;
}

void printOrderList(){
	uint16_t pix_x,pix_y;
	LCD_Clear(WHITE);
	st = '\0';
	while(commandUI == 1){
		LCD_ShowString(10,30,st,BLACK,WHITE);
		LCD_DrawLine(0, 240, 240, 240);
		Touch_GetXY(&pos_x, &pos_y, 1);
		Convert_Pos(pos_x,pos_y,&pix_x,&pix_y);
		if(pix_x >= 0 && pix_x <= 240 && pix_y >= 240 && pix_y <= 320 ){
			commandUI = 0;
		}
	}
}

void printWaitUI(){
	uint16_t pix_x,pix_y;
	while(commandUI == 0){
		LCD_DrawLine(0, 103, 240, 103);
		LCD_DrawLine(0, 206, 240, 206);
		LCD_DrawLine(120, 0, 120, 320);
		LCD_ShowString(30, 53, "TABLE 1", BLACK,WHITE);
		LCD_ShowString(150, 53, "TABLE 2", BLACK,WHITE);
		LCD_ShowString(30, 153, "TABLE 3", BLACK,WHITE);
		LCD_ShowString(150, 153, "TABLE 4", BLACK,WHITE);
		LCD_ShowString(30, 253, "TABLE 5", BLACK,WHITE);
		LCD_ShowString(150, 253, "TABLE 6", BLACK,WHITE);
		Touch_GetXY(&pos_x, &pos_y, 1);
		Convert_Pos(pos_x,pos_y,&pix_x,&pix_y);
		if(pix_x >= 0 && pix_x <= 120 && pix_y >= 0 && pix_y <= 103) commandUI = 1; // Table 1
		else if(pix_x >= 120 && pix_x <= 240 && pix_y >= 0 && pix_y <= 103) commandUI = 1; // Table 2
		else if(pix_x >= 0 && pix_x <= 120 && pix_y >= 103 && pix_y <= 206) commandUI = 1; // Table 3
		else if(pix_x >= 120 && pix_x <= 240 && pix_y >= 103 && pix_y <= 206) commandUI = 1; // Table 4
		else if(pix_x >= 0 && pix_x <= 120 && pix_y >= 206 && pix_y <= 320) commandUI = 1; // Table 5
		else if(pix_x >= 120 && pix_x <= 240 && pix_y >= 206 && pix_y <= 320) commandUI = 1; // Table 6
	}
	LCD_Clear(WHITE);
}
int main() {
   SystemInit();
   UsartInit();
   LCD_Init();
   Touch_Configuration();
   Touch_Adjust();
   LCD_Clear(WHITE);

   while (1) {
	   switch(commandUI){
	   	   case WAIT_ORDER:{
	   		   printWaitUI(); break;
	   	   }
	   	   case NEW_ORDER:{
	   		   printOrderList(); break;
	   	   }
	   	   default: {
	   		printWaitUI(); break;
	   	   }

	   }

   }
}
