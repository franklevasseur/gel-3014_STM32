#include "pwm_module.h"

// La fréquence du pwm se calcul ainsi:
// f (Hz) = 84MHz / ((prescaler + 1) * (period + 1))

// Le duty cycle du pwm se calcul ainsi:
// Duty (%) = (ccr / period) * 100

// Le PWM est branché sur la pin PB14

TIM_TypeDef* TIMX = TIM12;
GPIO_TypeDef* GPIOX = GPIOB;

uint16_t GPIO_PinSourceX = GPIO_PinSource14;
uint32_t GPIO_Pin_X = GPIO_Pin_14;

uint32_t TIM_CLCK = RCC_APB1Periph_TIM12;
uint32_t GPIO_CLCK = RCC_AHB1Periph_GPIOB;

uint8_t TIM_AF = GPIO_AF_TIM12;

uint16_t TIM_OCMode_PWMX = TIM_OCMode_PWM2;

void configure_timer_af(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(TIM_CLCK, ENABLE);

	/* GPIOC clock enable */
	RCC_AHB1PeriphClockCmd(GPIO_CLCK, ENABLE);

	/* GPIOC Configuration: TIM3 CH1 (PC6) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_X;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOX, &GPIO_InitStructure);

	/* Connect TIM3 pins to AF2 */
	GPIO_PinAFConfig(GPIOX, GPIO_PinSourceX, TIM_AF);
}

void configure_timer_freq(uint16_t prescaler, uint16_t period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIMX, &TIM_TimeBaseStructure);
}

void configure_timer_pwm(uint16_t ccr)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWMX;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIMX, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIMX, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIMX, ENABLE);
}

void init_pwm(uint16_t prescaler, uint16_t period, uint16_t ccr)
{
	configure_timer_af();
	configure_timer_freq(prescaler, period);
	configure_timer_pwm(ccr);

	TIM_Cmd(TIMX, ENABLE);
}
