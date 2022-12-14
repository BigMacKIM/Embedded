#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "lcd.h"
#include "touch.h"

volatile uint32_t ADC_Value[1];

void RCC_Configure() {
	//clock Enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // PB0
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // ADC1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // DMA1
}

void GPIO_Configure() {
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PIN B0 = ADC_Channel_8
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //AIN = Analog INput
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ADC_Configure() {
  ADC_InitTypeDef ADC_InitStructure;

  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //use ADC1 only
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1; //use one channel

  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));

  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  }

void DMA_Init2() {
  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(DMA1_Channel1); //channel 11
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1 ->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ADC_Value;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

int main() {
  SystemInit();
  RCC_Configure();
  GPIO_Configure();
  ADC_Configure();
  DMA_Init2();

  LCD_Init();
  Touch_Configuration();
  Touch_Adjust();
  LCD_Clear(WHITE);

  uint32_t threshold = 800;
  uint32_t flag = 0;
  uint32_t isGray = 0;

  while(1){
    if (!isGray && ADC_Value[0] > threshold)  flag = 1;
    if (isGray && ADC_Value[0] <= threshold)  flag = 1;

    if (flag) {
        if (isGray) {
            LCD_Clear(WHITE);
            isGray = 0;
        } else {
            LCD_Clear(GRAY);
            isGray = 1;
        }
        flag = 0;
    }

    LCD_ShowNum(20, 100, ADC_Value[0], 4, BLACK, WHITE);
  }
}

