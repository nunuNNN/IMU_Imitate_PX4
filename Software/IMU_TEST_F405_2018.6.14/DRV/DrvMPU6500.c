
#include "DrvMPU6500.h"

/******************************************************************************
* @函 数 名： DrvMPU6500_CS_HIGH
* @函数描述： CS H
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvMPU6500_CS_HIGH(MPU6500_STRUCT* pMPU6500)
{
	BspSpi_CS_HIGH(&pMPU6500->SPIx);
}

/******************************************************************************
* @函 数 名： DrvMPU6500_CS_LOW
* @函数描述： CS L
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvMPU6500_CS_LOW(MPU6500_STRUCT* pMPU6500)
{
	BspSpi_CS_LOW(&pMPU6500->SPIx);
}



/******************************************************************************
* @函 数 名： DrvMPU6500_Init
* @函数描述： MPU6500初始化
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvMPU6500_Init(MPU6500_STRUCT* pMPU6500)
{
	BspSPI_Init(&pMPU6500->SPIx);
	BspSpi_CS_HIGH(&pMPU6500->SPIx);
	
	pMPU6500->state = State_idle;
}



/******************************************************************************
* @函 数 名： DrvMPU6500_WriteReg
* @函数描述： MPU6500写寄存器
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvMPU6500_WriteReg(MPU6500_STRUCT* pMPU6500, u8 Reg, u8 Data)
{	
	if (DMA_GetCmdStatus(pMPU6500->SPIx.RxDma.DMAy_Streamx) == DISABLE)
	{    
		DrvMPU6500_CS_LOW(pMPU6500);//L
		
		pMPU6500->SPIx.TxDma.buf[0] = Reg;
		pMPU6500->SPIx.TxDma.buf[1] = Data;
		pMPU6500->SPIx.TxDma.dataLen = 2;
		pMPU6500->SPIx.RxDma.dataLen = 2;
		
		DMA_SetCurrDataCounter(pMPU6500->SPIx.RxDma.DMAy_Streamx, pMPU6500->SPIx.RxDma.dataLen);				
		DMA_Cmd(pMPU6500->SPIx.RxDma.DMAy_Streamx, ENABLE);		
							
		DMA_SetCurrDataCounter(pMPU6500->SPIx.TxDma.DMAy_Streamx, pMPU6500->SPIx.TxDma.dataLen);
		DMA_Cmd(pMPU6500->SPIx.TxDma.DMAy_Streamx, ENABLE);
	
	}
}


/******************************************************************************
* @函 数 名： DrvMPU6500_ReadReg
* @函数描述： MPU6500读寄存器
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvMPU6500_ReadReg(MPU6500_STRUCT* pMPU6500, u8 Reg, u16 Len)
{ 	
	if( pMPU6500->SPIx.TxDma.bufLen < Len )
	{
		return;
	}

	if (DMA_GetCmdStatus(pMPU6500->SPIx.RxDma.DMAy_Streamx) == DISABLE)
	{    
		DrvMPU6500_CS_LOW(pMPU6500);//L
		
		pMPU6500->txCnt++;
		pMPU6500->SPIx.TxDma.buf[0] = Reg|0x80; 		
		pMPU6500->SPIx.TxDma.dataLen = Len;
		pMPU6500->SPIx.RxDma.dataLen = Len;
		
		DMA_SetCurrDataCounter(pMPU6500->SPIx.RxDma.DMAy_Streamx, pMPU6500->SPIx.RxDma.dataLen);				
		DMA_Cmd(pMPU6500->SPIx.RxDma.DMAy_Streamx, ENABLE);
			
		DMA_SetCurrDataCounter(pMPU6500->SPIx.TxDma.DMAy_Streamx, pMPU6500->SPIx.TxDma.dataLen);
		DMA_Cmd(pMPU6500->SPIx.TxDma.DMAy_Streamx, ENABLE);
		
	}
}


/******************************************************************************
* @函 数 名： DrvMPU6500_Read
* @函数描述： MPU6500周期读数据
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvMPU6500_Read(MPU6500_STRUCT* pMPU6500, u32 loopHz)
{
	switch(pMPU6500->state)
	{ 
    case State_idle:
		{ 
      //Register 28  Accelerometer Configuration		
			pMPU6500->state = State_Config_Step1;
			DrvMPU6500_WriteReg(pMPU6500, ACCEL_CONFIG, 0x18);//±16g 
		}break;
    
		case State_Config_Step1:
		{
      // Register 107 Power Management 1
			pMPU6500->state = State_Config_Step2;
			DrvMPU6500_WriteReg(pMPU6500, PWR_MGMT_1, 0x00);
		}break;

		case State_Config_Step2:
		{
      //  Register 26 Configuration	
			pMPU6500->state = State_Config_Step3;
			DrvMPU6500_WriteReg(pMPU6500, IMU_CONFIG, 0x00);
		}break;

		case State_Config_Step3:
		{    
      // Register 25 Sample Rate Divider
			pMPU6500->state = State_Config_Step4;
			DrvMPU6500_WriteReg(pMPU6500, SMPLRT_DIV, 0x00);//1kHz internal sampling rate
		}break;

		case State_Config_Step4:
		{ 
      // Register 27 Gyroscope Configuration	
			pMPU6500->state = State_Config_Step5;
			DrvMPU6500_WriteReg(pMPU6500, GYRO_CONFIG, 0x18); // ±2000dps
		}break;

		case State_Config_Step5:
		{ 
      //Register 28  Accelerometer Configuration		
			pMPU6500->state = State_Config_Step6;
			DrvMPU6500_WriteReg(pMPU6500, ACCEL_CONFIG, 0x18);//±16g 
		}break;
    
 		case State_Config_Step6:
		{ 
      //Register 29 – Accelerometer Configuration 2		
			pMPU6500->state = State_Read_Config;
			DrvMPU6500_WriteReg(pMPU6500, ACCEL_CONFIG1, 0x08); 
		}break;

 		case State_Read_Config:
		{ 
			DrvMPU6500_ReadReg(pMPU6500, ACCEL_CONFIG, 3);
		}break; 		
    
		case State_Operate:
		{
			pMPU6500->state = State_Operate;
			DrvMPU6500_ReadReg(pMPU6500, ACCEL_XOUT_H, 15);
		}break;

		default:
		{
			pMPU6500->state = State_idle;
			break;
		}
	}  
}


/******************************************************************************
* @函 数 名： DrvMPU6500_spiMasterRxCpltCallback
* @函数描述： MPU6500主模式接收中断回调函数
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvMPU6500_spiMasterRxCpltCallback(MPU6500_STRUCT* pMPU6500, u8* data, u16 Len)
{ 
  float fnVal = 0;
	short tmpValue = 0;
  short accValue[3] = {0};
	short gyroValue[3] = {0};
  
  if(pMPU6500 == NULL )
  {
    return;  
  }
  
//   DrvMPU6500_CS_HIGH(pMPU6500);

	//! 15 bytes
	if(Len == 15)
	{
		accValue[0] = (short)(data[1] << 8) | (data[2] & 0x00FF);
		fnVal = accValue[0];
		pMPU6500->data.accX = fnVal/2048.0f;

		accValue[1] =  (short)(data[3] << 8) | (data[4] & 0x00FF);
		fnVal = accValue[1];
		pMPU6500->data.accY = fnVal/2048.0f;

		accValue[2] = (short)(data[5] << 8) | (data[6] & 0x00FF);
		fnVal = accValue[2];
		pMPU6500->data.accZ = fnVal/2048.0f;

		tmpValue = (short)(data[7] << 8) | (data[8] & 0x00FF);
		fnVal = tmpValue;
		pMPU6500->data.temp = (fnVal - 21.0f)/333.87f + 21.0f;

		gyroValue[0] = (short)(data[9] << 8) | (data[10] & 0x00FF);
		fnVal = gyroValue[0];
		pMPU6500->data.gyroX = fnVal/16.4f;

		gyroValue[1] = (short)(data[11] << 8) | (data[12] & 0x00FF);
		fnVal = gyroValue[1];
		pMPU6500->data.gyroY = fnVal/16.4f;

		gyroValue[2] = (short)(data[13] << 8) | (data[14] & 0x00FF);
		fnVal = gyroValue[2];
		pMPU6500->data.gyroZ = fnVal/16.4f;
		
		pMPU6500->rxCnt++;
		  
		if(pMPU6500->pMPU6500RecvDataProc)
		{
			pMPU6500->pMPU6500RecvDataProc(&pMPU6500->data);
		}  
	}
  else if(Len == 3)
  {		
    if( !(0x18 == data[1]) )
    {
      pMPU6500->rxReg28ErrCnt++;
      if(pMPU6500->rxReg28ErrCnt >= 3 )
      {
				pMPU6500->rxCnt = 0;
        pMPU6500->rxReg28ErrCnt = 0;
        pMPU6500->rxRegErrCnt++; 					
        pMPU6500->state = State_idle;
      }
    }
		else
		{
			pMPU6500->rxCnt++;
			if( pMPU6500->rxCnt >= 2 )
			{
				pMPU6500->rxCnt = 0;
				pMPU6500->state = State_Operate;
			}
		}
  }
}



/*********END OF FILE****/


