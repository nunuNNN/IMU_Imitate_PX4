/*
******************************************************************************
*@文件名字  : DrvTim.h
*@描述	    : Tim相关底层驱动，裸机和RTOS都支持
******************************************************************************
******************************************************************************
*/

#ifndef _DRV_TIM_H_
#define _DRV_TIM_H_


#ifdef __cplusplus
 extern "C" {
#endif 


#include "stmHead.h"


typedef enum
{
	ID_TIM2,
	ID_TIM3,	
	ID_TIM4,	
  ID_TIM5,
  ID_TIM_NUM, //TIM个数
}TIM_ID; 

   
extern void DrvTim_Init(TIM_ID timeId, u32 loopHz);
extern void DrvTim_RegisterIsrHandler(void *pfun, TIM_ID timId);



#ifdef __cplusplus
}
#endif


#endif

/*********END OF FILE****/


