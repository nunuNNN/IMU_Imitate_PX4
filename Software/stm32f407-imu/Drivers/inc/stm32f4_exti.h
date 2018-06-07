/*
The MIT License (MIT)

Copyright (c) 2015-? suhetao

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _STM32F4_EXTI_H
#define _STM32F4_EXTI_H

#include "stm32f4xx.h"
#include "stm32f4_rcc.h"

typedef struct EXTI_DRIVER_T
{
	GPIO_TypeDef* Gpio;
	RCC_AXXPeriphClockCmd GPIO_CLK;
	uint32_t GPIO_Func;
	uint16_t GPIO_Pin;
	uint8_t EXTI_PortSourceGPIO;
	uint8_t EXTI_PinSource;
	
	EXTI_InitTypeDef EXIT_Init;
	NVIC_InitTypeDef NVIC_Init;	
}EXTI_Driver;

void EXTIx_Init(EXTI_Driver* EXTIx);

#endif
