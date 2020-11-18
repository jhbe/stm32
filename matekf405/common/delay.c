#include "stm32f4xx.h"

static __IO uint32_t s_ticks;

void Delay(int x) {
    s_ticks = x;
    while (s_ticks > 0);
}

void SysTick_Handler(void) {
    if (s_ticks > 0) {
        s_ticks--;
    }
}
