#include "stm32f103xb.h"

#define SYSTICK_LOAD_VAL 8000 // 1ms delay at 8MHz
#define CTRL_ENABLE     (1 << 0)
#define CTRL_CLK_SRC    (1 << 2)
#define CTRL_COUNT_FLAG (1 << 16)

void systickDelayMs(int delay)
{
    //Reload with number of clocks per millisecond
    SysTick->LOAD = SYSTICK_LOAD_VAL - 1; // Set reload value for 1ms

    //Clear SysTick current value
    SysTick->VAL = 0;

    //Enable SysTick and select internal clock
    SysTick->CTRL = CTRL_ENABLE | CTRL_CLK_SRC;

    for ( int i = 0; i < delay; i++)
    {
        //wait until COUNTFLAG is set
        while ((SysTick->CTRL & CTRL_COUNT_FLAG) == 0);
    }

    //Disable SysTick
    SysTick->CTRL = 0;

}