/*
******************************************************************************
*@文件名字  : AppMain.h
*@描述        : App层规整
******************************************************************************
******************************************************************************
*/

/*****************************************************
Driver层是对BSP层的封装，提供App层驱动
*****************************************************/

#ifndef _APP_MAIN_H_
#define _APP_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif


/* USER CODE BEGIN 0*/
#include "sysTick.h"
#include "public.h"
#include "AppIMU.h"
#include "AppComm.h"
#include "AppTimer.h"
#include "DrvGpio.h"
/* USER CODE END   0*/

#define APP_TASK   (5)

typedef struct
{
    void (*pTask)(void);
    u32 Period;
    u32 Tick;
} sTask;

extern sTask appTask[APP_TASK];

extern void AppInit(void);
extern void AppMain(void);



#ifdef __cplusplus
}
#endif

#endif


/*********END OF FILE****/

