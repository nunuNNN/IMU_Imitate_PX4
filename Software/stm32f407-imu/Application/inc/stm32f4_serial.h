#ifndef _STM32F4_SERIAL_H
#define _STM32F4_SERIAL_H

#include "stm32f4xx.h"

#define PACKET_LENGTH (46)

//low function
void Serial_Init(void);
void Serial_SendByte(uint8_t byte);
void Serial_SendBytes(uint8_t* buffer, uint8_t length);

//
void Serial_Upload(short accel[3], short gyro[3], short compass[3], long quat[4], long temperature, long pressure);

#endif
