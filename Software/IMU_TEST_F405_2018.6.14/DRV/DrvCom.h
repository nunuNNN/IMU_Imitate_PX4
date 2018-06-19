/*
******************************************************************************
*@文件名字  : DrvCom.h
*@描述	    : Communication相关底层驱动，裸机和RTOS都支持
******************************************************************************
******************************************************************************
*/

#ifndef _DRV_COM_H_
#define _DRV_COM_H_


#ifdef __cplusplus
 extern "C" {
#endif 

#include "stmHead.h"
	  
typedef enum
{
	PORT_UART1, //-->motor key
	PORT_UART2, //-->motor
	//PORT_UART3, //-->DEBUG
	PORT_UART6, //-->BLE
  PORT_UART_NUM, //UART端口个数
}UART_PORT; 

extern void DrvCom_Init(UART_PORT uartId, u32 BaudRate);
extern void DrvCom_RegisterRxHandler(UART_PORT port, void *pFun);
extern void DrvCom_SendData(UART_PORT port, u8* sendBuf, u16 sendLen);



#ifdef __cplusplus
}
#endif


#endif

/*********END OF FILE****/


