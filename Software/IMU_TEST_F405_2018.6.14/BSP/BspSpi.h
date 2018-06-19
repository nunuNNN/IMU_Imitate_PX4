/*
******************************************************************************
*@文件名字  : BspSpi.h
*@描述	    : 
******************************************************************************
*/

#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_


#ifdef __cplusplus
 extern "C" {
#endif 


#include "stmHead.h"

	 	 
typedef struct
{
	SPI_TypeDef* SPIx;
	GPIO_STRUCT CS;
	GPIO_STRUCT CLK;
	GPIO_STRUCT MISO;
	GPIO_STRUCT MOSI;
  DMA_STRUCT RxDma;
	DMA_STRUCT TxDma;
	u32 SPIx_RCC;
	u8 SPIx_IRQn;
	u8 IRQ_MainPriority;
	u8 IRQ_SubPriority;
	bool isUseSpiIsr;
}SPI_STRUCT;

extern void BspSPI_Init(SPI_STRUCT* pSpi);
extern void BspSpi_CS_HIGH(SPI_STRUCT* pSpi);
extern void BspSpi_CS_LOW(SPI_STRUCT* pSpi);

#ifdef __cplusplus
}
#endif


#endif /*_BSP_SPI_H_*/

/*********END OF FILE****/

