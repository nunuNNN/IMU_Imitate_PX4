/**
  ******************************************************************************
  * @file    AppTimer.c
  * @brief
  ******************************************************************************
  ******************************************************************************
  */

#include "AppTimer.h"
#include "AppComm.h"
#include "AppIMU.h"

u8 sysShowDebugFlag = 0;
u8 sysCalibrateImuFlag = 0;
u32 sysRumTime1s = 0;

void gimbal_ekff_isr(TIM_ID timeId, u32 timHz);
void gimbal_readImu_isr(TIM_ID timeId, u32 timHz);
void gimbal_loopCore_isr(TIM_ID timeId, u32 timHz);
/*************************************************
 * @Function AppTimerInit
 * @param
 * @return
 * @additional 初始化定时器处理函数
 *
 *************************************************/
void AppTimer_Init(void)
{
	
	DrvTim_Init(ID_TIM2, 20);

	DrvTim_Init(ID_TIM3, 200);
	DrvTim_RegisterIsrHandler(&gimbal_ekff_isr, ID_TIM3);

	DrvTim_Init(ID_TIM4, 1000);
	DrvTim_RegisterIsrHandler(&gimbal_readImu_isr, ID_TIM4);
	
//	DrvTim_Init(ID_TIM5, 10000);
//	DrvTim_RegisterIsrHandler(&gimbal_loopCore_isr, ID_TIM5);

}


void AppTimer_ClearCount(void)
{
	TIM2->CNT = 0;
}


uint32_t AppTimer_GetCount(void)
{
	return (TIM2->CNT);
}


/*************************************************
 * @Function gimbal_ekff_isr
 * @param
 * @return
 * @additional ekff定时中断处理
 *
 *************************************************/
void gimbal_ekff_isr(TIM_ID timeId, u32 timHz)
{
#define _DT (1.0f/timHz)
	
	miniEKF_Update(&miniEKF, miniEKF.imu, _DT);
}



/*************************************************
 * @Function gimbal_readImu_isr
 * @param
 * @return
 * @additional  读取IMU数据
 *
 *************************************************/
void gimbal_readImu_isr(TIM_ID timeId, u32 timHz)
{
	u8 val;
	static unsigned int debugShowCount = 0;
	static unsigned int calibrateImuCnt = 0;	
	
	DrvImu_Read(IMU_MPU6500_Camera, timHz);
	
	debugShowCount++;
	if ( debugShowCount >= timHz/5)	 //发送给上位机
	{
		debugShowCount = 0;
		sysShowDebugFlag = 1;
	}	
	
	calibrateImuCnt++;
	if(calibrateImuCnt >= timHz)
	{
		calibrateImuCnt = 0;
		sysRumTime1s++;
		if(sysRumTime1s == 1)
		{		
			if(sysCalibrateImuFlag == 0)
			{
				sysCalibrateImuFlag = 1;
			}
		}		
	}
		
	if(sysCalibrateImuFlag == 1)
	{
		sysCalibrateImuFlag = 2;
	}
	else if(sysCalibrateImuFlag == 2)
	{
		val = AppIMU_CalibrateIMU(&imu_raw, &imu_bias, timHz);
		if( val )
		{
			 sysCalibrateImuFlag = 0;
		}		
	}
}


/*************************************************
 * @Function gimbal_loopCore_isr
 * @param
 * @return
 * @additional 主控处理函数
 *
 *************************************************/
void gimbal_loopCore_isr(TIM_ID timeId, u32 timHz)
{

}

/************************ (C) COPYRIGHT FORWARDX *****END OF FILE****/
