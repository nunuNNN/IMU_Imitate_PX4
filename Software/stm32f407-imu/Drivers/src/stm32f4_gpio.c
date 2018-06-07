#include "stm32f4_gpio.h"

void GPIOx_Init(GPIO_Driver* GPIOx)
{
	GPIOx->GPIO_CLK(GPIOx->GPIO_Func, ENABLE);
	GPIO_Init(GPIOx->Gpio, &GPIOx->GPIO_Init);
}
