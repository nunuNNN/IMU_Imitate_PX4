/*
******************************************************************************
*@文件名字  : BspSoftI2C.c
*@描述	    : I2C主机模拟
****************************************************************************** 
******************************************************************************
*/

#include "BspSoftI2C.h"

void BspSoftI2c_DelayUs(u32 delayUs)
{
    u32 i = 0;
    u32 delay = delayUs;
    while(delay--)
    {
        i = 5;    //自己定义
        while(i--);
    }
}


static inline void BspSoftI2c_delay(u16 num)
{ 
  while (num--);
}

/******************************************************************************
* @函 数 名： BspSoftI2c_SCL_HIGH
* @函数描述： SCL置高
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/  
static inline void BspSoftI2c_SCL_HIGH(BspI2c_STRUCT* pI2c)
{
	pI2c->SCL.GPIOx->BSRRL = pI2c->SCL.PIN; 
}

/******************************************************************************
* @函 数 名： BspSoftI2c_SCL_LOW
* @函数描述： SCL置低
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/  
static inline void BspSoftI2c_SCL_LOW(BspI2c_STRUCT* pI2c)
{
	pI2c->SCL.GPIOx->BSRRH = pI2c->SCL.PIN;
}

/******************************************************************************
* @函 数 名： BspSoftI2c_SDA_HIGH
* @函数描述： SDA置高
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/  
static inline void BspSoftI2c_SDA_HIGH(BspI2c_STRUCT* pI2c)
{
	pI2c->SDA.GPIOx->BSRRL = pI2c->SDA.PIN;	
}

/******************************************************************************
* @函 数 名： BspSoftI2c_SDA_LOW
* @函数描述： SDA置低
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/  
static inline void BspSoftI2c_SDA_LOW(BspI2c_STRUCT* pI2c)
{
	pI2c->SDA.GPIOx->BSRRH = pI2c->SDA.PIN;	
}

/******************************************************************************
* @函 数 名： BspSoftI2cMasterSDARead
* @函数描述： 读取SDA状态
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/  
static inline uint8_t BspSoftI2c_SDA_READ(BspI2c_STRUCT* pI2c)
{
	return GPIO_ReadInputDataBit(pI2c->SDA.GPIOx, pI2c->SDA.PIN); 
}

/******************************************************************************
* @函 数 名： BspI2c_SpeedToDelay
* @函数描述： 延时时间，和具体单片机以及时钟频率有关
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/
static void BspSoftI2c_SpeedToDelay(BspI2c_STRUCT* pI2c)
{
  pI2c->DelayLow = (pI2c->Speed <= 100) ? 120 : ((pI2c->Speed <= 400) ? 50 : 10);
  pI2c->DelayHigh = pI2c->DelayLow * 2;
}

/****************************************************************
* @函 数 名： I2C_gpioInit
* @函数描述： 
* @参    数： 
* @返 回 值： 
* @备    注： 
****************************************************************/ 
void BspSoftI2c_GpioInit(BspI2c_STRUCT* pI2c)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(pI2c->SCL.GPIOx_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(pI2c->SDA.GPIOx_RCC, ENABLE);
	
	//开漏输出，双向通信
  GPIO_InitStructure.GPIO_Pin = pI2c->SDA.PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(pI2c->SDA.GPIOx, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = pI2c->SCL.PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(pI2c->SCL.GPIOx, &GPIO_InitStructure);	
	
	if( true == pI2c->isUseWP )
	{
 	   RCC_AHB1PeriphClockCmd(pI2c->WP.GPIOx_RCC, ENABLE);	
		GPIO_InitStructure.GPIO_Pin = pI2c->WP.PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(pI2c->WP.GPIOx, &GPIO_InitStructure);
	}
}


/******************************************************************************
* @函 数 名： BspSoftI2c_Init
* @函数描述： 初始化主模式I2C
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/ 
void BspSoftI2c_Init(BspI2c_STRUCT* pI2c)
{
  if(pI2c == NULL )
  {    
    return;
  }
	
	BspSoftI2c_GpioInit(pI2c);
  
  BspSoftI2c_SpeedToDelay(pI2c);
  
  pI2c->ErrCnt = 0;
}


/******************************************************************************
* @函 数 名： BspSoftI2c_Start
* @函数描述： 产生I2c起始信号
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/ 
void BspSoftI2c_Start(BspI2c_STRUCT* pI2c)
{

  BspSoftI2c_SDA_HIGH(pI2c);
  BspSoftI2c_SCL_HIGH(pI2c);
  BspSoftI2c_delay(pI2c->DelayHigh/2);

  BspSoftI2c_SDA_LOW(pI2c);
  BspSoftI2c_delay(pI2c->DelayHigh/2);

  BspSoftI2c_SCL_LOW(pI2c);  //钳住I2C总线，准备发送或接收数据 
  BspSoftI2c_delay(pI2c->DelayLow);   
}	  


/******************************************************************************
* @函 数 名： BspSoftI2c_Stop
* @函数描述： 产生IIC停止信号
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/ 
void BspSoftI2c_Stop(BspI2c_STRUCT* pI2c)
{
  BspSoftI2c_SCL_LOW(pI2c);
  BspSoftI2c_SDA_LOW(pI2c); //避免变成start信号
  BspSoftI2c_delay(pI2c->DelayLow);

  BspSoftI2c_SCL_HIGH(pI2c);
  BspSoftI2c_delay(pI2c->DelayHigh/2);

  BspSoftI2c_SDA_HIGH(pI2c);
  BspSoftI2c_delay(pI2c->DelayHigh/2);    
}


/******************************************************************************
* @函 数 名： BspSoftI2c_WaitAck
* @函数描述： 等待应答信号到来
* @参    数： 
* @返 回 值： 接收应答成功0, 接收应答失败1
* @备    注：
******************************************************************************/ 
u8 BspSoftI2c_WaitAck(BspI2c_STRUCT* pI2c)
{ 
  u8 result = BSP_I2C_OK; 
  u16 ucErrTime = 0;

  BspSoftI2c_SCL_LOW(pI2c);   
  BspSoftI2c_SDA_HIGH(pI2c);   
  BspSoftI2c_delay(pI2c->DelayLow);

  BspSoftI2c_SCL_HIGH(pI2c);
  BspSoftI2c_delay(pI2c->DelayLow);

  while(BspSoftI2c_SDA_READ(pI2c))
  {
    ucErrTime++;
    if(ucErrTime > 500)
    {
      result = BSP_I2C_ERR;
      break;
    }
  }

  BspSoftI2c_SCL_HIGH(pI2c);
  BspSoftI2c_SCL_LOW(pI2c);
  BspSoftI2c_delay(pI2c->DelayLow);

  return (result);  
 
} 


/******************************************************************************
* @函 数 名： BspSoftI2c_Ack
* @函数描述： 产生ACK应答
* @参    数： 
* @返 回 值： 
* @备    注：SDA = 0 为应答信号
*****************************************************************************/ 
void BspSoftI2c_Ack(BspI2c_STRUCT* pI2c)
{  
  BspSoftI2c_SDA_LOW(pI2c);
  BspSoftI2c_delay(pI2c->DelayLow);
  
  BspSoftI2c_SCL_HIGH(pI2c);
  BspSoftI2c_delay(pI2c->DelayHigh);

  BspSoftI2c_SCL_LOW(pI2c);
  BspSoftI2c_delay(pI2c->DelayLow);
}


/******************************************************************************
* @函 数 名： BspSoftI2c_NoAck
* @函数描述： 不产生ACK应
* @参    数： 
* @返 回 值： 
* @备    注：SDA = 1 为非应答信号
******************************************************************************/ 
void BspSoftI2c_NoAck(BspI2c_STRUCT* pI2c)
{
  BspSoftI2c_SDA_HIGH(pI2c);
  BspSoftI2c_delay(pI2c->DelayLow);

  BspSoftI2c_SCL_HIGH(pI2c);
  BspSoftI2c_delay(pI2c->DelayHigh);

  BspSoftI2c_SCL_LOW(pI2c);
  BspSoftI2c_delay(pI2c->DelayLow);
}					 				     

/******************************************************************************
* @函 数 名： BspSoftI2c_SendByte
* @函数描述： I2C发送一个字节
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/
bool BspSoftI2c_SendByte(BspI2c_STRUCT* pI2c, u8 data)
{  
  u8 i = 0;
  u8 SendByte = data;
  u8 result = BSP_I2C_OK; 

  BspSoftI2c_SCL_LOW(pI2c);
  for(i = 0; i < 8; i++)
  {  
    if(SendByte&0x80)
    {
     BspSoftI2c_SDA_HIGH(pI2c);
    }
    else
    {
     BspSoftI2c_SDA_LOW(pI2c);
    } 

    SendByte <<= 1;
    BspSoftI2c_delay(pI2c->DelayLow);
    //数据建立保持一定延时----

    //产生一个上升沿[正脉冲] 
    BspSoftI2c_SCL_HIGH(pI2c);
    BspSoftI2c_delay(pI2c->DelayHigh); 

    BspSoftI2c_SCL_LOW(pI2c);
    BspSoftI2c_delay(pI2c->DelayLow);//延时,防止SCL还没变成低时改变SDA,从而产生START/STOP信号
  }
 
  //接收从机的应答 
  result = BspSoftI2c_WaitAck(pI2c);
  if( result == BSP_I2C_ERR )
  {
    pI2c->ErrCnt++;
    BspSoftI2c_Stop(pI2c);
    return false;
  }

  return true; 
} 	    

/******************************************************************************
* @函 数 名： BspSoftI2c_ReadByte
* @函数描述： 读1个字节，ack=1时，发送ACK，ack=0，发送nACK  
* @参    数： 
* @返 回 值： 
* @备    注：
******************************************************************************/
u8 BspSoftI2c_ReadByte(BspI2c_STRUCT* pI2c, u8 Ack)
{
  u8 i = 8;
  u8 RecvByte = 0;

  BspSoftI2c_SDA_HIGH(pI2c);
  BspSoftI2c_SCL_LOW(pI2c);

  for(i = 0; i < 8; i++)
  {
    BspSoftI2c_SCL_HIGH(pI2c);//产生时钟上升沿[正脉冲],让从机准备好数据 
    BspSoftI2c_delay(pI2c->DelayHigh); 
    
    RecvByte <<= 1;
    if(BspSoftI2c_SDA_READ(pI2c)) //读引脚状态
    {
      RecvByte |= 0x01; 
    } 
    
    BspSoftI2c_SCL_LOW(pI2c);//准备好再次接收数据  
    BspSoftI2c_delay(pI2c->DelayLow); //等待数据准备好         
  }  
   
  if (Ack)
  {
      BspSoftI2c_Ack(pI2c);  //发送ACK
  }
  else
  {
      BspSoftI2c_NoAck(pI2c);   //发送NACK  
  } 

  return RecvByte;       
}



///******************************************************************************
//* @函 数 名： BspSoftI2c_WriteAddr
//* @函数描述： i2c写地址
//* @参    数： 
//* @返 回 值： 
//* @备    注：
//******************************************************************************/ 
//static inline void BspSoftI2c_WriteAddr(BspI2c_STRUCT* pI2c, u32 devAddr, u32 addr)
//{ 
//  if( pI2c->AddrSize == BSP_ADDR_SIZE_16BIT )
//  {
//    BspSoftI2c_SendByte(pI2c, (devAddr & BSP_I2C_WRITE_MASK));	   //发送写命令
//    BspSoftI2c_SendByte(pI2c, U16_MSB(addr));//发送高地址	    
//  }
//  else 
//  {
//    BspSoftI2c_SendByte(pI2c, (devAddr + ( U16_MSB(addr) << 1 ) ) );   //发送器件地址0xA0+ 段地址	 
//  }

//  BspSoftI2c_SendByte(pI2c, U16_LSB(addr));   //发送低地址     
//}



///******************************************************************************
//* @函 数 名： BspSoftI2c_WriteBlock
//* @函数描述： 在指定地址写入指定个数的数据，最多写一页数据
//* @参    数： WriteAddr :开始写入的地址 
//              pBuffer   :数据数组首地址
//              NumToWrite:要写入数据的个数
//* @返 回 值： 
//* @备    注：
//******************************************************************************/ 
//void BspSoftI2c_WriteBlock(BspI2c_STRUCT* pI2c, u32 devAddr, u32 WriteAddr, u8 *pBuffer, u32 NumToWrite)
//{
//  u32 i;

//  BspSoftI2c_Start(pI2c);
//  
//  BspSoftI2c_WriteAddr(pI2c, devAddr, WriteAddr); // 写地址

//  for( i = 0; i < NumToWrite; i++)
//  {
//    BspSoftI2c_SendByte(pI2c, pBuffer[i]);     //发送字节	
//  }     
//  
//  BspSoftI2c_Stop(pI2c);    //产生一个停止条件    
//}

///******************************************************************************
//* @函 数 名： BspSoftI2c_ReadBlock
//* @函数描述： 在指定地址开始读出指定个数的数据
//* @参    数： ReadAddr :开始读出的地址
//              pBuffer   :数据数组首地址
//              NumToRead:要读出数据的个数
//* @返 回 值： 
//* @备    注：
//******************************************************************************/ 
//void BspSoftI2c_ReadBlock(BspI2c_STRUCT* pI2c, u32 devAddr, u32 ReadAddr, u8 *pBuffer, u32 NumToRead)
//{
//  u32 i = 0;
//  
//  BspSoftI2c_Start(pI2c);                      // 产生起始信号
//  BspSoftI2c_WriteAddr(pI2c, devAddr, ReadAddr);     // 写地址

//  BspSoftI2c_Start(pI2c);  	 	   
//  BspSoftI2c_SendByte(pI2c, (devAddr | BSP_I2C_READ_MASK));  //进入接收模式			   
//   
//  for( i = 0; i < (NumToRead - 1); i++)
//  {      
//    pBuffer[i] = BspSoftI2c_ReadByte(pI2c, BSP_I2C_ACK);
//  }

//  pBuffer[i] = BspSoftI2c_ReadByte(pI2c, BSP_I2C_NO_ACK);

//  BspSoftI2c_Stop(pI2c); 
//  
//}  



//u8 BspSoftI2c_Receive(void* pI2c, u8 devAddr,u8 *pBuffer, u16 len)
//{
//  u32 i = 0;
//  
//  BspSoftI2c_Start((BspI2c_STRUCT*)pI2c);                      // 产生起始信号
//  
//  BspSoftI2c_SendByte((BspI2c_STRUCT*)pI2c, (devAddr | BSP_I2C_READ_MASK));	   //发送读命令		   
//   
//  for( i = 0; i < len; i++)
//  {      
//    pBuffer[i] = BspSoftI2c_ReadByte((BspI2c_STRUCT*)pI2c, BSP_I2C_ACK);
//  }

//  BspSoftI2c_Stop((BspI2c_STRUCT*)pI2c);  

//  return 0;   
//}  

//u8 BspSoftI2c_Transmit(void* pI2c, u8 devAddr, u8 *pBuffer, u16 len)
//{
//  u32 i = 0;
//  
//  BspSoftI2c_Start((BspI2c_STRUCT*)pI2c);                      // 产生起始信号
//  BspSoftI2c_SendByte((BspI2c_STRUCT*)pI2c, (devAddr & BSP_I2C_WRITE_MASK));	   //发送读命令
// 		   
//   
//  for( i = 0; i < len; i++)
//  {      
//    BspSoftI2c_SendByte((BspI2c_STRUCT*)pI2c,pBuffer[i]);
//  }

//  BspSoftI2c_Stop((BspI2c_STRUCT*)pI2c);  

//  return 0;   
//}  



/*********END OF FILE****/

