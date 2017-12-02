#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "Lb_Printf.h"
#include "Lb_BT_Printf.h"

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
   char c;

   if (GetBluetoothKey(&c) == 1) { //Get Data Using RX's Signal of USART2
      Lb_printf("%c", c); //Send Data Using TX's Signal of USART1
   }
}

int main() {
   SystemInit();
   UsartInit();
   while (1) {
   }
}
