#include "stm32f4xx.h"
#include "../common/delay.h"

int main(void) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

    GPIO_InitTypeDef gis;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_100MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOA, &gis);
    gis.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &gis);
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOC, &gis);

    TIM_TimeBaseInitTypeDef tis;
    tis.TIM_Prescaler = 40000;
    tis.TIM_CounterMode = TIM_CounterMode_Up;
    tis.TIM_Period = 2000;
    tis.TIM_ClockDivision = TIM_CKD_DIV1;
    tis.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM8, &tis);
    TIM_Cmd(TIM8, ENABLE);

    TIM_OCInitTypeDef oci = {0,};
    oci.TIM_OCMode = TIM_OCMode_PWM2;
    oci.TIM_Pulse = 1500;
    oci.TIM_OutputState = TIM_OutputState_Enable;
    oci.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM8, &oci);
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);

    SysTick_Config(SystemCoreClock / 1000);

    while (1) {
        GPIO_SetBits(GPIOA, GPIO_Pin_14);
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
        Delay(100);
        GPIO_ResetBits(GPIOA, GPIO_Pin_14);
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
        Delay(200);
    }
}

void assert_failed(uint8_t* file, uint32_t line) {

}
