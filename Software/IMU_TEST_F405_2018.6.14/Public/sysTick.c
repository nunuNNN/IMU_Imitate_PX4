#include "sysTick.h"

__IO uint32_t uwTick;


void sysTick_Init(void)
{
    SysTick_Config(SystemCoreClock / 1000);
}


void sysTick_IncTick(void)
{
    uwTick++;
}


uint32_t sysTick_GetTick(void)
{
    return uwTick;
}


void sysTick_DelayMs(__IO uint32_t Delay)
{
    uint32_t tickstart = 0U;
    tickstart = sysTick_GetTick();
    while ((sysTick_GetTick() - tickstart) < Delay)
    {
    }
}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
//  */
void SysTick_Handler(void)
{
    sysTick_IncTick();
}



