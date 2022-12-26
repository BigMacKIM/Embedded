#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "lcd.h"
#include "touch.h"

void sendDataUART1(uint16_t data);
void sendDataUART2(uint16_t data);
void changemotor(char c);




void RCCInit() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
}

void GpioInit() {
  /*MOTOR pin setting */
  GPIO_InitTypeDef GPIO_InitStructure_IN_A1; //A6
  GPIO_InitStructure_IN_A1.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure_IN_A1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_IN_A1.GPIO_Mode = GPIO_Mode_AF_PP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure_IN_A1);

  GPIO_InitTypeDef GPIO_InitStructure_IN_A2; //A7
  GPIO_InitStructure_IN_A2.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure_IN_A2.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_IN_A2.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure_IN_A2);

  GPIO_InitTypeDef GPIO_InitStructure_IN_B1; //B0
  GPIO_InitStructure_IN_B1.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure_IN_B1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_IN_B1.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure_IN_B1);

  GPIO_InitTypeDef GPIO_InitStructure_IN_B2; //B1
  GPIO_InitStructure_IN_B2.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure_IN_B2.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_IN_B2.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure_IN_B2);

  /*SENSOR pin setting */
  GPIO_InitTypeDef GPIO_InitStructure_trig;      //PA0
  GPIO_InitStructure_trig.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure_trig.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_trig.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure_trig);

  GPIO_InitTypeDef GPIO_InitStructure_echo;      //PA1
  GPIO_InitStructure_echo.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure_echo.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_echo.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure_echo);

   /* USART1 pin setting */
  //TX
  GPIO_InitTypeDef GPIO_InitStructure_TX_1;   //USART1 //A9
  GPIO_InitStructure_TX_1.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure_TX_1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_TX_1.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure_TX_1);
  //RX
  GPIO_InitTypeDef GPIO_InitStructure_RX_1; //USART1 //A10
  GPIO_InitStructure_RX_1.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure_RX_1.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_RX_1.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure_RX_1);

  /* USART2 pin setting */
  //TX
  GPIO_InitTypeDef GPIO_InitStructure_TX_2; //USART2 //A2
  GPIO_InitStructure_TX_2.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure_TX_2.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_TX_2.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure_TX_2);
  //RX
  GPIO_InitTypeDef GPIO_InitStructure_RX_2; //USART2 //A3
  GPIO_InitStructure_RX_2.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure_RX_2.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure_RX_2.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure_RX_2);
}

void USART1_Init(void)
{
   USART_InitTypeDef USART1_InitStructure;

   USART_Cmd(USART1, ENABLE);

   USART1_InitStructure.USART_BaudRate = 9600;
   USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART1_InitStructure.USART_Parity = USART_Parity_No;
   USART1_InitStructure.USART_StopBits = USART_StopBits_1;
   USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_Init(USART1, &USART1_InitStructure);

   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART2_Init(void)
{
   USART_InitTypeDef USART2_InitStructure;

   USART_Cmd(USART2, ENABLE);

   USART2_InitStructure.USART_BaudRate = 9600;
   USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART2_InitStructure.USART_Parity = USART_Parity_No;
   USART2_InitStructure.USART_StopBits = USART_StopBits_1;
   USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_Init(USART2, &USART2_InitStructure);

   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void NvicInit() {

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);


  // USART1
  NVIC_InitTypeDef NVIC_InitStructure_USART1;
  NVIC_EnableIRQ(USART1_IRQn);
  NVIC_InitStructure_USART1.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure_USART1.NVIC_IRQChannelPreemptionPriority = 0x00; // TODO
  NVIC_InitStructure_USART1.NVIC_IRQChannelSubPriority = 0x01; // TODO
  NVIC_InitStructure_USART1.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure_USART1);
  // USART2
  NVIC_InitTypeDef NVIC_InitStructure_USART2;
  NVIC_EnableIRQ(USART2_IRQn);
  NVIC_InitStructure_USART2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure_USART2.NVIC_IRQChannelPreemptionPriority = 0x00; // TODO
  NVIC_InitStructure_USART2.NVIC_IRQChannelSubPriority = 0x02; // TODO
  NVIC_InitStructure_USART2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure_USART2);

  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM_configure() {
  //Micro count
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  TIM_TimeBaseStructure.TIM_Period = 10;
  TIM_TimeBaseStructure.TIM_Prescaler =72;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  // PWM
  TIM_TimeBaseInitTypeDef TIM3_TimeBaseStructure;
  TIM3_TimeBaseStructure.TIM_Period = 200;
  TIM3_TimeBaseStructure.TIM_Prescaler = 18;                //set 50Hz
  TIM3_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
  TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM3,ENABLE);
  TIM_Cmd(TIM3,ENABLE);
}

uint32_t us = 0, sec = 0, ms=0;

int32_t t_trig, t_echo = 0;
int16_t echo_flag = -1;     // ÃÊ±â
int32_t dist_start = 0;
int32_t dist_end = 0;
int32_t dist = 0;


void TIM2_IRQHandler(void) {
  us++;
  ms++;
  if (us % 100000 == 0) { sec++; }
  if(us == 0xffffffff) us = 1;

  t_trig++;
  t_echo++;
  //if(t_echo==0xffff) t_echo = 1;
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

}

void USART1_IRQHandler() {
    if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET){
        char word;
       // the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);
        sendDataUART2(word);

        // TODO implement
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
        changemotor(word);
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

void changemotor(char c){  //(percent,move) 3.5 (-90),  7.5(0), 10(90)

  TIM_OCInitTypeDef HIGH, LOW;

  HIGH.TIM_OCMode = TIM_OCMode_PWM1;
  HIGH.TIM_OCPolarity = TIM_OCPolarity_High;
  HIGH.TIM_OutputState = TIM_OutputState_Enable;
  HIGH.TIM_Pulse = 200;

  LOW.TIM_OCMode = TIM_OCMode_Inactive;

  if(c == 'w') {
    TIM_OC1Init(TIM3, &HIGH);
    TIM_OC2Init(TIM3, &LOW);
    TIM_OC3Init(TIM3, &HIGH);
    TIM_OC4Init(TIM3, &LOW);
  }
  else if(c == 'x') {
    TIM_OC1Init(TIM3, &LOW);
    TIM_OC2Init(TIM3, &HIGH);
    TIM_OC3Init(TIM3, &LOW);
    TIM_OC4Init(TIM3, &HIGH);
  }
  else if(c == 'a') {
    TIM_OC1Init(TIM3, &HIGH);
    TIM_OC2Init(TIM3, &LOW);
    TIM_OC3Init(TIM3, &LOW);
    TIM_OC4Init(TIM3, &LOW);
  }
  else if(c == 'd') {
    TIM_OC1Init(TIM3, &LOW);
    TIM_OC2Init(TIM3, &LOW);
    TIM_OC3Init(TIM3, &HIGH);
    TIM_OC4Init(TIM3, &LOW);
  }
  else if(c == 's') {
    TIM_OC1Init(TIM3, &LOW);
    TIM_OC2Init(TIM3, &LOW);
    TIM_OC3Init(TIM3, &LOW);
    TIM_OC4Init(TIM3, &LOW);
  }
}

char dist_word[3];

void delay() {
  ms=0;

  while(ms<10001) {
  }
}

void trigger_pulse() {

    GPIO_SetBits(GPIOA, GPIO_Pin_0);

    t_trig=0;
    while(t_trig<5);
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);

    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != Bit_SET);
    t_echo=0;

    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != Bit_RESET);
    dist = (t_echo)/6;
    LCD_ShowNum(50, 50, dist, 10, BLACK, WHITE);

    int i = 2;
    while (dist > 0){
      dist_word[i] = (dist%10) + '0';
      dist = dist/10;
      i--;
    }
    while (i >= 0){
      dist_word[i] = '0';
      i--;
    }
    for(int i=0; i<3; i++){
      sendDataUART2(dist_word[i]);
      delay();
    }
    sendDataUART2('\n');
    delay();
    return;
}



int main() {
  SystemInit();
  RCCInit();
  GpioInit();
  USART1_Init();
  TIM_configure();
  USART2_Init();
  NvicInit();
  LCD_Init();
  Touch_Configuration();
  //Touch_Adjust();
  LCD_Clear(WHITE);


  while(1) {
    trigger_pulse();
    delay();
  }
}
