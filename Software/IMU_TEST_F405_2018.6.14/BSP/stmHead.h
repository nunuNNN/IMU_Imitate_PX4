#ifndef __STM_HEAD_H
#define __STM_HEAD_H 	


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


#include "stm32f4xx.h"



#define HDW_F405_COM3  (0)
#define HDW_F405_USB   (1)
#define HDW_F401_USB   (2)
#define HDW_F401_COM2  (3)

#define HARD_WARE_VERSION   (HDW_F401_COM2)


#define U16_MSB(DATA)             ((u8)((u16)(((u16)((DATA) & (u16)(0xFF00))) >> 8)))
#define U16_LSB(DATA)             ((u8)((u16)((DATA) & (u16)(0x00FF))))

/* get fields (little-endian) ------------------------------------------------*/


#define TO_U8(p) 		((vu8)(*((vu8 *)(p))))
#define TO_U16(p) 		((vu16)(*((vu16 *)(p))))
#define TO_U32(p) 		((vu32)(*((vu32 *)(p))))

#define TO_S8(p) 		((vs8)(*((vs8 *)(p))))
#define TO_S16(p) 		((vs16)(*((vs16 *)(p))))
#define TO_S32(p) 		((vs32)(*((vs32 *)(p))))


#define TO_FLOAT(p)		((volatile float)(*((volatile float *)(p))))
#define TO_DOUBLE(p)	((volatile double)(*((volatile double *)(p))))
	
#define U8_MAX      ((uint8_t)255)
#define S8_MAX      ((int8_t)127)
#define S8_MIN      ((int8_t)-127)
#define U16_MAX     ((uint16_t)65535u)
#define S16_MAX     ((int16_t)32767)
#define S16_MIN     ((int16_t)-32767)
#define S16_STD_MIN ((int16_t)-32768)
#define U32_MAX     ((uint32_t)4294967295uL)
#define S32_MAX     ((int32_t)2147483647)
#define S32_MIN     ((int32_t)-2147483647)

typedef struct
{
	GPIO_TypeDef* GPIOx;
	u32 GPIOx_RCC;
	u16	PIN;
	u16 PIN_Source;
	u8 PIN_AF;
}GPIO_STRUCT;


typedef struct
{
	DMA_Stream_TypeDef* DMAy_Streamx;
	u32 DMAx_RCC;
	u32 DMAy_Chx;
	u8  DMAy_Streamx_IRQn;
	u8 IRQ_MainPriority;
	u8 IRQ_SubPriority;
	u16 bufLen;
	u16 dataLen;
	u8* buf;
	bool isUseDmaIsr;
}DMA_STRUCT;

#endif





























