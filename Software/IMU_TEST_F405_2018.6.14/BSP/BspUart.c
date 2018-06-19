/*
******************************************************************************
*@文件名字  : BspUart.c
*@描述	    : Uart BSP 层代码，裸机和RTOS都支持
*       
******************************************************************************
******************************************************************************
*/

#include "BspUart.h"

static void UART_GpioInit(BspUart_STRUCT* pUart)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(pUart->Tx.GPIOx_RCC, ENABLE);	
	RCC_AHB1PeriphClockCmd(pUart->Rx.GPIOx_RCC, ENABLE);	

  GPIO_PinAFConfig(pUart->Tx.GPIOx, pUart->Tx.PIN_Source, pUart->Tx.PIN_AF);
  GPIO_PinAFConfig(pUart->Rx.GPIOx, pUart->Rx.PIN_Source, pUart->Rx.PIN_AF);
	
	GPIO_StructInit(&GPIO_InitStructure);      

  /*TX*/
  GPIO_InitStructure.GPIO_Pin = pUart->Tx.PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(pUart->Tx.GPIOx, &GPIO_InitStructure);

  /*RX*/
  GPIO_InitStructure.GPIO_Pin = pUart->Rx.PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; 
  GPIO_Init(pUart->Rx.GPIOx, &GPIO_InitStructure);
}


static void UART_DmaInit(BspUart_STRUCT* pUart)
{
	DMA_InitTypeDef DMA_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(pUart->TxDma.DMAx_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(pUart->RxDma.DMAx_RCC, ENABLE);
	
	DMA_DeInit(pUart->TxDma.DMAy_Streamx);
  
	//TX
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel            = pUart->TxDma.DMAy_Chx;             
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&pUart->USARTx->DR; //数据传输的外设首地址
	DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&pUart->TxDma.buf[0];  //自己定义待发送数组的首地址，要强制转换为32位
	DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;    //数据传输方向选择为内存->外设
	DMA_InitStructure.DMA_BufferSize         = 0;    //传输数据大小 
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;    //内存地址自增允许，因为要读取一个数组
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设的数据大小，因为USART6_DR数据寄存器为8为，故选Byte
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;  //这里也选Byte
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;       //DMA传输模式为Normal，如果为Circular,将会循环传输
	DMA_InitStructure.DMA_Priority           = DMA_Priority_High; //优先级为High
	DMA_Init(pUart->TxDma.DMAy_Streamx, &DMA_InitStructure);

	DMA_ITConfig(pUart->TxDma.DMAy_Streamx, DMA_IT_TC, ENABLE); //使能传输完成中断  
	DMA_Cmd(pUart->TxDma.DMAy_Streamx, DISABLE);                

	// RX
  DMA_DeInit(pUart->RxDma.DMAy_Streamx);

	DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_Channel            = pUart->RxDma.DMAy_Chx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&pUart->USARTx->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&pUart->RxDma.buf[0];
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = pUart->RxDma.bufLen;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_Init(pUart->RxDma.DMAy_Streamx, &DMA_InitStructure);

//  DMA_ITConfig(pUart->RxDma.DMAy_Streamx, DMA_IT_TC, ENABLE); //不能使能，否则一直进入接收中断
  DMA_Cmd(pUart->RxDma.DMAy_Streamx, ENABLE);

}



static void UART_Config(BspUart_STRUCT* pUart)
{
	USART_InitTypeDef USART_InitStructure; 

	if( (pUart->USARTx == USART1) || (pUart->USARTx == USART6) )
	{
		RCC_APB2PeriphClockCmd(pUart->USARTx_RCC, ENABLE);//开启USART时钟
	}
	else
	{
		RCC_APB1PeriphClockCmd(pUart->USARTx_RCC, ENABLE);//开启USART时钟
	}

	USART_DeInit(pUart->USARTx);
	USART_InitStructure.USART_BaudRate = pUart->BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(pUart->USARTx, &USART_InitStructure);
	
	USART_ITConfig(pUart->USARTx, USART_IT_IDLE, ENABLE);//接收空闲中断

	USART_DMACmd(pUart->USARTx, USART_DMAReq_Tx, ENABLE);//将USART模块设置成DMA方式工作
	USART_DMACmd(pUart->USARTx, USART_DMAReq_Rx, ENABLE);

	USART_Cmd(pUart->USARTx, ENABLE);

	USART_ClearFlag(pUart->USARTx, USART_FLAG_TC);
	
}


static void UART_NVICInit(BspUart_STRUCT* pUart)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	if( true == pUart->TxDma.isUseDmaIsr )
	{
		//Tx isr
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);        //嵌套优先级分组
		NVIC_InitStructure.NVIC_IRQChannel = pUart->TxDma.DMAy_Streamx_IRQn;   //嵌套通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pUart->TxDma.IRQ_MainPriority; //抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = pUart->TxDma.IRQ_SubPriority;    //响应优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
		NVIC_Init(&NVIC_InitStructure);
	}
	
	if( true == pUart->RxDma.isUseDmaIsr )
	{
		//Rx isr
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);        //嵌套优先级分组
		NVIC_InitStructure.NVIC_IRQChannel = pUart->RxDma.DMAy_Streamx_IRQn;   //嵌套通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pUart->RxDma.IRQ_MainPriority; //抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = pUart->RxDma.IRQ_SubPriority;    //响应优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
		NVIC_Init(&NVIC_InitStructure);
	}

	if( true == pUart->isUseUartIsr )
	{
		/*USART中断配置*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  
		NVIC_InitStructure.NVIC_IRQChannel = pUart->USARTx_IRQn;        //通道设置为串口中断     
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pUart->IRQ_MainPriority;       //中断占先等级    
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = pUart->IRQ_SubPriority;              //中断响应优先级    
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断     
		NVIC_Init(&NVIC_InitStructure); 
	}		
}


void BspUART_Init(BspUart_STRUCT* pUart)
{
	UART_GpioInit(pUart);
	
	UART_Config(pUart);	
	
	UART_DmaInit(pUart);
	
	UART_NVICInit(pUart);
}

