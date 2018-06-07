#ifndef __STM32F4_GPIO_H
#define __STM32F4_GPIO_H

// Includes
#include "stm32f4xx.h"
#include "stm32f4_rcc.h"


typedef struct GPIO_DRIVER_T
{
	GPIO_TypeDef* Gpio;
	RCC_AXXPeriphClockCmd GPIO_CLK;
	uint32_t GPIO_Func;
	GPIO_InitTypeDef GPIO_Init;
}GPIO_Driver;

void GPIOx_Init(GPIO_Driver* GPIOx);

__inline void GPIOx_SetLow(GPIO_Driver* GPIOx)
{
	GPIO_ResetBits(GPIOx->Gpio, GPIOx->GPIO_Init.GPIO_Pin);
}

__inline void GPIOx_SetHigh(GPIO_Driver* GPIOx){
	GPIO_SetBits(GPIOx->Gpio, GPIOx->GPIO_Init.GPIO_Pin);
}

#endif
