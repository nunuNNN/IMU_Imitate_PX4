/*
******************************************************************************
*@文件名字  : BspSoftMasterI2C.h
*@描述	    : I2C主从机模拟，
******************************************************************************
*/

#ifndef _BSP_SOFT_I2C_H_
#define _BSP_SOFT_I2C_H_


#ifdef __cplusplus
 extern "C" {
#endif 


#include "stmHead.h"

#define BSP_I2C_OK      (1)
#define BSP_I2C_ERR     (0)

#define BSP_I2C_ACK		  (1)
#define BSP_I2C_NO_ACK	(0)
      
#define BSP_I2C_WRITE_MASK  (0xFE)
#define BSP_I2C_READ_MASK   (0x01)
   
#define BSP_ADDR_SIZE_8BIT    ((u8)(0x01))
#define BSP_ADDR_SIZE_16BIT		((u8)(0x10))
	 
	   
typedef struct
{
	GPIO_STRUCT SCL;
	GPIO_STRUCT SDA;
  GPIO_STRUCT WP;
  u16 Speed; 
  u16 DelayLow;
  u16 DelayHigh; 
  u16 ErrCnt;
  u8 AddrSize;
	bool isUseWP;
}BspI2c_STRUCT;

extern void BspSoftI2c_DelayUs(u32 delayUs);

extern void BspSoftI2c_Init(BspI2c_STRUCT* pI2c);

extern void BspSoftI2c_Start(BspI2c_STRUCT* pI2c);
extern void BspSoftI2c_Stop(BspI2c_STRUCT* pI2c);
extern u8 BspSoftI2c_WaitAck(BspI2c_STRUCT* pI2c);
extern void BspSoftI2c_Ack(BspI2c_STRUCT* pI2c);
extern void BspSoftI2c_NoAck(BspI2c_STRUCT* pI2c);
extern bool BspSoftI2c_SendByte(BspI2c_STRUCT* pI2c, u8 data);
extern u8 BspSoftI2c_ReadByte(BspI2c_STRUCT* pI2c, u8 Ack);

//extern void BspSoftI2c_ReadBlock(BspI2c_STRUCT* pI2c, u32 devAddr, u32 ReadAddr, u8 *pBuffer, u32 NumToRead);
//extern void BspSoftI2c_WriteBlock(BspI2c_STRUCT* pI2c, u32 devAddr, u32 WriteAddr, u8 *pBuffer, u32 NumToWrite);

//extern u8 BspSoftI2c_Receive(void* pI2c, u8 devAddr,u8 *pBuffer, u16 len);
//extern u8 BspSoftI2c_Transmit(void* pI2c, u8 devAddr, u8 *pBuffer, u16 len);


#ifdef __cplusplus
}
#endif


#endif

/*********END OF FILE****/

