/********************************************************/
//header
#include "stm32f4_rcc.h"
#include "stm32f4_delay.h"
#include "stm32f4_serial.h"
/********************************************************/
//data
/********************************************************/
int main(void)
{
	s16 s16Gyro[3] = {0, 1, 2}, s16Accel[3] = {3, 4, 5}, s16Mag[3] = {6, 7, 8};
	long lQuat[4] = {0, 1, 2, 3};
	s32 s32Temperature = 7, s32Pressure = 9;
	
	Serial_Init();
	
	
	Serial_Upload(s16Accel, s16Gyro, s16Mag, lQuat, s32Temperature, s32Pressure);
	while(1);
}
