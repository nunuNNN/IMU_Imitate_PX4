#include "stm32f4_delay.h"

//Cycle per microsecond
static __IO uint32_t us_ticks = 0;
static __IO uint32_t uptime_ticks = 0;
static __IO uint32_t cycle_ticks = 0;

void Delay_Init()
{
	/* 
	*定义RCC时钟频率配置的结构体变量，用于配置RCC。
	* 包括SYSCLK,HCLK,PCLK1,PCLK2,ADCCLK频率的设置
	*/
	RCC_ClocksTypeDef RCC_Clocks; 
	/*
	*获取不同片上时钟的频率，RCC_Clocks为前面定义的时钟频率结构体变量
	*该函数把当前不同片上时钟的频率存放在结构体RCC_Clocks中
	*/
  RCC_GetClocksFreq(&RCC_Clocks);
	
	us_ticks = RCC_Clocks.SYSCLK_Frequency / 1000000u;
	
	//enable DWT access
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	//enable the CPU cycle counter
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	//Reset counter
	DWT->CYCCNT = 0u;		////DWT当前PC采样器周期计数寄存器
	
	//唯一一个需要调用的固件库函数，
	//1us中断一次
	if(SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000u)){
		while (1); // Handle Error
	}
}

__inline uint32_t Millis(void)
{
	return uptime_ticks;
}

u32 Micros(void)
{
	register uint32_t old_cycle, cycle, timeMs;
	
	do{
		timeMs = __LDREXW(&uptime_ticks);
		cycle = DWT->CYCCNT;
		old_cycle = cycle_ticks;		
	}
	while ( __STREXW( timeMs , &uptime_ticks ) );
	return (timeMs * 1000) + (cycle - old_cycle) / us_ticks;
}

void Delay_Ms(u32 ms)
{
	while (ms--){
		Delay_Us(1000);
	}
}

void Delay_Us(u32 us)
{
	uint32_t elapsed = 0;
	uint32_t elapsed_us = 0;
	uint32_t lastCount = DWT->CYCCNT;
	register uint32_t current_count = DWT->CYCCNT;
	
	for(;;) {
		current_count = DWT->CYCCNT;
		elapsed += current_count - lastCount;
		lastCount = current_count;
		
		elapsed_us = elapsed / us_ticks;
		if (elapsed_us >= us){
			break;
		}
		us -= elapsed_us;
		elapsed %= us_ticks;
	}
}

int Get_Ms(unsigned long *count)
{
	count[0] = uptime_ticks;
	return 0;
}

//SysTick_Config中断服务函数，1us中断一次
void SysTick_Handler(void)
{
	cycle_ticks = DWT->CYCCNT;
	uptime_ticks++;
}
