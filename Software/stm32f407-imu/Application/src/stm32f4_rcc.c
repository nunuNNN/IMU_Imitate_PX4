#include "stm32f4_rcc.h"
#include "stm32f4xx_conf.h"

void RCC_SystemCoreClockUpdate(PLL_PARAMS params)
{
	//复位RCC的所有寄存器
	RCC_DeInit();
	
	//使能HSI
	RCC_HSICmd(ENABLE);
	
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	
	//switch the system clock to HSI
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	while(RCC_GetSYSCLKSource() != 0x00);
	
	//stop PLL, wait until down
	RCC_PLLCmd(DISABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == SET);
	
	//使能HSE
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	
	/*选择电压调节器的模式为1*/
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  PWR->CR |= PWR_CR_VOS;
	//change PLL params;
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div4);
	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_PLLConfig(RCC_PLLSource_HSE, params.PLLM, params.PLLN, params.PLLP, params.PLLQ);
		
	//start PLL, wait until up
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	//switch system clock to PLL, wait until switched
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	
	//update
	SystemCoreClockUpdate();
	
}
