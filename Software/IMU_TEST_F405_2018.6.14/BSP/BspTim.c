
#include "BspTim.h"


void BspTimer_Init(TIMER_STRUCT *pTim)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if((pTim->TIMx == TIM1) || (pTim->TIMx == TIM8)|| (pTim->TIMx == TIM9) ||
		 (pTim->TIMx == TIM10)|| (pTim->TIMx == TIM11) ) 
  {	
		RCC_APB2PeriphClockCmd(pTim->TIMx_RCC_Periph, ENABLE); 
	}
	else
	{
		RCC_APB1PeriphClockCmd(pTim->TIMx_RCC_Periph, ENABLE); 		
	}
	
  TIM_TimeBaseInitStructure.TIM_Period        = pTim->TIMx_Period; 
	TIM_TimeBaseInitStructure.TIM_Prescaler     = pTim->TIMx_Prescaler;   
	TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	
	TIM_TimeBaseInit(pTim->TIMx, &TIM_TimeBaseInitStructure);
	 
	TIM_Cmd(pTim->TIMx, ENABLE);
	
	if( true == pTim->IsUseIsr )
	{
		TIM_ITConfig(pTim->TIMx, TIM_IT_Update, ENABLE);

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	
		NVIC_InitStructure.NVIC_IRQChannel                   = pTim->TIMx_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pTim->IRQ_MainPriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority        = pTim->IRQ_SubPriority; 
		NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}
