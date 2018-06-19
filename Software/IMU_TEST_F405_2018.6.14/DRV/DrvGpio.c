
#include "DrvGpio.h"




#define  LED_PIN        (GPIO_Pin_10)
#define  LED_GPIO_PORT  (GPIOA)
#define  LED_GPIO_RCC   (RCC_AHB1Periph_GPIOA)


void DrvLed_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LED_GPIO_RCC, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	

  GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	
	DrvLed_OFF();
}


void DrvLed_ON(void)
{	
		GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, Bit_SET);
}


void DrvLed_OFF(void)
{
		GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, Bit_RESET);		
}


void DrvLed_Toggle(void)
{
	GPIO_ToggleBits(LED_GPIO_PORT, LED_PIN);	
}
