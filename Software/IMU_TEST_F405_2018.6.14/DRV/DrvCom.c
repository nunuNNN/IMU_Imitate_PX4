/*
******************************************************************************
*@文件名字  : DrvTim.c
*@描述	    : Tim相关底层驱动，裸机和RTOS都支持
******************************************************************************
******************************************************************************
*/


#include "DrvCom.h"
#include "BspUart.h"


#define DRV_COM_BUF_LEN              (128)
#define DRV_BLE_DEBUG_COM_BUF_LEN    (512)



/*uart dma 缓存地址*/
u8 DrvComDMARxBuf1[DRV_COM_BUF_LEN] = {0};
u8 DrvComDMATxBuf1[DRV_COM_BUF_LEN] = {0};

u8 DrvComDMARxBuf2[DRV_COM_BUF_LEN] = {0};
u8 DrvComDMATxBuf2[DRV_COM_BUF_LEN] = {0};

//u8 DrvComDMARxBuf3[DRV_BLE_DEBUG_COM_BUF_LEN] = {0};
//u8 DrvComDMATxBuf3[DRV_BLE_DEBUG_COM_BUF_LEN] = {0};

u8 DrvComDMARxBuf6[DRV_BLE_DEBUG_COM_BUF_LEN] = {0};
u8 DrvComDMATxBuf6[DRV_BLE_DEBUG_COM_BUF_LEN] = {0};

BspUart_STRUCT drvUartArry[PORT_UART_NUM];


void (*pDrvCom_RxDataProc[PORT_UART_NUM])(u8* Buf, u16 Len);



/******************************************************************************
* @函 数 名： DrvCom_Init
* @函数描述： 初始化Com
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvCom_Init(UART_PORT uartId, u32 BaudRate)
{	
	if( PORT_UART1 == uartId )
	{
		drvUartArry[uartId].USARTx           = USART1;
		drvUartArry[uartId].USARTx_RCC       = RCC_APB2Periph_USART1;
		drvUartArry[uartId].USARTx_IRQn      = USART1_IRQn;	
		drvUartArry[uartId].BaudRate         = BaudRate;	
		drvUartArry[uartId].isUseUartIsr     = true;
		drvUartArry[uartId].IRQ_MainPriority = 2;
		drvUartArry[uartId].IRQ_SubPriority  = 1;	

		drvUartArry[uartId].Tx.GPIOx      = GPIOB;
		drvUartArry[uartId].Tx.GPIOx_RCC  = RCC_AHB1Periph_GPIOB;
		drvUartArry[uartId].Tx.PIN        = GPIO_Pin_6;
		drvUartArry[uartId].Tx.PIN_Source = GPIO_PinSource6;
		drvUartArry[uartId].Tx.PIN_AF     = GPIO_AF_USART1;
		
		drvUartArry[uartId].Rx.GPIOx      = GPIOB;
		drvUartArry[uartId].Rx.GPIOx_RCC  = RCC_AHB1Periph_GPIOB;
		drvUartArry[uartId].Rx.PIN        = GPIO_Pin_7;
		drvUartArry[uartId].Rx.PIN_Source = GPIO_PinSource7;
		drvUartArry[uartId].Rx.PIN_AF     = GPIO_AF_USART1;
		
		drvUartArry[uartId].TxDma.DMAx_RCC          = RCC_AHB1Periph_DMA2;
		drvUartArry[uartId].TxDma.DMAy_Streamx      = DMA2_Stream7;
		drvUartArry[uartId].TxDma.DMAy_Chx          = DMA_Channel_4;
		drvUartArry[uartId].TxDma.buf               = &DrvComDMATxBuf1[0];
		drvUartArry[uartId].TxDma.bufLen            = DRV_COM_BUF_LEN;
		drvUartArry[uartId].TxDma.isUseDmaIsr       = true;
		drvUartArry[uartId].TxDma.DMAy_Streamx_IRQn = DMA2_Stream7_IRQn;
		drvUartArry[uartId].TxDma.IRQ_MainPriority  = 2;
		drvUartArry[uartId].TxDma.IRQ_SubPriority   = 1;
		
		drvUartArry[uartId].RxDma.DMAx_RCC          = RCC_AHB1Periph_DMA2;
		drvUartArry[uartId].RxDma.DMAy_Streamx      = DMA2_Stream2;
		drvUartArry[uartId].RxDma.DMAy_Chx          = DMA_Channel_4;
		drvUartArry[uartId].RxDma.buf               = &DrvComDMARxBuf1[0];
		drvUartArry[uartId].RxDma.bufLen            = DRV_COM_BUF_LEN;
		drvUartArry[uartId].RxDma.isUseDmaIsr       = true;
		drvUartArry[uartId].RxDma.DMAy_Streamx_IRQn = DMA2_Stream2_IRQn;
		drvUartArry[uartId].RxDma.IRQ_MainPriority  = 1;
		drvUartArry[uartId].RxDma.IRQ_SubPriority   = 1;
		BspUART_Init(&drvUartArry[uartId]);
	}	
	else 	if( PORT_UART2 == uartId )	
	{
		drvUartArry[uartId].USARTx           = USART2;
		drvUartArry[uartId].USARTx_RCC       = RCC_APB1Periph_USART2;
		drvUartArry[uartId].USARTx_IRQn      = USART2_IRQn;	
		drvUartArry[uartId].BaudRate         = BaudRate;	
		drvUartArry[uartId].isUseUartIsr     = true;
		drvUartArry[uartId].IRQ_MainPriority = 1;
		drvUartArry[uartId].IRQ_SubPriority  = 2;	

		drvUartArry[uartId].Tx.GPIOx      = GPIOA;
		drvUartArry[uartId].Tx.GPIOx_RCC  = RCC_AHB1Periph_GPIOA;
		drvUartArry[uartId].Tx.PIN        = GPIO_Pin_2;
		drvUartArry[uartId].Tx.PIN_Source = GPIO_PinSource2;
		drvUartArry[uartId].Tx.PIN_AF     = GPIO_AF_USART2;
		
		drvUartArry[uartId].Rx.GPIOx      = GPIOA;
		drvUartArry[uartId].Rx.GPIOx_RCC  = RCC_AHB1Periph_GPIOA;
		drvUartArry[uartId].Rx.PIN        = GPIO_Pin_3;
		drvUartArry[uartId].Rx.PIN_Source = GPIO_PinSource3;
		drvUartArry[uartId].Rx.PIN_AF     = GPIO_AF_USART2;
		
		drvUartArry[uartId].TxDma.DMAx_RCC          = RCC_AHB1Periph_DMA1;
		drvUartArry[uartId].TxDma.DMAy_Streamx      = DMA1_Stream6;
		drvUartArry[uartId].TxDma.DMAy_Chx          = DMA_Channel_4;
		drvUartArry[uartId].TxDma.buf               = &DrvComDMATxBuf2[0];
		drvUartArry[uartId].TxDma.bufLen            = DRV_COM_BUF_LEN;
		drvUartArry[uartId].TxDma.isUseDmaIsr       = true;
		drvUartArry[uartId].TxDma.DMAy_Streamx_IRQn = DMA1_Stream6_IRQn;
		drvUartArry[uartId].TxDma.IRQ_MainPriority  = 1;
		drvUartArry[uartId].TxDma.IRQ_SubPriority   = 1;
		
		drvUartArry[uartId].RxDma.DMAx_RCC          = RCC_AHB1Periph_DMA1;
		drvUartArry[uartId].RxDma.DMAy_Streamx      = DMA1_Stream5;
		drvUartArry[uartId].RxDma.DMAy_Chx          = DMA_Channel_4;
		drvUartArry[uartId].RxDma.buf               = &DrvComDMARxBuf2[0];
		drvUartArry[uartId].RxDma.bufLen            = DRV_COM_BUF_LEN;
		drvUartArry[uartId].RxDma.isUseDmaIsr       = true;
		drvUartArry[uartId].RxDma.DMAy_Streamx_IRQn = DMA1_Stream5_IRQn;
		drvUartArry[uartId].RxDma.IRQ_MainPriority  = 3;
		drvUartArry[uartId].RxDma.IRQ_SubPriority   = 2;
		BspUART_Init(&drvUartArry[uartId]);
	}	
	else 	if( PORT_UART6 == uartId )	
	{
		// ADD BLE	
		drvUartArry[uartId].USARTx           = USART6;
		drvUartArry[uartId].USARTx_RCC       = RCC_APB2Periph_USART6;
		drvUartArry[uartId].USARTx_IRQn      = USART6_IRQn;	
		drvUartArry[uartId].BaudRate         = BaudRate;	
		drvUartArry[uartId].isUseUartIsr     = true;
		drvUartArry[uartId].IRQ_MainPriority = 4;
		drvUartArry[uartId].IRQ_SubPriority  = 1;	

		drvUartArry[uartId].Tx.GPIOx      = GPIOC;
		drvUartArry[uartId].Tx.GPIOx_RCC  = RCC_AHB1Periph_GPIOC;
		drvUartArry[uartId].Tx.PIN        = GPIO_Pin_6;
		drvUartArry[uartId].Tx.PIN_Source = GPIO_PinSource6;
		drvUartArry[uartId].Tx.PIN_AF     = GPIO_AF_USART6;
		
		drvUartArry[uartId].Rx.GPIOx      = GPIOC;
		drvUartArry[uartId].Rx.GPIOx_RCC  = RCC_AHB1Periph_GPIOC;
		drvUartArry[uartId].Rx.PIN        = GPIO_Pin_7;
		drvUartArry[uartId].Rx.PIN_Source = GPIO_PinSource7;
		drvUartArry[uartId].Rx.PIN_AF     = GPIO_AF_USART6;
		
		drvUartArry[uartId].TxDma.DMAx_RCC          = RCC_AHB1Periph_DMA2;
		drvUartArry[uartId].TxDma.DMAy_Streamx      = DMA2_Stream6;
		drvUartArry[uartId].TxDma.DMAy_Chx          = DMA_Channel_5;
		drvUartArry[uartId].TxDma.buf               = &DrvComDMATxBuf6[0];
		drvUartArry[uartId].TxDma.bufLen            = DRV_BLE_DEBUG_COM_BUF_LEN;
		drvUartArry[uartId].TxDma.isUseDmaIsr       = true;
		drvUartArry[uartId].TxDma.DMAy_Streamx_IRQn = DMA2_Stream6_IRQn;
		drvUartArry[uartId].TxDma.IRQ_MainPriority  = 4;
		drvUartArry[uartId].TxDma.IRQ_SubPriority   = 1;
			
		drvUartArry[uartId].RxDma.DMAx_RCC          = RCC_AHB1Periph_DMA2;
		drvUartArry[uartId].RxDma.DMAy_Streamx      = DMA2_Stream1;
		drvUartArry[uartId].RxDma.DMAy_Chx          = DMA_Channel_5;
		drvUartArry[uartId].RxDma.buf               = &DrvComDMARxBuf6[0];
		drvUartArry[uartId].RxDma.bufLen            = DRV_BLE_DEBUG_COM_BUF_LEN;
		drvUartArry[uartId].RxDma.isUseDmaIsr       = true;
		drvUartArry[uartId].RxDma.DMAy_Streamx_IRQn = DMA2_Stream1_IRQn;
		drvUartArry[uartId].RxDma.IRQ_MainPriority  = 4;
		drvUartArry[uartId].RxDma.IRQ_SubPriority   = 1;
		BspUART_Init(&drvUartArry[uartId]);	
  }		
		
}

/******************************************************************************
* @函 数 名： DrvCom_RegisterRxHandler
* @函数描述： 注册串口通信处理函数
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvCom_RegisterRxHandler(UART_PORT port, void *pFun)
{
  if(pFun)
  {
    pDrvCom_RxDataProc[port] = (void(*)(u8* , u16 ))pFun;
  }
}

/******************************************************************************
* @函 数 名： DrvCom_DmaSendData
* @函数描述： 串口DMA发送
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvCom_SendData(UART_PORT port, u8* buf, u16 len)
{ 
	if( PORT_UART_NUM <= port )
	{
		return;
	}
	
	BspUart_STRUCT *pDrvUart = &drvUartArry[port];
	
	if( len > pDrvUart->TxDma.bufLen )
	{
		return;
	}
	
	if(DMA_GetCmdStatus(pDrvUart->TxDma.DMAy_Streamx) == DISABLE)
	{
		memcpy(pDrvUart->TxDma.buf, buf, len);

		DMA_SetCurrDataCounter(pDrvUart->TxDma.DMAy_Streamx, len);
		DMA_Cmd(pDrvUart->TxDma.DMAy_Streamx, ENABLE);
	} 
}


/******************************************************************************
* @函 数 名： DrvCom_Idle_IRQHandler
* @函数描述： 串口空闲中断处理
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
static void DrvCom_Idle_IRQHandler(UART_PORT port) 
{
	u16 rxLen = 0; 
	u32 tmp;

	if( PORT_UART_NUM <= port )
	{
		return;
	}
	
	BspUart_STRUCT *pDrvUart = &drvUartArry[port];
  if(USART_GetITStatus(pDrvUart->USARTx, USART_IT_IDLE) != RESET)  
  {  
    tmp = pDrvUart->USARTx->SR;  
    tmp = pDrvUart->USARTx->DR; //清USART_IT_IDLE标志  
    USART_ClearITPendingBit(pDrvUart->USARTx, USART_IT_IDLE);

    DMA_Cmd(pDrvUart->RxDma.DMAy_Streamx, DISABLE);  

    //获得当前接收到数据长度
    rxLen = pDrvUart->RxDma.bufLen - DMA_GetCurrDataCounter(pDrvUart->RxDma.DMAy_Streamx);  
 
    if( rxLen > 0 )
		{
			if(pDrvCom_RxDataProc[port] != NULL)
			{
				pDrvCom_RxDataProc[port](pDrvUart->RxDma.buf, rxLen);
			}	
	  }
    
		DMA_SetCurrDataCounter(pDrvUart->RxDma.DMAy_Streamx, pDrvUart->RxDma.bufLen); 
    DMA_Cmd(pDrvUart->RxDma.DMAy_Streamx, ENABLE);  //使能DMA RX		
  } 
}


/******************************************************************************
*      											串口1 中断处理
******************************************************************************/
/******************************************************************************
* @函 数 名： USART1_IRQHandler
* @函数描述： 串口1接收中断  
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void USART1_IRQHandler(void)                                 
{     
	DrvCom_Idle_IRQHandler(PORT_UART1);
}  

/******************************************************************************
* @函 数 名： DMA2_Stream7_IRQHandler
* @函数描述： UART1 DMA TX
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DMA2_Stream7_IRQHandler(void)
{
	UART_PORT port = PORT_UART1;
	BspUart_STRUCT *pDrvUart = &drvUartArry[port];
	
  DMA_ClearITPendingBit(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TEIF7);

  if(DMA_GetITStatus(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TCIF7) != RESET)
  {
    DMA_ClearITPendingBit(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TCIF7);
    DMA_Cmd(pDrvUart->TxDma.DMAy_Streamx, DISABLE);
	}
}


/******************************************************************************
* @函 数 名： DMA2_Stream2_IRQHandler
* @函数描述： UART1 DMA RX
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DMA2_Stream2_IRQHandler(void)
{
	UART_PORT port = PORT_UART1;
	BspUart_STRUCT *pDrvUart = &drvUartArry[port];
	
	DMA_ClearITPendingBit(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TEIF2);
  
	if(DMA_GetITStatus(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TCIF2) != RESET)      
  {      
    DMA_ClearITPendingBit(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TCIF2);

    if(pDrvCom_RxDataProc[port] != NULL)
    {
      pDrvCom_RxDataProc[port](pDrvUart->RxDma.buf, pDrvUart->RxDma.bufLen);
    }	
	}	
}





/******************************************************************************
*      											串口2 中断处理
******************************************************************************/
/******************************************************************************
* @函 数 名： USART2_IRQHandler
* @函数描述： 串口2接收中断  
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void USART2_IRQHandler(void)                                 
{     
	DrvCom_Idle_IRQHandler(PORT_UART2);
}  

/******************************************************************************
* @函 数 名： DMA1_Stream6_IRQHandler
* @函数描述： UART2 DMA TX
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DMA1_Stream6_IRQHandler(void)
{
	UART_PORT port = PORT_UART2;
	BspUart_STRUCT *pDrvUart = &drvUartArry[port];
	
  DMA_ClearITPendingBit(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TEIF6);

  if(DMA_GetITStatus(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TCIF6) != RESET)
  {
    DMA_ClearITPendingBit(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TCIF6);
    DMA_Cmd(pDrvUart->TxDma.DMAy_Streamx, DISABLE);
	}
}


/******************************************************************************
* @函 数 名： DMA1_Stream5_IRQHandler
* @函数描述： UART2 DMA RX
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DMA1_Stream5_IRQHandler(void)
{
	UART_PORT port = PORT_UART2;
	BspUart_STRUCT *pDrvUart = &drvUartArry[port];
	
	DMA_ClearITPendingBit(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TEIF5);
  
	if(DMA_GetITStatus(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TCIF5) != RESET)      
  {      
    DMA_ClearITPendingBit(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TCIF5);

    if(pDrvCom_RxDataProc[port] != NULL)
    {
      pDrvCom_RxDataProc[port]( pDrvUart->RxDma.buf, pDrvUart->RxDma.bufLen);
    }		
	}	
}




/******************************************************************************
*      											串口6 中断处理
******************************************************************************/
/******************************************************************************
* @函 数 名： USART6_IRQHandler
* @函数描述： 串口6 接收中断  
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void USART6_IRQHandler(void)                                 
{     
	DrvCom_Idle_IRQHandler(PORT_UART6);
}  

/******************************************************************************
* @函 数 名： DMA2_Stream6_IRQHandler
* @函数描述： UART6 DMA TX
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DMA2_Stream6_IRQHandler(void)
{
	UART_PORT port = PORT_UART6;
	BspUart_STRUCT *pDrvUart = &drvUartArry[port];
	
  DMA_ClearITPendingBit(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TEIF6);

  if(DMA_GetITStatus(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TCIF6) != RESET)
  {
    DMA_ClearITPendingBit(pDrvUart->TxDma.DMAy_Streamx, DMA_IT_TCIF6);
    DMA_Cmd(pDrvUart->TxDma.DMAy_Streamx, DISABLE);
	}
}


/******************************************************************************
* @函 数 名： DMA2_Stream1_IRQHandler
* @函数描述： UART6 DMA RX
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DMA2_Stream1_IRQHandler(void)
{
	UART_PORT port = PORT_UART6;
	BspUart_STRUCT *pDrvUart = &drvUartArry[port];
	
	DMA_ClearITPendingBit(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TEIF1);
  
	if(DMA_GetITStatus(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TCIF1) != RESET)      
  {      
    DMA_ClearITPendingBit(pDrvUart->RxDma.DMAy_Streamx, DMA_IT_TCIF1);

    if(pDrvCom_RxDataProc[port] != NULL)
    {
      pDrvCom_RxDataProc[port](pDrvUart->RxDma.buf, pDrvUart->RxDma.bufLen);
    }	
	}	
}



/*********END OF FILE****/


