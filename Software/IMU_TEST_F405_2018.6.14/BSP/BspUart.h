/*
******************************************************************************
*@文件名字  : BspUart.h
*@描述	    : Uart BSP 层代码，裸机和RTOS都支持
*       
******************************************************************************
******************************************************************************
*/

#ifndef _BSP_UART_H
#define _BSP_UART_H


#ifdef __cplusplus
 extern "C" {
#endif 


#include "stmHead.h"
   
	 
   
typedef struct
{
	USART_TypeDef* USARTx;
	GPIO_STRUCT Rx;
	GPIO_STRUCT Tx;
  DMA_STRUCT RxDma;
	DMA_STRUCT TxDma;
	u32 USARTx_RCC;
	u32	BaudRate;
	u8 USARTx_IRQn;
	u8 IRQ_MainPriority;
	u8 IRQ_SubPriority;
	bool isUseUartIsr;
}BspUart_STRUCT;

extern void BspUART_Init(BspUart_STRUCT* pUart);

#ifdef __cplusplus
  }
#endif
  
  
#endif

/*********END OF FILE****/


