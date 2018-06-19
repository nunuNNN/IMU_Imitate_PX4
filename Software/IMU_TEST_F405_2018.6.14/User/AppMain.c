/*
******************************************************************************
*@文件名字  : AppMain.c
*@描述	    : App层规整
******************************************************************************
******************************************************************************
*/


#include "AppMain.h"


sTask appTask[APP_TASK];


void AppAdd_Task(void *pFun, u32 Period, u32 Tick )
{
	static u8 appTaskIndx = 0;

	if (appTaskIndx < APP_TASK )
	{
		if (pFun)
		{
			appTask[appTaskIndx].pTask  = (void(*)(void))pFun;
			appTask[appTaskIndx].Period = Period;
			appTask[appTaskIndx].Tick   = Tick;
			appTaskIndx++;
		}
	}
}



void AppInit(void)
{
	DrvLed_Init();
	AppIMU_Init();
	AppTimer_Init();
	AppComm_Init();
	
	//防止SW IO 被禁用
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource13 | GPIO_PinSource14, GPIO_AF_SWJ);
}


extern u8 sysShowDebugFlag;

void AppMain(void)
{
	while (1)
	{
		if(sysShowDebugFlag)
		{
			DrvLed_Toggle();
			sysShowDebugFlag = 0;
			//AppShowDebugData();
			AppShowData();
		}
	}
}



/************************ (C) COPYRIGHT FORWARDX *****END OF FILE****/

