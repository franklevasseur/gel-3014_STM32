#include "motor_control_process.h"

uint16_t ccrs[4] = {0, 0, 0, 0};
uint32_t references[4] = {50, 50, 50, 50};
uint32_t kp = 1;
uint32_t ki = 1;
int32_t cumulativesErr[4] = {0};

void do_motor_control(void) {

	while(1) {
    	char nextByte = readInstructionBuffer();
    	if (nextByte != '\0') {
    		char currentInstruction[4] = {'\0'};
			if (parseInstruction(nextByte, currentInstruction)) {
				char motor = currentInstruction[0];
				char action = currentInstruction[1];
				char reference = currentInstruction[2];
				char checkSum = currentInstruction[3];

				// faire une action
			}
    	}

		uart_write(69);
		uart_write(42);
	}
}

void controlMotors(void)
{
	int32_t counts[4] = {0};
	getCounts(counts);

	for (uint8_t i = 0; i < 4; i++) {
		int32_t err = references[i] - counts[i];
		cumulativesErr[i] += err;
		int32_t correction = (err * kp) + (cumulativesErr[i] * ki);

		ccrs[i] += correction;

		if (ccrs[i] > 1000) {
			ccrs[i] = 1000;
		} else if (ccrs[i] < 0) {
			ccrs[i] = 0;
		}

		configure_pwm_ccr(i, ccrs[i]);
	}

	resetCounts();
}
