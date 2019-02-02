#include "pwm_module.h"

// La fréquence du pwm se calcul ainsi:
// f (Hz) = 84MHz / ((prescaler + 1) * (period + 1))

// Le duty cycle du pwm se calcul ainsi:
// Duty (%) = (ccr / period) * 100

// Channel1 PA0
// Channel2 PA1
// Channel3 PA2
// Channel4 PA3

TIM_TypeDef* TIMX = TIM5;
GPIO_TypeDef* GPIOX = GPIOA;

uint16_t GPIO_PinSourceX1 = GPIO_PinSource0;
uint32_t GPIO_Pin_X1 = GPIO_Pin_0;

uint16_t GPIO_PinSourceX2 = GPIO_PinSource1;
uint32_t GPIO_Pin_X2 = GPIO_Pin_1;

uint16_t GPIO_PinSourceX3 = GPIO_PinSource2;
uint32_t GPIO_Pin_X3 = GPIO_Pin_2;

uint16_t GPIO_PinSourceX4 = GPIO_PinSource3;
uint32_t GPIO_Pin_X4 = GPIO_Pin_3;

uint32_t TIM_CLCK = RCC_APB1Periph_TIM5;
uint32_t GPIO_CLCK = RCC_AHB1Periph_GPIOA;

uint8_t TIM_AF = GPIO_AF_TIM5;

uint16_t TIM_OCMode_PWMX = TIM_OCMode_PWM1;

void configure_timer_af(void)
{
	RCC_APB1PeriphClockCmd(TIM_CLCK, ENABLE);

	RCC_AHB1PeriphClockCmd(GPIO_CLCK, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_X1 | GPIO_Pin_X2 | GPIO_Pin_X3 | GPIO_Pin_X4;
	GPIO_Init(GPIOX, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOX, GPIO_PinSourceX1, TIM_AF);
	GPIO_PinAFConfig(GPIOX, GPIO_PinSourceX2, TIM_AF);
	GPIO_PinAFConfig(GPIOX, GPIO_PinSourceX3, TIM_AF);
	GPIO_PinAFConfig(GPIOX, GPIO_PinSourceX4, TIM_AF);
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

void configure_timer_pwm(int channel, uint16_t ccr)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWMX;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	if (channel == 1) {
		TIM_OC1Init(TIMX, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMX, TIM_OCPreload_Enable);
	} else if (channel == 2) {
		TIM_OC2Init(TIMX, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMX, TIM_OCPreload_Enable);
	} else if (channel == 3) {
		TIM_OC3Init(TIMX, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMX, TIM_OCPreload_Enable);
	} else if (channel == 4) {
		TIM_OC4Init(TIMX, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMX, TIM_OCPreload_Enable);
	}

	TIM_ARRPreloadConfig(TIMX, ENABLE);
}

void start_pwm(void)
{
	TIM_Cmd(TIMX, ENABLE);
}
