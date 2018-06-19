/**
  ******************************************************************************
  * @file    AppComm.c
  * @brief   通信处理
  ******************************************************************************
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "AppComm.h"
#include "DrvUSB.h"
#include "public.h"

#define COMM_DEBUG_BAUD_RATE    (115200)  //460800


u8 commSendMsgBuf[256] = { 0 };
u8 packageTxMsgBuf[256] = { 0 };


extern void AppCom_recvComm2PCDataProc(u8* buf, u16 len);
extern void AppCom_recvComm2BLEDataProc(u8* buf, u16 len);
/******************************************************************************
* @函 数 名： comm_Init
* @函数描述： 通信处理初始化
* @参    数：
* @返 回 值：
* @备    注：
******************************************************************************/
void AppComm_Init(void)
{
	DrvCom_Init(PORT_UART2, COMM_DEBUG_BAUD_RATE); 
	DrvCom_RegisterRxHandler(PORT_UART2, &AppCom_recvComm2PCDataProc);
}



/******************************************************************************
* @函 数 名： AppComm_SendDebugData
* @函数描述： 通信处理初始化
* @参    数：
* @返 回 值：
* @备    注：
******************************************************************************/
void AppComm_SendDebugData(u8* buf, u16 len)
{	
	DrvCom_SendData(PORT_UART2, buf, len);       
}


/************************ (C) COPYRIGHT FORWARDX *****END OF FILE****/
