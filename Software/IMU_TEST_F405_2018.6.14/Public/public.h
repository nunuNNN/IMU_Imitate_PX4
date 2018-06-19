
#ifndef _PUBLIC_H_
#define _PUBLIC_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "miniMatrix.h"
	
#define HALF_PI       1.5707963267948966192313216916398f
#define _PI           3.1415926535897932384626433832795f
#define TWO_PI        6.283185307179586476925286766559f

#define RAD_TO_DEG(x)  ((x) * 57.2957796f)
#define DEG_RAD_TO(x)  ((x) * 0.01745329251994f)

#define _SQUARE(x)     (((float)x)*((float)x))
	
#define _G0 	         (9.80f)   //9.81f       //重力加速度
#define _RAD           (0.01745329251994f)     //度转弧度的因子 pi/180
	
typedef struct 
{
  float _A;
  float _B;
  float _C;
  float _D;
}Quaternion_Type;

typedef struct
{
  float _A11;
  float _A12;
  float _A13;
  float _A21;
  float _A22;
  float _A23;
  float _A31;
  float _A32;
  float _A33; 
}Matrix3x3_Type;

typedef struct 
{
  float pitch;
  float roll;
  float yaw;
}Euler_Type;

extern float arctan2(float y,float x);
extern unsigned char GetCheckSum8(unsigned char *buf, unsigned int len);

extern void Data2Buf(char *pBuf, void *pData, unsigned short dataType);
extern void Buf2Data(void *pData, char *pBuf, unsigned short dataType);

extern void buf2float(float *tfloat, char *buf);
extern void buf2int(int *tint, char *buf);
extern void float2buf(char *buf, float *tfloat);
extern void int2buf(char *buf, int *tint);
extern unsigned char bufferCompare(unsigned char* pBuffer1, unsigned char* pBuffer2, unsigned int len);

extern float FastSqrtI(float x);
extern float FastSqrt(float x);

void Quaterion_ToCosMatrix(Quaternion_Type* pQData, Matrix3x3_Type* pCosineM);
void Euler_ToQuate(Euler_Type* pRadAngle, Quaternion_Type* pQData);
void Quaternion_ToEuler( Quaternion_Type* pQData, Euler_Type* pRadAngle);

#ifdef __cplusplus
}
#endif	

#endif /* _PUBLIC_H_ */
