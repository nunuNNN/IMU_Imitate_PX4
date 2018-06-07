#include "stm32f4_icm20689.h"
#include "stm32f4_exti.h"
#include "stm32f4_spi.h"
#include "stm32f4_delay.h"


/*****************************************************************************/
//basic SPI driver for ICM20689
static SPI_Driver mICM20689 = {
	SPI1, RCC_APB2PeriphClockCmd, RCC_APB2Periph_SPI1,
	GPIOB, RCC_APB1PeriphClockCmd, RCC_AHB1Periph_GPIOB,
	GPIOG, RCC_APB1PeriphClockCmd, RCC_AHB1Periph_GPIOG,
	GPIO_Pin_6, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5,
	GPIO_PinSource3, GPIO_PinSource4, GPIO_PinSource5,
		{
		SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, SPI_DataSize_8b, 
		SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_2,
		SPI_FirstBit_MSB, 7
	},
#ifdef SPIx_USE_DMA

#endif
	GPIO_AF_SPI1
};
static SPI_Driver* pICM20689 = &mICM20689;



