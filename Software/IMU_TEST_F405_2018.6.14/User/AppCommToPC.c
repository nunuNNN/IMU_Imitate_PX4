/**
  ******************************************************************************
  * @file    AppComm.c
  * @brief   通信处理
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "AppComm.h"
#include "public.h"
#include "AppIMU.h"

#define SHOW_DATA_LEN  (12)

float showData[SHOW_DATA_LEN] = {0};



/************************************************************************************
* @brief : 串口接收数据处理
* @input :
* @output:
* @return: None
************************************************************************************/
void AppCom_recvComm2PCDataProc(u8* buf, u16 len)
{
 
}


/************************************************************************************
* @brief : 显示调试数据
* @input :
* @output:
* @return: None
************************************************************************************/
void AppShowDebugData(void)
{
    int indx, showDataMsgLen;

    memset(packageTxMsgBuf, 0, 256);

		showData[0] = IMU_Data.accX;
		showData[1] = IMU_Data.accY;
		showData[2] = IMU_Data.accZ;

		showData[3] = IMU_Data.gyroX;
		showData[4] = IMU_Data.gyroY;
		showData[5] = IMU_Data.gyroZ;

		showData[6] = IMU_Data.temp;
		showData[7] = 0;
		showData[8] = 0;
	
		showData[9] = miniEKF.angle.pitch;
		showData[10] = miniEKF.angle.roll;
		showData[11] = miniEKF.angle.yaw;
	
		showData[12] = miniEKF.imu.gyroN;
		showData[13] = miniEKF.imu.gyroE;
		showData[14] = miniEKF.imu.gyroD;

    for ( indx = 0; indx < SHOW_DATA_LEN; indx++ )
    {
        Data2Buf((char*)&packageTxMsgBuf[4 * indx + 4],  &showData[indx], 4);
    }

    showDataMsgLen = 4 * SHOW_DATA_LEN + 4 + 1;

    packageTxMsgBuf[0] = 0x0A;
    packageTxMsgBuf[1] = showDataMsgLen;
    packageTxMsgBuf[2] = 0xBC;
    packageTxMsgBuf[3] = Register_Show_Data;

    packageTxMsgBuf[showDataMsgLen - 1] = GetCheckSum8(packageTxMsgBuf, showDataMsgLen - 1);

    AppComm_SendDebugData(packageTxMsgBuf, showDataMsgLen);
}

/************************************************************************************
* @brief : 发送数据到匿名四轴上位机
* @input :
* @output:
* @return: None
************************************************************************************/
void MY_float2buf(u8 *buf, float data)
{
	unsigned char *p;
	
	p =(unsigned char *)&data;
	
	buf[0]=(unsigned char)(*(p+3));
	buf[1]=(unsigned char)(*(p+2));
	buf[2]=(unsigned char)(*(p+1));
	buf[3]=(unsigned char)(*(p+0));	
}


void AppShowData(void)
{
    int indx, showDataMsgLen;

    memset(packageTxMsgBuf, 0, 256);



		
		showData[0] = miniEKF.angle.pitch;
		showData[1] = miniEKF.angle.roll;
		showData[2] = miniEKF.angle.yaw;
	
		showData[3] = miniEKF.imu.gyroN;
		showData[4] = miniEKF.imu.gyroE;
		showData[5] = miniEKF.imu.gyroD;


    for ( indx = 0; indx < SHOW_DATA_LEN; indx++ )
    {
			MY_float2buf(&packageTxMsgBuf[4 * indx + 3],  showData[indx]);    
		}

    showDataMsgLen = 4 * SHOW_DATA_LEN + 3 + 1;

    packageTxMsgBuf[0] = 0x88;
    packageTxMsgBuf[1] = 0xA1;
		packageTxMsgBuf[2] = 4 * SHOW_DATA_LEN;

    packageTxMsgBuf[showDataMsgLen - 1] = GetCheckSum8(packageTxMsgBuf, showDataMsgLen - 1);

    AppComm_SendDebugData(packageTxMsgBuf, showDataMsgLen);
}


/************************ (C) COPYRIGHT FORWARDX *****END OF FILE****/
