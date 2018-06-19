/**
  ******************************************************************************
  * @file    AppImu.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  ******************************************************************************
  */


#ifndef __APP_IMU_H
#define __APP_IMU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DrvIMU.h"
#include "miniEKF.h"
	
typedef struct
{
	IMU_Type imuSum;
	u32 cnt; 
	
}IMUBias_Type;
	
extern IMUData_STRUCT IMU_Data;
extern IMU_Type imu_bias;
extern IMU_Type imu_raw;
extern miniEKF_Type miniEKF;

extern void AppIMU_Init(void);
//u8 AppIMU_CalibrateGyro(IMU_Type *pImu, IMU_Type *pBias, u32 loopHz);
u8 AppIMU_CalibrateIMU(IMU_Type *pImu, IMU_Type *pBias, u32 loopHz);

#ifdef __cplusplus
}
#endif

#endif /* __APP_IMU_H */

/************************ (C) COPYRIGHT ZEROTECH *****END OF FILE****/
