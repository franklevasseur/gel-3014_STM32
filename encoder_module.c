#include "encoder_module.h"

uint32_t initialCount = 0x7FFF;

// moteur 1: PE9, PE11
// moteur 2: PA5, PB3
// moteur 3: PC6, PB5
// moteur 4; PD12, PD13

void init_timer(TIM_TypeDef* TIMX);

void init_encoders()
{
	// les clocks des GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	// les clocks des timers
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,  ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,  ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,  ENABLE);

	// les AFs
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5,  GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,  GPIO_AF_TIM2);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,  GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,  GPIO_AF_TIM3);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);

	// Les GPIOs
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	// Setter les encodeurs
	init_timer(TIM1);
	init_timer(TIM2);
	init_timer(TIM3);
	init_timer(TIM4);
}

void init_timer(TIM_TypeDef* TIMX)
{
	TIM_EncoderInterfaceConfig(TIMX,
							   TIM_EncoderMode_TI12,
							   TIM_ICPolarity_Rising,
							   TIM_ICPolarity_Rising);
	TIM_SetAutoreload(TIMX, 0xffff);
	TIMX -> CNT = initialCount;
	TIM_Cmd(TIMX, ENABLE);
}

void getCounts(int32_t counts[])
{
//	counts[0] = (TIM1 -> CNT) - initialCount;
//	counts[1] = (TIM2 -> CNT) - initialCount;
//	counts[2] = (TIM3 -> CNT) - initialCount;
//	counts[3] = (TIM4 -> CNT) - initialCount;

	// Le shield est fucké fait qu'on a du refaire la correspondance
	counts[0] = (TIM2 -> CNT) - initialCount;
	counts[1] = (TIM3 -> CNT) - initialCount;
	counts[2] = (TIM4 -> CNT) - initialCount;
	counts[3] = (TIM1 -> CNT) - initialCount;
}

void resetCounts()
{
	TIM1 -> CNT = initialCount;
	TIM2 -> CNT = initialCount;
	TIM3 -> CNT = initialCount;
	TIM4 -> CNT = initialCount;
}
