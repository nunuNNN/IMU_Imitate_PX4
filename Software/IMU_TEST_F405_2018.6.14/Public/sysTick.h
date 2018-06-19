#ifndef __SYS_TICK_H
#define __SYS_TICK_H


#include "stmHead.h"

extern void sysTick_Init(void);

extern uint32_t sysTick_GetTick(void);

extern void sysTick_DelayMs(__IO uint32_t Delay);



#endif





























