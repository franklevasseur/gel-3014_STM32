#include "motor_control_process.h"

#define MAX_SPEED 10400
#define DEBUG_MOTOR 1
#define TRANS_UART 0
#define DEMO 1

double commands[4] = {0, 0, 0, 0};
double references[4] = {0, 0, 0, 0};
double kp = 0.05;
double ki = 0.4;
double cumulativesErr[4] = {0, 0, 0, 0};
uint32_t totalTick[4] = {0, 0, 0, 0};
uint32_t maxTicks[4] = {-1, -1, -1, -1};
uint32_t sampling_frequency;

void resetTickCounts();

void do_motor_control(uint32_t p_sampling_frequency) {

	// On prend la led bleue (PD15) pour debugger
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_15);

	sampling_frequency = p_sampling_frequency;

	if (DEMO) {
		resetTickCounts();

		// va tout drette
		set_motor_action(1, CW);
		set_motor_action(2, BLOCK);
		set_motor_action(3, CCW);
		set_motor_action(4, BLOCK);
		references[0] = 8000;
		references[1] = 0;
		references[2] = 8000;
		references[3] = 0;
		while(totalTick[2] < 40000);

		resetTickCounts();

		// tourne
		set_motor_action(1, CW);
		set_motor_action(2, CW);
		set_motor_action(3, CW);
		set_motor_action(4, CW);
		references[0] = 6000;
		references[1] = 6000;
		references[2] = 6000;
		references[3] = 6000;
		while(totalTick[2] < 10000);

		resetTickCounts();

		// revient voir papa
		set_motor_action(1, CW);
		set_motor_action(2, BLOCK);
		set_motor_action(3, CCW);
		set_motor_action(4, BLOCK);
		references[0] = 5000;
		references[1] = 0;
		references[2] = 5000;
		references[3] = 0;
		while(totalTick[2] < 40000);

		resetTickCounts();

		set_motor_action(1, CCW);
		set_motor_action(2, CCW);
		set_motor_action(3, CCW);
		set_motor_action(4, CCW);
		references[0] = 6000;
		references[1] = 6000;
		references[2] = 6000;
		references[3] = 6000;
		while(totalTick[2] < 10000);

		set_motor_action(1, BLOCK);
		set_motor_action(2, BLOCK);
		set_motor_action(3, BLOCK);
		set_motor_action(4, BLOCK);
	}

	while(1) {

    	char nextByte = readInstructionBuffer();
    	if (nextByte != 0xFF) {

    		GPIO_ResetBits(GPIOD, GPIO_Pin_15);

    		uint16_t currentInstruction[5] = {'\0'};
			if (parseInstruction(nextByte, currentInstruction)) {
				uint16_t motor = currentInstruction[0];
				uint16_t action = currentInstruction[1];
				uint16_t reference = currentInstruction[2];
				uint16_t max_tick = currentInstruction[3];

				int motor_index = (int) motor;
				references[motor_index - 1] = reference;
				maxTicks[motor_index - 1] = max_tick;

				int action_type;
				if (action == 0x1) {
					action_type = BLOCK;
				} else if (action == 0x2) {
					action_type = CW;
				} else if (action == 0x3) {
					action_type = CCW;
				}
				set_motor_action(motor_index, action_type);
			}
    	}
	}
}

void absoluteCounts(int32_t counts[]) {
	for (int motor = 0; motor < 4; motor++) {
		if (counts[motor] < 0) {
			counts[motor] = (-1) * counts[motor];
		}
	}
}

void controlMotors(void)
{
	int32_t counts[4] = {0};
	getCounts(counts);
	absoluteCounts(counts);

	for (uint8_t motor = 0; motor < 4; motor++) {

		totalTick[motor] += counts[motor];

		if (totalTick[motor] >= maxTicks[motor]) {
			set_motor_action(motor + 1, BLOCK);
		} else {
			int32_t ticksPerSecond = counts[motor] * sampling_frequency;
			int32_t residual = references[motor] - ticksPerSecond;
			cumulativesErr[motor] += residual;
			commands[motor] = (residual * kp) + (cumulativesErr[motor] * ki);

			if (commands[motor] > MAX_SPEED) {
				commands[motor] = MAX_SPEED;
			} else if (commands[motor] < 0) {
				commands[motor] = 0;
			}

			double fuckin_fraction = commands[motor] / MAX_SPEED;
			uint32_t ccr = fuckin_fraction * 1000;

			if (ccr >= 1000) {
				ccr = 999;
			}

			configure_pwm_ccr(motor + 1, ccr);

			if (TRANS_UART && motor + 1 == DEBUG_MOTOR) {
				transmit_sample((int16_t)ticksPerSecond);
				transmit_sample((int16_t)residual);
				transmit_sample((int16_t)commands[DEBUG_MOTOR]);
				transmit_sample((int16_t)ccr);
			}
		}
	}

	resetCounts();
}

void resetTickCounts() {
	totalTick[0] = 0;
	totalTick[1] = 0;
	totalTick[2] = 0;
	totalTick[3] = 0;
}
