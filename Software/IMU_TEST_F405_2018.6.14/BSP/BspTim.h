#ifndef _BSP_TIM_H
#define _BSP_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif 


#include "stmHead.h"
	 
typedef struct
{
	TIM_TypeDef* TIMx;
	uint32_t TIMx_RCC_Periph;
	u32 TIMx_Period; 
	u16 TIMx_Prescaler; 
	u8 TIMx_IRQn;
	u8 IRQ_MainPriority;
	u8 IRQ_SubPriority;
	bool IsUseIsr;
}TIMER_STRUCT;


extern void BspTimer_Init(TIMER_STRUCT *pTim);


#ifdef __cplusplus
  }
#endif
  
  
#endif /*_BSP_TIM_H*/


/*********END OF FILE****/
