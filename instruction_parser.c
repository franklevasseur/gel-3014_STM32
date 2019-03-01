#include "instruction_parser.h"

char currentMotor = '\0';
char currentAction = '\0';

char currentReferenceFirstByte = '\0';
int isRefFirstByteIn = 0;
char currentReferenceSecondByte = '\0';
uint16_t currentReference = '\0';
int isRefIn = 0;

char currentMaxTickFirstByte = '\0';
int isMaxTickFirstByteIn = 0;
char currentMaxtTickSecondByte = '\0';
uint16_t currentMaxTick = '\0';
int isMaxTickIn = 0;

char currentChecksum = '\0';

int isValidMotor(char byte) {
	return byte == 0x01 || byte == 0x02 || byte == 0x03 || byte == 0x04;
}

int isValidAction(char byte) {
	return byte == 0x01 || byte == 0x02 || byte == 0x03;
}

int isValidRef(uint16_t ref) {
	return 1;
}

int isValidTickMax(uint16_t max_tick) {
	return 1;
}

int isValidChecksum(char byte) {
	int sum = (currentMotor
			+ currentAction
			+ currentReferenceFirstByte
			+ currentReferenceSecondByte
			+ currentMaxTickFirstByte
			+ currentMaxtTickSecondByte
			+ byte) % 256;
	return sum == 0;
}

void eraseAll() {
	currentMotor = 0;
	currentAction = 0;

	currentReferenceFirstByte = 0;
	currentReferenceSecondByte = 0;
	isRefFirstByteIn = 0;
	currentReference = 0;
	isRefIn = 0;

	currentMaxTickFirstByte = 0;
	currentMaxtTickSecondByte = 0;
	isMaxTickFirstByteIn = 0;
	currentMaxTick = 0;
	isMaxTickIn = 0;

	currentChecksum = 0;
}

int parseInstruction(char byte, uint16_t fullInstruction[5]) {

	if (currentMotor == '\0') {

		if (isValidMotor(byte)) {
			currentMotor = byte;
		} else {
			eraseAll();
		}
		return 0;
	}

	if (currentAction == '\0') {

		if (isValidAction(byte)) {
			currentAction = byte;
		} else {
			eraseAll();
		}

		return 0;
	}

	if (!isRefFirstByteIn) {
		isRefFirstByteIn = 1;
		currentReferenceFirstByte = byte;
		return 0;
	}

	if (!isRefIn) {
		uint16_t ref = (uint16_t)currentReferenceFirstByte << 8 | byte;
		if (isValidRef(ref)) {
			isRefIn = 1;
			currentReferenceSecondByte = byte;
			currentReference = ref;
		} else {
			eraseAll();
		}
		return 0;
	}

	if (!isMaxTickFirstByteIn) {
		isMaxTickFirstByteIn = 1;
		currentMaxTickFirstByte = byte;
		return 0;
	}

	if (!isMaxTickIn) {
		uint16_t max_tick = (uint16_t)currentMaxTickFirstByte << 8 | byte;
		if (isValidTickMax(max_tick)) {
			isMaxTickIn = 1;
			currentMaxtTickSecondByte = byte;
			currentMaxTick = max_tick;
		} else {
			eraseAll();
		}
		return 0;
	}

	if (isValidChecksum(byte)) {
		currentChecksum = byte;

		fullInstruction[0] = (uint16_t)currentMotor;
		fullInstruction[1] = (uint16_t)currentAction;
		fullInstruction[2] = (uint16_t)currentReference;
		fullInstruction[3] = (uint16_t)currentMaxTick;
		fullInstruction[4] = (uint16_t)currentChecksum;

		eraseAll();
		return 1;
	}

	eraseAll();
	return 0;
}
