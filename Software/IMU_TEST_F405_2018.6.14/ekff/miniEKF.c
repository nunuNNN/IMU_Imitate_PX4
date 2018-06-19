/*
Copyright (c) 2018-? zxl
*/


#include "miniEKF.h"


#define _PQ_INITIAL 0.001f
#define _QQ_INITIAL 0.05f

#define _PWB_INITIAL 0.001f
#define _QWB_INITIAL 0.0000005f

#define _RA_INITIAL  0.005346f


/******************************************************************************
* @函 数 名： miniEKF_Init
* @函数描述： EKF初始化
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/
void miniEKF_Init(miniEKF_Type *pEKF)
{
	if(pEKF == NULL )
	{
		return;
	}
	
	memset(pEKF, 0, sizeof(miniEKF_Type));
		
	pEKF->_F[0] = 1.0f;
	pEKF->_F[8] = 1.0f;
	pEKF->_F[16] = 1.0f;
	pEKF->_F[24] = 1.0f;
	pEKF->_F[32] = 1.0f;
	pEKF->_F[40] = 1.0f;
	pEKF->_F[48] = 1.0f;
	
	pEKF->_I[0] = 1.0f;
	pEKF->_I[8] = 1.0f;
	pEKF->_I[16] = 1.0f;
	pEKF->_I[24] = 1.0f;
	pEKF->_I[32] = 1.0f;
	pEKF->_I[40] = 1.0f;
	pEKF->_I[48] = 1.0f;	
	
	pEKF->_P[0] = _PQ_INITIAL;
	pEKF->_P[8] = _PQ_INITIAL;
	pEKF->_P[16] = _PQ_INITIAL;
	pEKF->_P[24] = _PQ_INITIAL;
	pEKF->_P[32] = _PWB_INITIAL;
	pEKF->_P[40] = _PWB_INITIAL;
	pEKF->_P[48] = _PWB_INITIAL;	
	
	pEKF->_Q[0] = _QQ_INITIAL;
	pEKF->_Q[8] = _QQ_INITIAL;
	pEKF->_Q[16] = _QQ_INITIAL;
	pEKF->_Q[24] = _QQ_INITIAL;
	pEKF->_Q[32] = _QWB_INITIAL;
	pEKF->_Q[40] = _QWB_INITIAL;
	pEKF->_Q[48] = _QWB_INITIAL;	
	
	pEKF->_R[0] = _RA_INITIAL;
	pEKF->_R[4] = _RA_INITIAL;
	pEKF->_R[8] = _RA_INITIAL;	

	pEKF->ekfInitFlag = 1;
}

/******************************************************************************
* @函 数 名： miniEKF_AngleInit
* @函数描述： EKF获取初始角度
* @参    数： 
* @返 回 值： 
* @备    注：imu 参数中acc单位g/s^2  gyro单位rad/s
******************************************************************************/
static void miniEKF_AngleInit(miniEKF_Type *pEKF, IMU_Type imu)
{
	float accNED[3] = {0, 0 , -1.0f};
	float accelVector[3] = {0, 0, 0};
	float crossVector[3];
	float norm;
	float cosw, sinhalfw, coshalfw;
	
	if( (pEKF == NULL) || (pEKF->ekfInitFlag == 0) )
	{
		return;
	}

	//归一化
	norm = FastSqrtI(_SQUARE(imu.accN) + _SQUARE(imu.accE) + _SQUARE(imu.accD));
	accelVector[0] = imu.accN * norm;
	accelVector[1] = imu.accE * norm;
	accelVector[2] = imu.accD * norm;
	
	//cross product between accel and reference
	crossVector[0] = accelVector[1] * accNED[2] - accelVector[2] * accNED[1];
	crossVector[1] = accelVector[2] * accNED[0] - accelVector[0] * accNED[2];
	crossVector[2] = accelVector[0] * accNED[1] - accelVector[1] * accNED[0];
	norm = FastSqrtI(_SQUARE(crossVector[0]) + _SQUARE(crossVector[1]) + _SQUARE(crossVector[2]));
	crossVector[0] *= norm;
	crossVector[1] *= norm;
	crossVector[2] *= norm;

	//the angle between accel and reference is the dot product of the two vectors
	cosw = accelVector[0] * accNED[0] + accelVector[1] * accNED[1] + accelVector[2] * accNED[2];
	coshalfw = FastSqrt((1.0f + cosw) * 0.5f);
	sinhalfw = FastSqrt((1.0f - cosw) * 0.5f);

	pEKF->xEKF[0] = coshalfw;
	pEKF->xEKF[1] = crossVector[0] * sinhalfw;
	pEKF->xEKF[2] = crossVector[1] * sinhalfw;
	pEKF->xEKF[3] = crossVector[2] * sinhalfw;

	norm = FastSqrtI(_SQUARE(pEKF->xEKF[0]) + _SQUARE(pEKF->xEKF[1]) + _SQUARE(pEKF->xEKF[2]) + _SQUARE(pEKF->xEKF[3]));
	pEKF->xEKF[0] *= norm;
	pEKF->xEKF[1] *= norm;
	pEKF->xEKF[2] *= norm;
	pEKF->xEKF[3] *= norm;	

	pEKF->angleInitFlag = 1;	
}

/******************************************************************************
* @函 数 名： miniEKF_Update
* @函数描述： EKF 主函数
* @参    数： 
* @返 回 值： 
* @备    注：imu 参数中acc单位g/s^2  gyro单位rad/s
******************************************************************************/
void miniEKF_Update(miniEKF_Type *pEKF, IMU_Type imu, float dt)
{
	float norm;
	Quaternion_Type quate;
  Euler_Type halfTAngle;
	Quaternion_Type halfTQuate;
	Quaternion_Type quate2;
	float halfT = 0.5f*dt;
	float accelVector[3] = {0, 0, 0};
	
	if( (pEKF == NULL) || (pEKF->angleInitFlag == 0) )
	{
		//获得初始角度
		miniEKF_AngleInit(pEKF, imu);
		return;
	}
	
	halfTAngle.roll  = halfT*(imu.gyroN - pEKF->xEKF[4]);//横滚
	halfTAngle.pitch = halfT*(imu.gyroE - pEKF->xEKF[5]);//俯仰
	halfTAngle.yaw   = halfT*(imu.gyroD - pEKF->xEKF[6]);//航向 	
	
	//1. tate prediction	
  quate._A = pEKF->xEKF[0];
  quate._B = pEKF->xEKF[1];
  quate._C = pEKF->xEKF[2];
  quate._D = pEKF->xEKF[3];
	
	pEKF->xEKF[0] = quate._A + (-quate._B * halfTAngle.roll - quate._C * halfTAngle.pitch - quate._D * halfTAngle.yaw);
	pEKF->xEKF[1] = quate._B + ( quate._A * halfTAngle.roll - quate._D * halfTAngle.pitch + quate._C * halfTAngle.yaw);
	pEKF->xEKF[2] = quate._C + ( quate._D * halfTAngle.roll + quate._A * halfTAngle.pitch - quate._B * halfTAngle.yaw);
	pEKF->xEKF[3] = quate._D + (-quate._C * halfTAngle.roll + quate._B * halfTAngle.pitch + quate._A * halfTAngle.yaw);
	norm = FastSqrtI(_SQUARE(pEKF->xEKF[0]) + _SQUARE(pEKF->xEKF[1]) + _SQUARE(pEKF->xEKF[2]) + _SQUARE(pEKF->xEKF[3]));
	pEKF->xEKF[0] *= norm;
	pEKF->xEKF[1] *= norm;
	pEKF->xEKF[2] *= norm;
	pEKF->xEKF[3] *= norm;	
	pEKF->quate._A = pEKF->xEKF[0];
	pEKF->quate._B = pEKF->xEKF[1];
	pEKF->quate._C = pEKF->xEKF[2];
	pEKF->quate._D = pEKF->xEKF[3];		
	Quaterion_ToCosMatrix(&pEKF->quate, &pEKF->Cbn);	
  	
	//2.求状态转移方程的雅克比矩阵F
  halfTQuate._A = halfT*quate._A;
  halfTQuate._B = halfT*quate._B;
  halfTQuate._C = halfT*quate._C;
  halfTQuate._D = halfT*quate._D;

	//第1行
	pEKF->_F[1] = -halfTAngle.roll;
	pEKF->_F[2] = -halfTAngle.pitch;
	pEKF->_F[3] = -halfTAngle.yaw;
	pEKF->_F[4] = halfTQuate._B;
	pEKF->_F[5] = halfTQuate._C;
	pEKF->_F[6] = halfTQuate._D;
	
	//第2行
	pEKF->_F[7] = halfTAngle.roll;
	pEKF->_F[9] = halfTAngle.yaw;
	pEKF->_F[10] = -halfTAngle.pitch;
	pEKF->_F[11] = -halfTQuate._A;
	pEKF->_F[12] = halfTQuate._D;
	pEKF->_F[13] = -halfTQuate._C;	
		
	//第3行
	pEKF->_F[14] = halfTAngle.pitch;
	pEKF->_F[15] = -halfTAngle.yaw;
	pEKF->_F[17] = halfTAngle.roll;
	pEKF->_F[18] = -halfTQuate._D;
	pEKF->_F[19] = -halfTQuate._A;
	pEKF->_F[20] = halfTQuate._B;	
	
	//第4行
	pEKF->_F[21] = halfTAngle.yaw;
	pEKF->_F[22] = halfTAngle.pitch;
	pEKF->_F[23] = -halfTAngle.roll;
	pEKF->_F[25] = halfTQuate._C;
	pEKF->_F[26] = -halfTQuate._B;
	pEKF->_F[27] = -halfTQuate._A;	
	
	//3.计算协方差矩阵	P = F*P*F' + Q;
	Matrix_Multiply(pEKF->_F, 7, 7, pEKF->_P, 7, pEKF->_PX);
	Matrix_Multiply_With_Transpose(pEKF->_PX, 7, 7, pEKF->_F, 7, pEKF->_P);
	Maxtrix_Add(pEKF->_P, 7, 7, pEKF->_Q, pEKF->_P);
	
	//4.观测方程的雅可比矩阵H
	quate2._A = 2.0f*pEKF->xEKF[0];
	quate2._B = 2.0f*pEKF->xEKF[1];
	quate2._C = 2.0f*pEKF->xEKF[2];
  quate2._D = 2.0f*pEKF->xEKF[3];
		
	//第1行
	pEKF->_H[0] = quate2._C;
	pEKF->_H[1] = -quate2._D;
	pEKF->_H[2] = quate2._A;
	pEKF->_H[3] = -quate2._B;
	
	//第2行
	pEKF->_H[7] = -quate2._B;
	pEKF->_H[8] = -quate2._A;
	pEKF->_H[9] = -quate2._D;
	pEKF->_H[10] = -quate2._C;
	
	//第3行
	pEKF->_H[14] = -quate2._A;
	pEKF->_H[15] = quate2._B;
	pEKF->_H[16] = quate2._C;
	pEKF->_H[17] = -quate2._D;
	
	//5.更新增益K值K = P * H' / (R + H * P * H')
	Matrix_Multiply_With_Transpose(pEKF->_P, 7, 7, pEKF->_H, 3, pEKF->_PXY);
	Matrix_Multiply(pEKF->_H, 3, 7, pEKF->_PXY, 3, pEKF->_S);
	Maxtrix_Add(pEKF->_S, 3, 3, pEKF->_R, pEKF->_S);
	Matrix_Inverse(pEKF->_S, 3, pEKF->_SI);
	Matrix_Multiply(pEKF->_PXY, 7, 3, pEKF->_SI, 3, pEKF->_K);
	
	//6.根据加速度测量模型计算观测量 h(k)
	pEKF->_Y[0] = -2.0f * (pEKF->xEKF[1] * pEKF->xEKF[3] - pEKF->xEKF[0] * pEKF->xEKF[2]);
	pEKF->_Y[1] = -2.0f * (pEKF->xEKF[2] * pEKF->xEKF[3] + pEKF->xEKF[0] * pEKF->xEKF[1]);
	pEKF->_Y[2] = 1.0f - 2.0f * (pEKF->xEKF[0] * pEKF->xEKF[0] + pEKF->xEKF[3] * pEKF->xEKF[3]);

	//归一化 accel
	norm = FastSqrtI(_SQUARE(imu.accN) + _SQUARE(imu.accE) + _SQUARE(imu.accD));
	accelVector[0] = imu.accN * norm;
	accelVector[1] = imu.accE * norm;
	accelVector[2] = imu.accD * norm;

	//观测模型误差 Zk - h(k)
	pEKF->_Y[0] = accelVector[0] - pEKF->_Y[0];
	pEKF->_Y[1] = accelVector[1] - pEKF->_Y[1];
	pEKF->_Y[2] = accelVector[2] - pEKF->_Y[2];
	
	// Update State Vector 	X = X + K * Y;
	Matrix_Multiply(pEKF->_K, 7, 3, pEKF->_Y, 1, pEKF->_KY);
	Maxtrix_Add(pEKF->xEKF, 7, 1, pEKF->_KY, pEKF->xEKF);

	//normalize quaternion
	norm = FastSqrtI(_SQUARE(pEKF->xEKF[0]) + _SQUARE(pEKF->xEKF[1]) + _SQUARE(pEKF->xEKF[2]) + _SQUARE(pEKF->xEKF[3]));
	pEKF->xEKF[0] *= norm;
	pEKF->xEKF[1] *= norm;
	pEKF->xEKF[2] *= norm;
	pEKF->xEKF[3] *= norm;	
	pEKF->quate._A = pEKF->xEKF[0];
	pEKF->quate._B = pEKF->xEKF[1];
	pEKF->quate._C = pEKF->xEKF[2];
	pEKF->quate._D = pEKF->xEKF[3];		
	Quaterion_ToCosMatrix(&pEKF->quate, &pEKF->Cbn);
	
	//covariance measurement update
	//P = (I - K * H) * P
	//P = P - K * H * P
	Matrix_Multiply(pEKF->_K, 7, 3, pEKF->_H, 7, pEKF->_PX);
	Matrix_Multiply(pEKF->_PX, 7, 7, pEKF->_P, 7, pEKF->_PXX);
	Maxtrix_Add(pEKF->_P, 7, 7, pEKF->_PXX, pEKF->_P);
	
	//计算角度
	Quaternion_ToEuler(&pEKF->quate, &pEKF->angle);
	
	pEKF->angle.pitch = RAD_TO_DEG(pEKF->angle.pitch);
	pEKF->angle.roll = RAD_TO_DEG(pEKF->angle.roll);
	pEKF->angle.yaw = RAD_TO_DEG(pEKF->angle.yaw);	
}


/************************ (C) COPYRIGHT FORWARDX *****END OF FILE****/
