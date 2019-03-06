#include "bridge_module.h"
#include "stm32f4_discovery.h"

// utilise GPIOD avec les pins 1-4 et 6-9

uint32_t allPins = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4
| GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;

GPIO_TypeDef* GPIOY = GPIOD;

typedef struct Motor {
	uint32_t pin1;
	uint32_t pin2;
} Motor;

Motor motors[4];

void init_bridge_pins(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = allPins;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOY, &GPIO_InitStructure);

	GPIO_SetBits(GPIOY, allPins);

	Motor motor_struct;
	motor_struct.pin1 = GPIO_Pin_1;
	motor_struct.pin2 = GPIO_Pin_6;
	motors[0] = motor_struct;

	motor_struct.pin1 = GPIO_Pin_2;
	motor_struct.pin2 = GPIO_Pin_7;
	motors[1] = motor_struct;

	motor_struct.pin1 = GPIO_Pin_8; // ont été inversés pour que ca tourne clockwise
	motor_struct.pin2 = GPIO_Pin_3;
	motors[2] = motor_struct;

	motor_struct.pin1 = GPIO_Pin_9; // ont été inversés pour que ca tourne clockwise
	motor_struct.pin2 = GPIO_Pin_4;
	motors[3] = motor_struct;
}

void set_motor_action(int motor, int action) {
	if (action == BLOCK)
	{
		GPIO_SetBits(GPIOY, motors[motor - 1].pin1);
		GPIO_SetBits(GPIOY, motors[motor - 1].pin2);
	}
	else if (action == CW)
	{
		GPIO_SetBits(GPIOY, motors[motor - 1].pin1);
		GPIO_ResetBits(GPIOY, motors[motor - 1].pin2);
	}
	else if (action == CCW)
	{
		GPIO_ResetBits(GPIOY, motors[motor - 1].pin1);
		GPIO_SetBits(GPIOY, motors[motor - 1].pin2);
	}
}
