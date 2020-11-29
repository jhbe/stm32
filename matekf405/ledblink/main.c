#include "stm32f4xx.h"
#include "../common/delay.h"

#define  RED_LED_PIN  GPIO_Pin_14
#define BLUE_LED_PIN  GPIO_Pin_9
#define  RED_LED_PORT GPIOA
#define BLUE_LED_PORT GPIOB

int main(void) {
    /*
     * Enable PORTA. It is on the AHB1 bus.
     */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /*
     * Enable PORTB. It is on the AHB1 bus.
     */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /*
     * Initialize two pins driving the LEDs.
     */
    GPIO_InitTypeDef gis;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_100MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Pin = RED_LED_PIN;
    GPIO_Init(RED_LED_PORT, &gis);
    gis.GPIO_Pin = BLUE_LED_PIN;
    GPIO_Init(BLUE_LED_PORT, &gis);

    DelayInit();

    while (1) {
        /*
         * Setting a pin turns the LED off. Resetting it turns the LED on.
         */
        GPIO_SetBits(RED_LED_PORT, RED_LED_PIN);
        GPIO_ResetBits(BLUE_LED_PORT, BLUE_LED_PIN);
        DelayMs(100);

        GPIO_ResetBits(RED_LED_PORT, RED_LED_PIN);
        GPIO_SetBits(BLUE_LED_PORT, BLUE_LED_PIN);
        DelayMs(300);
    }
}

void assert_failed(uint8_t *file, uint32_t line) {
    while (1);
}
