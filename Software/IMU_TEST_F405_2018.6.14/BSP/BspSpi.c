/*
******************************************************************************
*@文件名字  : BspSpi.c
*@描述	    : 
****************************************************************************** 
******************************************************************************
*/

#include "BspSpi.h"



static void SPI_GpioInit(SPI_STRUCT* pSPI)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(pSPI->CS.GPIOx_RCC, ENABLE);	
	RCC_AHB1PeriphClockCmd(pSPI->CLK.GPIOx_RCC, ENABLE);	
	RCC_AHB1PeriphClockCmd(pSPI->MISO.GPIOx_RCC, ENABLE);	
	RCC_AHB1PeriphClockCmd(pSPI->MOSI.GPIOx_RCC, ENABLE);	
	
//  GPIO_PinAFConfig(pSPI->CS.GPIOx, pSPI->CS.PIN_Source, pSPI->CS.PIN_AF);
  GPIO_PinAFConfig(pSPI->CLK.GPIOx, pSPI->CLK.PIN_Source, pSPI->CLK.PIN_AF);
  GPIO_PinAFConfig(pSPI->MISO.GPIOx, pSPI->MISO.PIN_Source, pSPI->MISO.PIN_AF);
  GPIO_PinAFConfig(pSPI->MOSI.GPIOx, pSPI->MOSI.PIN_Source, pSPI->MOSI.PIN_AF);
	
	GPIO_StructInit(&GPIO_InitStructure);      

  /*CS*/
  GPIO_InitStructure.GPIO_Pin = pSPI->CS.PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(pSPI->CS.GPIOx, &GPIO_InitStructure);

  /*CLK*/
  GPIO_InitStructure.GPIO_Pin = pSPI->CLK.PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(pSPI->CLK.GPIOx, &GPIO_InitStructure);
	
	 /*MISO*/
  GPIO_InitStructure.GPIO_Pin = pSPI->MISO.PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(pSPI->MISO.GPIOx, &GPIO_InitStructure);

  /*MOSI*/
  GPIO_InitStructure.GPIO_Pin = pSPI->MOSI.PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(pSPI->MOSI.GPIOx, &GPIO_InitStructure);
}


static void SPI_DmaInit(SPI_STRUCT* pSPI)
{
	DMA_InitTypeDef DMA_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(pSPI->TxDma.DMAx_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(pSPI->RxDma.DMAx_RCC, ENABLE);
	
	DMA_DeInit(pSPI->TxDma.DMAy_Streamx);
  
	//TX
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel            = pSPI->TxDma.DMAy_Chx;             
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&pSPI->SPIx->DR; //数据传输的外设首地址
	DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&pSPI->TxDma.buf[0];  //自己定义待发送数组的首地址，要强制转换为32位
	DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;    //数据传输方向选择为内存->外设
	DMA_InitStructure.DMA_BufferSize         = 0;    //传输数据大小 
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;    //内存地址自增允许，因为要读取一个数组
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设的数据大小，因为USART6_DR数据寄存器为8为，故选Byte
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;  //这里也选Byte
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;       //DMA传输模式为Normal，如果为Circular,将会循环传输
	DMA_InitStructure.DMA_Priority           = DMA_Priority_High; //优先级为High
	DMA_Init(pSPI->TxDma.DMAy_Streamx, &DMA_InitStructure);

//	DMA_ITConfig(pSPI->TxDma.DMAy_Streamx, DMA_IT_TC, ENABLE); //使能传输完成中断  
	DMA_Cmd(pSPI->TxDma.DMAy_Streamx, DISABLE);                

	// RX
  DMA_DeInit(pSPI->RxDma.DMAy_Streamx);
	DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_Channel            = pSPI->RxDma.DMAy_Chx;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&pSPI->SPIx->DR;
  DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&pSPI->RxDma.buf[0];
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize         = pSPI->RxDma.bufLen;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
  DMA_Init(pSPI->RxDma.DMAy_Streamx, &DMA_InitStructure);

  DMA_ITConfig(pSPI->RxDma.DMAy_Streamx, DMA_IT_TC, ENABLE); 
  DMA_Cmd(pSPI->RxDma.DMAy_Streamx, DISABLE);

}



static void SPI_Config(SPI_STRUCT* pSPI)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	if( pSPI->SPIx == SPI1 )
	{
		RCC_APB2PeriphClockCmd(pSPI->SPIx_RCC, ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(pSPI->SPIx_RCC, ENABLE);
	}
	
  SPI_DeInit(pSPI->SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;  //CLK空闲时为高电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //CLK上升沿采样

  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; //APB1=42MHz,42/8= 5.2Mhz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(pSPI->SPIx, &SPI_InitStructure);

  SPI_DMACmd(pSPI->SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
  SPI_DMACmd(pSPI->SPIx, SPI_I2S_DMAReq_Rx, ENABLE);

  SPI_Cmd(pSPI->SPIx, ENABLE);
	 	 
  SPI_ITConfig(pSPI->SPIx, SPI_IT_RXNE, ENABLE);
	
}


static void SPI_NVICInit(SPI_STRUCT* pSPI)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	if( true == pSPI->TxDma.isUseDmaIsr )
	{
		//Tx isr
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);        //嵌套优先级分组
		NVIC_InitStructure.NVIC_IRQChannel = pSPI->TxDma.DMAy_Streamx_IRQn;   //嵌套通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pSPI->TxDma.IRQ_MainPriority; //抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = pSPI->TxDma.IRQ_SubPriority;    //响应优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
		NVIC_Init(&NVIC_InitStructure);
	}
	
	if( true == pSPI->RxDma.isUseDmaIsr )
	{
		//Rx isr
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);        //嵌套优先级分组
		NVIC_InitStructure.NVIC_IRQChannel = pSPI->RxDma.DMAy_Streamx_IRQn;   //嵌套通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pSPI->RxDma.IRQ_MainPriority; //抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = pSPI->RxDma.IRQ_SubPriority;    //响应优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
		NVIC_Init(&NVIC_InitStructure);
	}

	if( true == pSPI->isUseSpiIsr )
	{
		/*中断配置*/
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  
		NVIC_InitStructure.NVIC_IRQChannel = pSPI->SPIx_IRQn;        //通道设置为串口中断     
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pSPI->IRQ_MainPriority;       //中断占先等级    
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = pSPI->IRQ_SubPriority;              //中断响应优先级    
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断     
		NVIC_Init(&NVIC_InitStructure);
	}  	
}

/******************************************************************************
* @函 数 名： BspSPI_Init
* @函数描述： SPI 初始化
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/  
void BspSPI_Init(SPI_STRUCT* pSpi)
{
	SPI_GpioInit(pSpi);
	
	SPI_Config(pSpi);	
	
	SPI_DmaInit(pSpi);
	
	SPI_NVICInit(pSpi);
}


/******************************************************************************
* @函 数 名： BspSpi_CS_HIGH
* @函数描述： CS置高
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/  
 void BspSpi_CS_HIGH(SPI_STRUCT* pSpi)
{
	pSpi->CS.GPIOx->BSRRL = pSpi->CS.PIN; 
}

/******************************************************************************
* @函 数 名： BspSpi_CS_LOW
* @函数描述： CS置低
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/  
 void BspSpi_CS_LOW(SPI_STRUCT* pSpi)
{
	pSpi->CS.GPIOx->BSRRH = pSpi->CS.PIN;
}
/*********END OF FILE****/

