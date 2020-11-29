#include "stm32f4xx.h"
#include "delay.h"

/*
 * This counter is increased by one eveny milli second.
 */
static __IO uint32_t s_ticks;

/**
 * Initializes the SysTick used for Delay functions.
 */
void DelayInit(void) {
    SysTick_Config(SystemCoreClock / 1000);
}

/**
 * Blocks for a given number of milliseconds.
 *
 * @param ms The number of milliseconds to delay.
 */
void DelayMs(uint32_t ms) {
    s_ticks = ms;
    while (s_ticks > 0);
}

/**
 * SysTick interrupt handler. Will be invoked at 1kHz.
 */
void SysTick_Handler(void) {
    if (s_ticks > 0) {
        s_ticks--;
    }
}
