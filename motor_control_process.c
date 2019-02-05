#include "motor_control_process.h"

#define MAX_SPEED 2000

uint16_t commands[4] = {0, 0, 0, 0};
uint32_t references[4] = {0, 0, 0, 0};
uint32_t kp = 1;
uint32_t ki = 1;
int32_t cumulativesErr[4] = {0};
uint32_t sampling_frequency;

void do_motor_control(uint32_t p_sampling_frequency) {

	sampling_frequency = p_sampling_frequency;

	while(1) {
    	char nextByte = readInstructionBuffer();
    	if (nextByte != '\0') {
    		char currentInstruction[4] = {'\0'};
			if (parseInstruction(nextByte, currentInstruction)) {
				char motor = currentInstruction[0];
				char action = currentInstruction[1];
				char reference = currentInstruction[2];
				char checkSum = currentInstruction[3];

				int motor_index = (int) motor;
				references[motor_index - 1] = (uint32_t) reference;

				int action_type;
				if (action == 0x0) {
					action_type = BLOCK;
				} else if (action == 0x1) {
					action_type = CW;
				} else if (action == 0x2) {
					action_type = CCW;
				}
				set_motor_action(motor_index, action_type);
			}
    	}
	}
}

void controlMotors(void)
{
	int32_t counts[4] = {0};
	getCounts(counts);

	for (uint8_t i = 0; i < 4; i++) {
		uint32_t ticksPerSecond = counts[i] * sampling_frequency;

		int32_t err = references[i] - ticksPerSecond;
		cumulativesErr[i] += err;
		int32_t correction = (err * kp) + (cumulativesErr[i] * ki);

		commands[i] += correction;

		if (commands[i] > MAX_SPEED) {
			commands[i] = MAX_SPEED;
		} else if (commands[i] < 0) {
			commands[i] = 0;
		}

		uint16_t ccr = (commands[i] / MAX_SPEED) * 1000;
		configure_pwm_ccr(i, ccr);
	}

	resetCounts();
}
