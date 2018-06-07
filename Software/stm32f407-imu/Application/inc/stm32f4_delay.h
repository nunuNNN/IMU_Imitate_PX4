#ifndef _STM32F4_DELAY_H
#define _STM32F4_DELAY_H

#include "stm32f4xx.h"

void Delay_Init(void);
void Delay_Ms(u32 ms);
void Delay_Us(u32 value);

int Get_Ms(unsigned long *count);

u32 Micros(void);
u32 Millis(void);

#endif
