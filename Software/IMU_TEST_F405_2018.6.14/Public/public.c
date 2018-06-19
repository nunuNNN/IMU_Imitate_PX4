
#include "public.h"

/****************************************************************
* @brief : arctan2
* @input :
* @output:
* @return: None
*****************************************************************/
float arctan2(float y,float x)
{
	float psi;

	if (x == 0)
	{
		psi = HALF_PI;
		if (y < 0)
		{
			psi = -psi;
		}
		else if (y == 0)
		{
			psi = 0;
		}
	}
	else
	{
		psi = atan(y/x);
		if (x < 0)
		{
			if (y < 0)
			{
				psi = psi - _PI;
			}
			else
			{
				psi = psi + _PI;
			}
		}
	}

	return psi;
}

void buf2float(float *tfloat, char *buf)
{
    int i;
    char *p1 = (char *)tfloat;
    char *p3 = buf;

    for (i = 0; i < 4; i++)
    {
        *p1 = *p3;
        p1++;
        p3++;
    }
}


void buf2int(int *tint, char *buf)
{
    int i;
    char *p1 = (char *)tint;
    char *p3 = buf;

    for (i = 0; i < 4; i++)
    {
        *p1 = *p3;
        p1++;
        p3++;
    }
}

void float2buf(char *buf, float *tfloat)
{
    int i;
    char *p1 = (char *)tfloat;
    char *p3 = buf;

    for (i = 0; i < 4; i++)
    {
        *p3 = *p1;
        p1++;
        p3++;
    }
}

void int2buf(char *buf, int *tint)
{
    int i;
    char *p1 = (char *)tint;
    char *p3 = buf;

    for (i = 0; i < 4; i++)
    {
        *p3 = *p1;
        p1++;
        p3++;
    }
}


void Data2Buf(char *pBuf, void *pData, unsigned short dataType)
{
    unsigned short i;
    long *inp = (long *) pData;
    char *outp = (char *) pBuf;

    for (i = 0; i < dataType; i++)
    {
        *(outp++) = (*inp >> (i * 8)) & 0x00FF;
    }
}


void Buf2Data(void *pData, char *pBuf, unsigned short dataType)
{
    unsigned short i;
    char * pByte = (char *)pData;

    for (i = 0; i < dataType; i++)
    {
        *(pByte++) = pBuf[i];
    }
}

unsigned char GetCheckSum8(unsigned char *buf, unsigned int len)
{
    unsigned char checksum = 0;
    unsigned int i = 0;

    for (i = 0; i < len; i++)
    {
        checksum += buf[i];
    }

    return checksum & 0xFF;
}


unsigned char bufferCompare(unsigned char* pBuffer1, unsigned char* pBuffer2, unsigned int len)
{
    while (len--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return 0;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return 1;
}

// Quake inverse square root
float FastSqrtI(float x)
{
	//////////////////////////////////////////////////////////////////////////
	//less accuracy, more faster
	/*
	L2F l2f;
	float xhalf = 0.5f * x;
	l2f.f = x;

	l2f.i = 0x5f3759df - (l2f.i >> 1);
	x = l2f.f * (1.5f - xhalf * l2f.f * l2f.f);
	return x;
	*/
	//////////////////////////////////////////////////////////////////////////
	union { unsigned int i; float f;} l2f;
	l2f.f = x;
	l2f.i = 0x5F1F1412 - (l2f.i >> 1);
	return l2f.f * (1.69000231f - 0.714158168f * x * l2f.f * l2f.f);
}

float FastSqrt(float x)
{
	return x * FastSqrtI(x);
}



/************************************************************************************
* @brief : 根据四元数计算方向余弦矩阵
* @input : pQData 四元数
* @output: pCosineM 方向余弦矩阵
* @return: None
************************************************************************************/
void Quaterion_ToCosMatrix(Quaternion_Type* pQData, Matrix3x3_Type* pCosineM)
{
	pCosineM->_A11 = 1 - 2 * (pQData->_C * pQData->_C + pQData->_D * pQData->_D);
	pCosineM->_A12 = 2 * (pQData->_B * pQData->_C - pQData->_A * pQData->_D);
	pCosineM->_A13 = 2 * (pQData->_B * pQData->_D + pQData->_A * pQData->_C);
	
	pCosineM->_A21 = 2 * (pQData->_B * pQData->_C + pQData->_A * pQData->_D);
	pCosineM->_A22 = 1 - 2 * (pQData->_B * pQData->_B + pQData->_D * pQData->_D);
	pCosineM->_A23 = 2 * (pQData->_C * pQData->_D - pQData->_A * pQData->_B);
	
	pCosineM->_A31 = 2 * (pQData->_B * pQData->_D - pQData->_A * pQData->_C);
	pCosineM->_A32 = 2 * (pQData->_C * pQData->_D + pQData->_A * pQData->_B);
	pCosineM->_A33 = 1 - 2 * (pQData->_B * pQData->_B + pQData->_C * pQData->_C);	
}


/************************************************************************************
* @brief : 欧拉角(弧度)转换成四元数 R->P->Y顺序
* @input : pRadAngle 欧拉角(弧度)
* @output: pQData 四元数
* @return: None
************************************************************************************/
//R->P->Y顺序
void Euler_ToQuate(Euler_Type* pRadAngle, Quaternion_Type* pQData)
{
	//R->P->Y
	float halfP = pRadAngle->pitch/2.0f; //弧度
	float halfR = pRadAngle->roll/2.0f;
	float halfY = pRadAngle->yaw/2.0f;
	
	float sinP = sinf(halfP);
	float cosP = cosf(halfP);
	float sinR = sinf(halfR);
	float cosR = cosf(halfR);
	float sinY = sinf(halfY);
	float cosY = cosf(halfY);
	
	pQData->_A = cosY*cosR*cosP + sinY*sinR*sinP;
	pQData->_B = cosY*sinR*cosP - sinY*cosR*sinP;
	pQData->_C = cosY*cosR*sinP + sinY*sinR*cosP;
	pQData->_D = sinY*cosR*cosP - cosY*sinR*sinP;	
}


/************************************************************************************
* @brief : 四元数转换成欧拉角(弧度)
* @input : pQData 四元数
* @output: pRadAngle 欧拉角(弧度)
* @return: None   x-> roll y-> pitch z-> yaw
[ cos(y)*cos(z), cos(z)*sin(x)*sin(y) - cos(x)*sin(z), sin(x)*sin(z) + cos(x)*cos(z)*sin(y)]
[ cos(y)*sin(z), cos(x)*cos(z) + sin(x)*sin(y)*sin(z), cos(x)*sin(y)*sin(z) - cos(z)*sin(x)]
[       -sin(y),                        cos(y)*sin(x),                        cos(x)*cos(y)]
************************************************************************************/
void Quaternion_ToEuler( Quaternion_Type* pQData, Euler_Type* pRadAngle)
{
	//R->P->Y
  float temp = 0;	
	Matrix3x3_Type _OUT;
	
	Quaterion_ToCosMatrix(pQData, &_OUT);
  
	temp = FastSqrt(fabs(1 - _OUT._A31 * _OUT._A31));

	if (temp < 0.000001f)
	{
		if (_OUT._A31 < 0)
		{
			pRadAngle->pitch = HALF_PI;
		}
		else if (_OUT._A31 > 0)
		{
			pRadAngle->pitch = -HALF_PI;
		}
		else
		{
			pRadAngle->pitch = 0;
		}
	}
	else
	{
		pRadAngle->pitch = atan(-_OUT._A31/temp);
	}

	pRadAngle->roll = arctan2(_OUT._A32, _OUT._A33);
	pRadAngle->yaw  = arctan2(_OUT._A21, _OUT._A11);

}

