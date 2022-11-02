#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void EXTI_Configure(void);
void USART1_Init(void);
void NVIC_Configure(void);

void EXTI15_10_IRQHandler(void);

void Delay(void);

void sendDataUART1(uint16_t data);
void sendDataUART2(uint16_t data); 


//---------------------------------------------------------------------------------------------------

void RCC_Configure(void)
{
   // TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'

   /* UART TX/RX port clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

   /* JoyStick Up/Down port clock enable */
   //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

   /* JoyStick Selection port clock enable */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

   /* LED port clock enable */
   //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

   /* USART1 clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
   
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

   /* Alternate Function IO clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void)
{
    //GPIO_InitTypeDef GPIO_InitStructure;
    //GPIO_InitTypeDef GPIO_InitStructure_Joy;
//  GPIO_InitTypeDef GPIO_InitStructure_Joysel;
    //GPIO_InitTypeDef GPIO_InitStructure_btn;
    //GPIO_InitTypeDef GPIO_InitStructure_UART;
    GPIO_InitTypeDef GPIO_InitStructure_TX;
    GPIO_InitTypeDef GPIO_InitStructure_RX;
    GPIO_InitTypeDef GPIO_InitStructure_TX_1; // USART1
    GPIO_InitTypeDef GPIO_InitStructure_RX_1; // USART1

   // TODO: Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'

    /* JoyStick up, down pin setting */
//   GPIO_InitStructure_Joy.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_2;
//   GPIO_InitStructure_Joy.GPIO_Speed = GPIO_Speed_50MHz;
  // GPIO_InitStructure_Joy.GPIO_Mode = GPIO_Mode_IPU;
  // GPIO_Init(GPIOC, &GPIO_InitStructure_Joy);
   /* JoyStick selection pin setting */

    /* button pin setting */
 //   GPIO_InitStructure_btn.GPIO_Pin = GPIO_Pin_11;
 //   GPIO_InitStructure_btn.GPIO_Speed = GPIO_Speed_50MHz;
 //   GPIO_InitStructure_btn.GPIO_Mode = GPIO_Mode_IPU;
 //   GPIO_Init(GPIOD, &GPIO_InitStructure_btn);

    /* LED pin setting*/
 //   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
  //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //   GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* UART2 pin setting */
    //TX
    GPIO_InitStructure_TX.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure_TX.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure_TX.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure_TX);
    //RX
    GPIO_InitStructure_RX.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure_RX.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure_RX.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure_RX);
    
    /* UART1 pin setting */
    //TX
    GPIO_InitStructure_TX.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure_TX.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure_TX.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure_TX);
    //RX
    GPIO_InitStructure_RX.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure_RX.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure_RX.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure_RX);
}

void USART1_Init(void)
{
   USART_InitTypeDef USART1_InitStructure;

   // Enable the USART1 peripheral
   USART_Cmd(USART1, ENABLE);

   // TODO: Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
   USART1_InitStructure.USART_BaudRate = 9600;
   USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART1_InitStructure.USART_Parity = USART_Parity_No;
   USART1_InitStructure.USART_StopBits = USART_StopBits_1;
   USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_Init(USART1, &USART1_InitStructure);

   // TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART2_Init(void) 
{
   USART_InitTypeDef USART2_InitStructure;

   // Enable the USART1 peripheral
   USART_Cmd(USART2, ENABLE);

   // TODO: Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
   USART2_InitStructure.USART_BaudRate = 9600;
   USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART2_InitStructure.USART_Parity = USART_Parity_No;
   USART2_InitStructure.USART_StopBits = USART_StopBits_1;
   USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_Init(USART2, &USART2_InitStructure);

   // TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure2;

    // TODO: fill the arg you want
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

   // TODO: Initialize the NVIC using the structure 'NVIC_InitTypeDef' and the function 'NVIC_Init'

    
    // UART1
   // 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // TODO
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // UART2 
   // 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0x00; // TODO
    NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0x01; // TODO
    NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure2);
}

void USART1_IRQHandler() {
    char word;
    if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET){
       // the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);
        // TODO implement
        sendDataUART2(word);

        // clear 'Read data register not empty' flag
       USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void USART2_IRQHandler() { 
   char word;
    if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET){
       // the most recent received data by the USART2 peripheral
        word = USART_ReceiveData(USART2);
        // TODO implement
        sendDataUART1(word);

        // clear 'Read data register not empty' flag
       USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void sendDataUART1(uint16_t data) {
	/* Wait till TC is set */
	//while ((USART1->SR & USART_SR_TC) == 0);
	USART_SendData(USART1, data);
}

void sendDataUART2(uint16_t data) { 
	/* Wait till TC is set */
	//while ((USART2->SR & USART_SR_TC) == 0);
	USART_SendData(USART2, data);
}



int main(void)
{

    SystemInit();

    RCC_Configure();

    GPIO_Configure();

    USART1_Init();
    
    USART2_Init();

    NVIC_Configure();

    while (1) {
    }
    return 0;
}
