/*
******************************************************************************
*@文件名字  : DrvMPU6500.h
*@描述	    : MPU6500
******************************************************************************
******************************************************************************
*/

#ifndef _DRV_MPU6500_H_
#define _DRV_MPU6500_H_

#ifdef __cplusplus
 extern "C" {
#endif 

#include "DrvIMU.h"	 
#include "BspSpi.h"

//相关寄存器
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：
#define	IMU_CONFIG		0x1A	//低通滤波频率，典型值：
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：
#define	ACCEL_CONFIG1	0x1D	//加速计自检、测量范围及高通滤波频率，典型值:

#define	ACCEL_XOUT_H	0x3B  //加速度计 x轴加速度 高8位
#define	ACCEL_XOUT_L	0x3C	//加速度计 x轴加速度 低8位
#define	ACCEL_YOUT_H	0x3D	//加速度计 y轴加速度 高8位
#define	ACCEL_YOUT_L	0x3E	//加速度计 y轴加速度 低8位
#define	ACCEL_ZOUT_H	0x3F	//加速度计 z轴加速度 高8位
#define	ACCEL_ZOUT_L	0x40	//加速度计 z轴加速度 低8位
#define	TEMP_OUT_H		0x41	//温度  高8位
#define	TEMP_OUT_L		0x42	//温度  低8位
#define	GYRO_XOUT_H		0x43	//陀螺仪   x轴陀螺仪 高8位
#define	GYRO_XOUT_L		0x44	//陀螺仪   x轴陀螺仪 低8位
#define	GYRO_YOUT_H		0x45	//陀螺仪   y轴陀螺仪 高8位
#define	GYRO_YOUT_L		0x46	//陀螺仪   y轴陀螺仪 低8位
#define	GYRO_ZOUT_H		0x47	//陀螺仪   z轴陀螺仪 高8位
#define	GYRO_ZOUT_L		0x48	//陀螺仪   z轴陀螺仪 低8位

#define	WHO_AM_I		  0x75	//IIC地址寄存器(默认数值0x70，只读)
#define USER_CONTROL  0x6A  //0x10 I2C_IF_DIS  ENABLE
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用) 

typedef enum
{
	State_idle = 0,
	State_Config_Step1,
	State_Config_Step2,
	State_Config_Step3,
	State_Config_Step4,
	State_Config_Step5,
	State_Config_Step6,
	State_Read_Config,
	State_Operate,

}MPU6500State_Enum;



typedef struct 
{
  SPI_STRUCT SPIx;
  MPU6500State_Enum state;
	IMUData_STRUCT data;
  void (*pMPU6500RecvDataProc)(IMUData_STRUCT* pData);
  unsigned long rxCnt;
  unsigned long rxReg28ErrCnt;
  unsigned long rxRegErrCnt;
  unsigned long txCnt;
  unsigned long txErrCnt;
}MPU6500_STRUCT;


extern void DrvMPU6500_Init(MPU6500_STRUCT* pMPU6500);
extern void DrvMPU6500_Read(MPU6500_STRUCT* pMPU6500, u32 loopHz);

extern void DrvMPU6500_WriteReg(MPU6500_STRUCT* pMPU6500, u8 Reg, u8 Data);
extern void DrvMPU6500_ReadReg(MPU6500_STRUCT* pMPU6500, u8 Reg, u16 Len);

extern void DrvMPU6500_CS_HIGH(MPU6500_STRUCT* pMPU6500);
extern void DrvMPU6500_CS_LOW(MPU6500_STRUCT* pMPU6500);

extern void DrvMPU6500_spiMasterRxCpltCallback(MPU6500_STRUCT* pMPU6500, u8* data, u16 Len);
 
#ifdef __cplusplus
}
#endif

#endif


/*********END OF FILE****/

 
