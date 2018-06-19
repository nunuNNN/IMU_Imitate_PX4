/**
  ******************************************************************************
  * @file    AppIMU.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "AppIMU.h"


IMUData_STRUCT IMU_Data;
IMU_Type imu_bias;
IMU_Type imu_raw;

u32 imuAccBiasCnt = 0;
u32 imuGyroBiasCnt = 0;
IMU_Type sumIMU;

miniEKF_Type miniEKF;

/*************************************************
 * @Function 获取IMU raw data
 * @param
 * @return
 * @additional 
 *************************************************/
void IMU_Camera_DataHandle(IMUData_STRUCT* pImu)
{
  //TODO:
	IMU_Data.accX = pImu->accX;
	IMU_Data.accY = pImu->accY;
	IMU_Data.accZ = pImu->accZ;

	IMU_Data.gyroX = pImu->gyroX;
	IMU_Data.gyroY = pImu->gyroY;
	IMU_Data.gyroZ = pImu->gyroZ;	
	
	IMU_Data.temp = pImu->temp;
	
	//TODO：
	imu_raw.accN = IMU_Data.accY;
	imu_raw.accE = -IMU_Data.accX;
	imu_raw.accD = IMU_Data.accZ;

	imu_raw.gyroN = IMU_Data.gyroY;
	imu_raw.gyroE = -IMU_Data.gyroX;
	imu_raw.gyroD = IMU_Data.gyroZ;	
	
	miniEKF.imu.accN = (imu_raw.accN - imu_bias.accN)*_G0;
	miniEKF.imu.accE = (imu_raw.accE - imu_bias.accE)*_G0;
	miniEKF.imu.accD = (imu_raw.accD - imu_bias.accD)*_G0;

	miniEKF.imu.gyroN = (imu_raw.gyroN - imu_bias.gyroN)*_RAD;
	miniEKF.imu.gyroE = (imu_raw.gyroE - imu_bias.gyroE)*_RAD;
	miniEKF.imu.gyroD = (imu_raw.gyroD - imu_bias.gyroD)*_RAD;	
}

/*************************************************
 * @Function 初始化IMU
 * @param
 * @return
 * @additional 
 *************************************************/
void AppIMU_Init(void)
{
		miniEKF_Init(&miniEKF);
	
    DrvImu_Init();
    DrvImu_RegisterRecvDataHandler(IMU_MPU6500_Camera, &IMU_Camera_DataHandle);
}

/*************************************************
 * @Function 校准初始化
 * @param
 * @return
 * @additional 
 *************************************************/
void AppIMU_CalibrateInit(void)
{
	imuAccBiasCnt = 0;
	imuGyroBiasCnt = 0;
	
	sumIMU.accN = 0;
	sumIMU.accE = 0;
	sumIMU.accD = 0;

	sumIMU.gyroN = 0;
	sumIMU.gyroE = 0;
	sumIMU.gyroD = 0;	
	
}

/*************************************************
 * @Function 校准IMU ACC
 * @param
 * @return
 * @additional 
 *************************************************/
u8 AppIMU_CalibrateIMU(IMU_Type *pImu, IMU_Type *pBias, u32 loopHz)
{
#define _BIAS_TIME  (loopHz)
	
	u8 result = 0;
	
	imuAccBiasCnt++;

	sumIMU.accN += pImu->accN;
	sumIMU.accE += pImu->accE;
	sumIMU.accD += pImu->accD;
	
	sumIMU.gyroN += pImu->gyroN;
	sumIMU.gyroE += pImu->gyroE;
	sumIMU.gyroD += pImu->gyroD;		
		
	if (imuAccBiasCnt >= _BIAS_TIME)
	{
		pBias->accN = sumIMU.accN/_BIAS_TIME;
		pBias->accE = sumIMU.accE/_BIAS_TIME;
		pBias->accD = sumIMU.accD/_BIAS_TIME + 1.0f;
		
		
		pBias->gyroN = sumIMU.gyroN/_BIAS_TIME;
		pBias->gyroE = sumIMU.gyroE/_BIAS_TIME;
		pBias->gyroD = sumIMU.gyroD/_BIAS_TIME;
		
		imuAccBiasCnt = 0;
		sumIMU.accN = 0;
		sumIMU.accE = 0;
		sumIMU.accD = 0;
		
		sumIMU.gyroN = 0;
		sumIMU.gyroE = 0;
		sumIMU.gyroD = 0;				
	
		result = 1;

   }
	
	 return(result);	
}

///*************************************************
// * @Function 校准IMU GYRO
// * @param
// * @return
// * @additional 
// *************************************************/
//u8 AppIMU_CalibrateGyro(IMU_Type *pImu, IMU_Type *pBias, u32 loopHz)
//{
//#define GYRO_BIAS_TIME  (loopHz)
//	
//	u8 result = 0;
//	
//	imuGyroBiasCnt++;
//	
//	sumIMU.gyroN += pImu->gyroN;
//	sumIMU.gyroE += pImu->gyroE;
//	sumIMU.gyroD += pImu->gyroD;	
//	
//	if (imuGyroBiasCnt >= GYRO_BIAS_TIME)
//	{
//		pBias->gyroN = sumIMU.gyroN/GYRO_BIAS_TIME;
//		pBias->gyroE = sumIMU.gyroE/GYRO_BIAS_TIME;
//		pBias->gyroD = sumIMU.gyroD/GYRO_BIAS_TIME;
//		
//		imuGyroBiasCnt = 0;

//		sumIMU.gyroN = 0;
//		sumIMU.gyroE = 0;
//		sumIMU.gyroD = 0;			
//		result = 1;

//   }
//	
//	 return(result);		
//}

/************************ (C) COPYRIGHT FORWARDX *****END OF FILE****/
