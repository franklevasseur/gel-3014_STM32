#include "motor_control_process.h"

#define MAX_SPEED 10400
#define MIN_SPEED 1000
#define DEBUG_MOTOR 1
#define TRANS_UART 0
#define REDUCE_SPEED 1

double commands[4] = {0, 0, 0, 0};
double references[4] = {0, 0, 0, 0};
double kp = 1;
double ki = 2;
float k_reduce_speed = 0.5;
double cumulativesErr[4] = {0, 0, 0, 0};
uint32_t totalTick[4] = {0, 0, 0, 0};
uint32_t maxTicks[4] = {-1, -1, -1, -1};
int actions[4] = {1, 1, 1, 1};
uint32_t sampling_frequency;

void absoluteCounts(int32_t counts[]);
void blockAllWheels();
void reduceSpeed(uint8_t motor, int32_t current_speed);

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

	while(1) {

    	char nextByte = readInstructionBuffer();
    	if (nextByte != 0xFF) {
    		InstructionContainer container;
    		memset(&container, 0, sizeof(container));

    		if (parseInstruction(nextByte, &container)) {
    			if (container.instructionType == INSTRUCTION_BLOCK_ALL) {
    				blockAllWheels();
    			}
    			else if (container.instructionType == INSTRUCTION_SINGLE) {
    				uint16_t motor = container.singleWheelInstruction.currentMotor;
					uint16_t action = container.singleWheelInstruction.currentAction;
					uint16_t reference = container.singleWheelInstruction.currentReference;
					uint16_t max_tick = container.singleWheelInstruction.currentMaxTick;

					int motor_index = (int) motor;
					references[motor_index - 1] = reference;
					maxTicks[motor_index - 1] = max_tick;
					totalTick[motor_index - 1] = 0;

					int current_action = actions[motor_index - 1];
					if (action != current_action) {
						cumulativesErr[motor_index - 1] = 0;
					}
					actions[motor_index - 1] = action;
					set_motor_action(motor_index, action);
    			}
    			else if (container.instructionType == INSTRUCTION_ALLWHEELS) {
    				for (int motor = 0; motor < 4; motor++) {
    					int new_action = container.allWheelInstruction.actions[motor];

    					if (new_action != actions[motor]) {
    						cumulativesErr[motor] = 0;
    					}
    					actions[motor] = new_action;
    					set_motor_action(motor + 1, new_action);
    					references[motor] = container.allWheelInstruction.speeds[motor];
    					maxTicks[motor] = container.allWheelInstruction.max_ticks[motor];
    					totalTick[motor] = 0;
    				}
    			}
    		}
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
			blockAllWheels();
		} else {
			int32_t ticksPerSecond = counts[motor] * sampling_frequency;
			if (REDUCE_SPEED) {
				reduceSpeed(motor, ticksPerSecond);
			}
			int32_t residual = references[motor] - ticksPerSecond;
			cumulativesErr[motor] += (double)residual / sampling_frequency;
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

void absoluteCounts(int32_t counts[]) {
	for (int motor = 0; motor < 4; motor++) {
		if (counts[motor] < 0) {
			counts[motor] = (-1) * counts[motor];
		}
	}
}

void blockAllWheels() {
	for (int motor = 0; motor < 4; motor++) {
		set_motor_action(motor + 1, BLOCK);
		configure_pwm_ccr(motor + 1, 0);
		actions[motor] = BLOCK;
		references[motor] = 0;
		cumulativesErr[motor] = 0;
		totalTick[motor] = 0;
	}
}

void reduceSpeed(uint8_t motor, int32_t current_speed) {
	if (totalTick[motor] >= (maxTicks[motor] / 2)) {
		uint32_t distance_left = maxTicks[motor] - totalTick[motor];
		uint32_t delta_speed = k_reduce_speed * (current_speed * current_speed) / (distance_left * sampling_frequency);
		references[motor] -= delta_speed;
		if(references[motor] < MIN_SPEED) {
			references[motor] = MIN_SPEED;
		}
	}
}
