/*
******************************************************************************
*@文件名字  : DrvTim.c
*@描述	    : Tim相关底层驱动，裸机和RTOS都支持
******************************************************************************
******************************************************************************
*/


#include "DrvTim.h"
#include "BspTim.h"

#define TIMER_MAX_TASK_NUM (1)

typedef struct
{
	TIMER_STRUCT Timer;
	u32 timHz;
  u8 maxTaskNum;
  u8 usedTaskNum; 
  void (*pTimerCallBackProc)(TIM_ID timId);

}DRV_TIMER_STRUCT;

DRV_TIMER_STRUCT drvTimerArry[ID_TIM_NUM];
void (*pDrvTimerTask[ID_TIM_NUM][TIMER_MAX_TASK_NUM])(TIM_ID timId, u32 timHz) = {NULL};
void Timer_isrProc(TIM_ID timId);


static void DrvTim_SetTimerParam(DRV_TIMER_STRUCT *pDrvTim, u32 loopHz)
{
  u32 timeClk = SystemCoreClock;
  
//	if((pDrvTim->Timer.TIMx == TIM1) || (pDrvTim->Timer.TIMx == TIM8)|| (pDrvTim->Timer.TIMx == TIM9) ||
//		 (pDrvTim->Timer.TIMx == TIM10)|| (pDrvTim->Timer.TIMx == TIM11) ) 
//  {
//    timeClk = SystemCoreClock;
//  }
//  else
//  {
//    timeClk = SystemCoreClock/2;
//  }
	
  pDrvTim->timHz = loopHz;
	pDrvTim->Timer.TIMx_Prescaler = timeClk/1000000u - 1;
	pDrvTim->Timer.TIMx_Period = timeClk/(loopHz*(pDrvTim->Timer.TIMx_Prescaler + 1)) - 1;
  
}

/******************************************************************************
* @函 数 名： DrvTim_Init
* @函数描述： 初始化Tim
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvTim_Init(TIM_ID timeId, u32 loopHz)
{
	if(ID_TIM2 == timeId)
	{
		drvTimerArry[timeId].Timer.IsUseIsr = false;
		drvTimerArry[timeId].Timer.TIMx = TIM2;
		drvTimerArry[timeId].Timer.TIMx_RCC_Periph = RCC_APB1Periph_TIM2;
    DrvTim_SetTimerParam(&drvTimerArry[timeId], loopHz);
		BspTimer_Init(&drvTimerArry[timeId].Timer);	
	}	
	else if(ID_TIM3 == timeId)
	{
		drvTimerArry[timeId].Timer.IsUseIsr = true;
		drvTimerArry[timeId].Timer.TIMx = TIM3;
		drvTimerArry[timeId].Timer.TIMx_RCC_Periph = RCC_APB1Periph_TIM3;
		drvTimerArry[timeId].Timer.TIMx_IRQn = TIM3_IRQn;
		drvTimerArry[timeId].Timer.IRQ_MainPriority = 4;
		drvTimerArry[timeId].Timer.IRQ_SubPriority = 1;
		drvTimerArry[timeId].maxTaskNum = TIMER_MAX_TASK_NUM;
		drvTimerArry[timeId].usedTaskNum = 0;
    DrvTim_SetTimerParam(&drvTimerArry[timeId], loopHz);		
		BspTimer_Init(&drvTimerArry[timeId].Timer);	
	}
	else if(ID_TIM4 == timeId)
	{		
		drvTimerArry[timeId].Timer.IsUseIsr = true;
		drvTimerArry[timeId].Timer.TIMx = TIM4;
		drvTimerArry[timeId].Timer.TIMx_RCC_Periph = RCC_APB1Periph_TIM4;
    drvTimerArry[timeId].Timer.TIMx_IRQn = TIM4_IRQn;
		drvTimerArry[timeId].Timer.IRQ_MainPriority = 2;
		drvTimerArry[timeId].Timer.IRQ_SubPriority = 1;
		drvTimerArry[timeId].maxTaskNum = TIMER_MAX_TASK_NUM;
		drvTimerArry[timeId].usedTaskNum = 0;
    DrvTim_SetTimerParam(&drvTimerArry[timeId], loopHz);		
		BspTimer_Init(&drvTimerArry[timeId].Timer);	
	}
	else if(ID_TIM5 == timeId)
	{
		drvTimerArry[timeId].Timer.IsUseIsr = true;
		drvTimerArry[timeId].Timer.TIMx = TIM5;
		drvTimerArry[timeId].Timer.TIMx_RCC_Periph = RCC_APB1Periph_TIM5;
    drvTimerArry[timeId].Timer.TIMx_IRQn = TIM5_IRQn;
		drvTimerArry[timeId].Timer.IRQ_MainPriority = 3;
		drvTimerArry[timeId].Timer.IRQ_SubPriority = 1;
		drvTimerArry[timeId].maxTaskNum = TIMER_MAX_TASK_NUM;
		drvTimerArry[timeId].usedTaskNum = 0;
    DrvTim_SetTimerParam(&drvTimerArry[timeId], loopHz);		
		BspTimer_Init(&drvTimerArry[timeId].Timer);	
	}
}

/******************************************************************************
* @函 数 名： DrvTim_RegisterIsrHandler
* @函数描述： 注册timer定时器中断处理函数
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/

void DrvTim_RegisterIsrHandler(void *pfun, TIM_ID timId)
{
  if (drvTimerArry[timId].usedTaskNum < drvTimerArry[timId].maxTaskNum)
  {
    pDrvTimerTask[timId][drvTimerArry[timId].usedTaskNum++] = ( void(*)(TIM_ID, u32) )pfun;
  }
}


/******************************************************************************
* @函 数 名： Timer_isrProc
* @函数描述： timer定时器中断处理函数
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void Timer_isrProc(TIM_ID timId)
{
  int i = 0;
  TIM_ID isrTimeId = timId;
  void (*pTask)(TIM_ID, u32);
   
  if(isrTimeId >= ID_TIM_NUM)  
  {
    return;
  }

  for( i = 0; i < drvTimerArry[isrTimeId].usedTaskNum; i++ ) 
  {
    if (pDrvTimerTask[isrTimeId][i] != NULL)
    {
      pTask = ( void (*)(TIM_ID, u32) ) pDrvTimerTask[isrTimeId][i];
      pTask(isrTimeId, drvTimerArry[isrTimeId].timHz);
    }
  }  
}


/******************************************************************************
* @函 数 名： TIM3_IRQHandler
* @函数描述： timer3中断
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
        
    Timer_isrProc(ID_TIM3);
  }
}

/******************************************************************************
* @函 数 名： TIM4_IRQHandler
* @函数描述： timer4中断
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void TIM4_IRQHandler(void)
{	
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
    
    Timer_isrProc(ID_TIM4);
  }
}

/******************************************************************************
* @函 数 名： TIM5_IRQHandler
* @函数描述： timer5中断
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void TIM5_IRQHandler(void)
{	
  if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);
    
    Timer_isrProc(ID_TIM5);
  }
}



/*********END OF FILE****/


