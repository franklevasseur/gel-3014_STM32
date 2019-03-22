/*
 * all_wheels_instruction.c
 *
 *  Created on: 19 mars 2019
 *      Author: francois
 */

#include "all_wheels_instruction.h"

#define ALL_WHEELS_CHECKSUM 128

char currentMotorIndex = 0;

char current_actions[4] = {0};

int isSpeedFirstByteIn[4] = {0};
char speedFirstBytes[4] = {0};

int isSpeedSecondByteIn[4] = {0};
char speedSecondBytes[4] = {0};

int isTickFirstByteIn[4] = {0};
char ticksFirstBytes[4] = {0};

int isTickSecondByteIn[4] = {0};
char ticksSecondBytes[4] = {0};

char checksum = 0;

void all_wheels_erase_all() {
	memset(current_actions, 0, 4 * sizeof(char));

	memset(isSpeedFirstByteIn, 0, 4 * sizeof(int));
	memset(speedFirstBytes, 0, 4 * sizeof(char));

	memset(isSpeedSecondByteIn, 0, 4 * sizeof(int));
	memset(speedSecondBytes, 0, 4 * sizeof(char));

	memset(isTickFirstByteIn, 0, 4 * sizeof(int));
	memset(ticksFirstBytes, 0, 4 * sizeof(char));

	memset(isTickSecondByteIn, 0, 4 * sizeof(int));
	memset(ticksSecondBytes, 0, 4 * sizeof(char));

	checksum = 0;
	currentMotorIndex = 0;
}

int all_wheels_is_valid_action(char byte) {
	return byte == 0x01 | byte == 0x02 | byte == 0x03;
}

int all_wheels_is_valid_checksum(char byte) {
	uint16_t sum = 0;
	for (int motor = 0; motor < 4; motor++) {
		sum += current_actions[motor];
		sum += speedFirstBytes[motor];
		sum += speedSecondBytes[motor];
		sum += ticksFirstBytes[motor];
		sum += ticksSecondBytes[motor];
	}
	sum += byte;

	return (sum % ALL_WHEELS_CHECKSUM == 0);
}

int all_wheels_handle_next_byte(char byte, AllWheelsInstruction * instruction) {

	if (current_actions[currentMotorIndex] == '\0') {
		if (all_wheels_is_valid_action(byte)) {
			current_actions[currentMotorIndex] = byte;
			return 0;
		}
		all_wheels_erase_all();
		return -1;
	}

	if (!isSpeedFirstByteIn[currentMotorIndex]) {
		isSpeedFirstByteIn[currentMotorIndex] = 1;
		speedFirstBytes[currentMotorIndex] = byte;
		return 0;
	}

	if (!isSpeedSecondByteIn[currentMotorIndex]) {
		isSpeedSecondByteIn[currentMotorIndex] = 1;
		speedSecondBytes[currentMotorIndex] = byte;
		return 0;
	}

	if (!isTickFirstByteIn[currentMotorIndex]) {
		isTickFirstByteIn[currentMotorIndex] = 1;
		ticksFirstBytes[currentMotorIndex] = byte;
		return 0;
	}

	if (!isTickSecondByteIn[currentMotorIndex]) {
		isTickSecondByteIn[currentMotorIndex] = 1;
		ticksSecondBytes[currentMotorIndex] = byte;

		if (currentMotorIndex < 3) {
			currentMotorIndex++;
		}

		return 0;
	}

	if (all_wheels_is_valid_checksum(byte)) {

		for (int motor = 0; motor < 4; motor++) {
			instruction->actions[motor] = (uint16_t)current_actions[motor];
			instruction->speeds[motor] = (uint16_t)speedFirstBytes[motor] << 8 | speedSecondBytes[motor];
			instruction->max_ticks[motor] = (uint16_t)ticksFirstBytes[motor] << 8 | ticksSecondBytes[motor];
			instruction->checksum = byte;
		}
		all_wheels_erase_all();
		return 1;
	}

	all_wheels_erase_all();
	return -1;
}


