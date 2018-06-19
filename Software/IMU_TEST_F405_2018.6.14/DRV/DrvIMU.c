
#include "DrvIMU.h"
#include "DrvMPU6500.h"

#define DRV_IMU_BUF_LEN  (32)

MPU6500_STRUCT mpu6500DataCamera;
u8 DrvMPU6500CameraTxDmaBuf[DRV_IMU_BUF_LEN] = {0};
u8 DrvMPU6500CameraRxDmaBuf[DRV_IMU_BUF_LEN] = {0};

/******************************************************************************
* @函 数 名： DrvImu_Init
* @函数描述： 初始化IMU
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvImu_Init(void)
{	
	mpu6500DataCamera.SPIx.SPIx = SPI3;
	mpu6500DataCamera.SPIx.SPIx_RCC = RCC_APB1Periph_SPI3;
	mpu6500DataCamera.SPIx.isUseSpiIsr = false;
//	mpu6500DataCamera.SPIx.SPIx_IRQn = SPI3_IRQn;
//	mpu6500DataCamera.SPIx.IRQ_MainPriority = 3;
//	mpu6500DataCamera.SPIx.IRQ_SubPriority = 1;

	//CS
	mpu6500DataCamera.SPIx.CS.GPIOx      = GPIOC;
	mpu6500DataCamera.SPIx.CS.GPIOx_RCC  = RCC_AHB1Periph_GPIOC;
	mpu6500DataCamera.SPIx.CS.PIN        = GPIO_Pin_15;
//	mpu6500DataCamera.SPIx.CS.GPIOx      = GPIOA;
//	mpu6500DataCamera.SPIx.CS.GPIOx_RCC  = RCC_AHB1Periph_GPIOA;
//	mpu6500DataCamera.SPIx.CS.PIN        = GPIO_Pin_15;	

	//CLK
	mpu6500DataCamera.SPIx.CLK.GPIOx      = GPIOC;
	mpu6500DataCamera.SPIx.CLK.GPIOx_RCC  = RCC_AHB1Periph_GPIOC;
	mpu6500DataCamera.SPIx.CLK.PIN        = GPIO_Pin_10;
	mpu6500DataCamera.SPIx.CLK.PIN_Source = GPIO_PinSource10;
	mpu6500DataCamera.SPIx.CLK.PIN_AF     = GPIO_AF_SPI3;

	//MISO
	mpu6500DataCamera.SPIx.MISO.GPIOx      = GPIOC;
	mpu6500DataCamera.SPIx.MISO.GPIOx_RCC  = RCC_AHB1Periph_GPIOC;
	mpu6500DataCamera.SPIx.MISO.PIN        = GPIO_Pin_11;
	mpu6500DataCamera.SPIx.MISO.PIN_Source = GPIO_PinSource11;
	mpu6500DataCamera.SPIx.MISO.PIN_AF     = GPIO_AF_SPI3;

  //MOSI
	mpu6500DataCamera.SPIx.MOSI.GPIOx      = GPIOC;
	mpu6500DataCamera.SPIx.MOSI.GPIOx_RCC  = RCC_AHB1Periph_GPIOC;
	mpu6500DataCamera.SPIx.MOSI.PIN        = GPIO_Pin_12;
	mpu6500DataCamera.SPIx.MOSI.PIN_Source = GPIO_PinSource12;
	mpu6500DataCamera.SPIx.MOSI.PIN_AF     = GPIO_AF_SPI3;
	
	mpu6500DataCamera.SPIx.TxDma.DMAx_RCC          = RCC_AHB1Periph_DMA1;
	mpu6500DataCamera.SPIx.TxDma.DMAy_Streamx      = DMA1_Stream7;
	mpu6500DataCamera.SPIx.TxDma.DMAy_Chx          = DMA_Channel_0;
	mpu6500DataCamera.SPIx.TxDma.DMAy_Streamx_IRQn = DMA1_Stream7_IRQn;
	mpu6500DataCamera.SPIx.TxDma.buf               = &DrvMPU6500CameraTxDmaBuf[0];
	mpu6500DataCamera.SPIx.TxDma.bufLen            = DRV_IMU_BUF_LEN;
	mpu6500DataCamera.SPIx.TxDma.isUseDmaIsr       = false;
	mpu6500DataCamera.SPIx.TxDma.IRQ_MainPriority  = 1;
	mpu6500DataCamera.SPIx.TxDma.IRQ_SubPriority   = 1;
	
	mpu6500DataCamera.SPIx.RxDma.DMAx_RCC          = RCC_AHB1Periph_DMA1;
	mpu6500DataCamera.SPIx.RxDma.DMAy_Streamx      = DMA1_Stream0;
	mpu6500DataCamera.SPIx.RxDma.DMAy_Chx          = DMA_Channel_0;
	mpu6500DataCamera.SPIx.RxDma.DMAy_Streamx_IRQn = DMA1_Stream0_IRQn;
	mpu6500DataCamera.SPIx.RxDma.buf               = &DrvMPU6500CameraRxDmaBuf[0];
	mpu6500DataCamera.SPIx.RxDma.bufLen            = DRV_IMU_BUF_LEN;
	mpu6500DataCamera.SPIx.RxDma.isUseDmaIsr       = true;
	mpu6500DataCamera.SPIx.RxDma.IRQ_MainPriority  = 1;
	mpu6500DataCamera.SPIx.RxDma.IRQ_SubPriority   = 1;
	
	DrvMPU6500_Init(&mpu6500DataCamera); 
}


/******************************************************************************
* @函 数 名： DrvImu_RegisterRecvDataHandler
* @函数描述： 注册IMU接收函数
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvImu_RegisterRecvDataHandler(IMU_PORT port, void *pFun)
{
  switch(port)
  {   
    case IMU_MPU6500_Camera:
    {
			if(pFun)
			{
        mpu6500DataCamera.pMPU6500RecvDataProc = ( void(*)(IMUData_STRUCT*) )pFun; 
			}
    }break;
    
    case IMU_MPU6500_Carrier:
    {
    
    }break;
    
    default: break;
  }  
}


/******************************************************************************
* @函 数 名： DrvImu_Read
* @函数描述： IMU周期读数据函数
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvImu_Read(IMU_PORT port, u32 loopHz)
{
  u32 readLoopHz = loopHz;
 
  switch(port)
  {   
    case IMU_MPU6500_Camera:
    {
        DrvMPU6500_Read(&mpu6500DataCamera, readLoopHz);
    }break;
    
    case IMU_MPU6500_Carrier:
    {
            
    }break;
    
    default: break;
  }
}


/******************************************************************************
* @函 数 名： DrvImu_WriteReg
* @函数描述： imu写寄存器
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvImu_WriteReg(IMU_PORT port, u8 Reg, u8 Data)
{
  u8 writeReg = Reg;
  u8 writeData = Data;

  switch(port)
  {   
    case IMU_MPU6500_Camera:
    {
      DrvMPU6500_WriteReg(&mpu6500DataCamera, writeReg, writeData);
    }break;

    case IMU_MPU6500_Carrier:
    {
    
    }break;

    default: break;
  }  
}

/******************************************************************************
* @函 数 名： DrvImu_ReadReg
* @函数描述： imu读寄存器
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvImu_ReadReg(IMU_PORT port, u8 Reg, u16 Len)
{
  u8 readReg = Reg;
  u16 readLen = Len;
  
  switch(port)
  {   
    case IMU_MPU6500_Camera:
    {
        DrvMPU6500_ReadReg(&mpu6500DataCamera, readReg, readLen);
    }break;
    
    case IMU_MPU6500_Carrier:
    {
     
    }break;
    
    default: break;
  }  
}


//spi3 Rx DMA 
void DMA1_Stream0_IRQHandler(void)
{
	 MPU6500_STRUCT *pMPU6500 = &mpu6500DataCamera;
	
   DrvMPU6500_CS_HIGH(pMPU6500);
	
	 DMA_ClearITPendingBit(pMPU6500->SPIx.TxDma.DMAy_Streamx, DMA_IT_TEIF7);
	 DMA_ClearITPendingBit(pMPU6500->SPIx.TxDma.DMAy_Streamx, DMA_IT_TCIF7);
	 DMA_Cmd(pMPU6500->SPIx.TxDma.DMAy_Streamx, DISABLE); 
	
	 DMA_ClearITPendingBit(pMPU6500->SPIx.RxDma.DMAy_Streamx, DMA_IT_TEIF0);
	 DMA_Cmd(pMPU6500->SPIx.RxDma.DMAy_Streamx, DISABLE);
 	
   if(DMA_GetITStatus(pMPU6500->SPIx.RxDma.DMAy_Streamx, DMA_IT_TCIF0) != RESET)
   {
      DMA_ClearITPendingBit(pMPU6500->SPIx.RxDma.DMAy_Streamx, DMA_IT_TCIF0);			 
			DrvMPU6500_spiMasterRxCpltCallback(pMPU6500, pMPU6500->SPIx.RxDma.buf, pMPU6500->SPIx.RxDma.dataLen);      
	 }
}




/*********END OF FILE****/


