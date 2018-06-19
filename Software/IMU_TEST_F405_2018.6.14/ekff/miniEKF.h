/*
Copyright (c) 2018-? zxl
*/

#ifndef _MINI_EKF_H_
#define _MINI_EKF_H_


#ifdef __cplusplus
extern "C" {
#endif

	
#include "public.h"	



typedef struct
{
	//北东地坐标系
  float accN;   //g/s^2
  float accE;
  float accD;	
  float gyroN;  //rad/s
  float gyroE;
  float gyroD;
}IMU_Type;


typedef struct
{	
	float xEKF[7]; //状态量
	float _Y[3]; //观测量
	
	float _F[49];
	float _P[49];
	float _I[49];	
	
  float _H[21]; 	
  float _K[21];

  float _Q[49];
  float _R[9];
	
	float _KY[7];
	float _PX[49];
	float _PXX[49];
	float _PXY[21];
	float _S[9];
	float _SI[9];	

	unsigned char ekfInitFlag;
	unsigned char angleInitFlag;

	IMU_Type imu;
	Quaternion_Type quate;
	Matrix3x3_Type Cbn;	
	Euler_Type angle;	
}miniEKF_Type;

extern void miniEKF_Init(miniEKF_Type *pEKF);
extern void miniEKF_Update(miniEKF_Type *pEKF, IMU_Type imu, float dt);
	

#ifdef __cplusplus
}
#endif	

#endif
