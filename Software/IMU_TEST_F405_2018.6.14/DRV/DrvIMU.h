/*
******************************************************************************
*@文件名字  : DrvImu.h
*@描述	    : IMU
******************************************************************************
******************************************************************************
*/

#ifndef _DRV_IMU_H_
#define _DRV_IMU_H_

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stmHead.h"
     
typedef struct 
{
	float accX;
	float accY;
	float accZ;
	float gyroX;
	float gyroY;
	float gyroZ;
  float temp;
}IMUData_STRUCT;

typedef enum
{
  IMU_MPU6500_Camera,
  IMU_MPU6500_Carrier,
  IMU_NUM, 
}IMU_PORT;


extern void DrvImu_Init(void);
extern void DrvImu_RegisterRecvDataHandler(IMU_PORT port, void *pFun);

extern void DrvImu_Read(IMU_PORT port, u32 loopHz);

extern void DrvImu_WriteReg(IMU_PORT port, u8 Reg, u8 Data);
extern void DrvImu_ReadReg(IMU_PORT port, u8 Reg, u16 Len);



 
#ifdef __cplusplus
}
#endif

#endif


/*********END OF FILE****/

 
