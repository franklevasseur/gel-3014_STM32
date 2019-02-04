#include "instruction_parser.h"

char currentMotor = '\0';
char currentAction = '\0';
char currentReference = '\0';
char currentChecksum = '\0';

int isValidMotor(char byte) {
	return byte == 0x01 || byte == 0x02 || byte == 0x03 || byte == 0x04;
}

int isValidAction(char byte) {
	return byte == 0x01 || byte == 0x02 || byte == 0x03;
}

int isValidRef(char byte) {
	return 1;
}

int isValidChecksum(char byte) {
	int sum = (currentMotor + currentAction + currentReference + byte) % 256;
	return sum == 0;
}

void eraseAll() {
	currentMotor = '\0';
	currentAction = '\0';
	currentReference = '\0';
	currentChecksum = '\0';
}

int parseInstruction(char byte, char fullInstruction[4]) {

	if (currentMotor == '\0' && isValidMotor(byte)) {
		currentMotor = byte;
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

	if (currentReference == '\0') {

		if (isValidRef(byte)) {
			currentReference = byte;
		} else {
			eraseAll();
		}

		return 0;
	}

	if (isValidChecksum(byte)) {
		currentChecksum = byte;

		fullInstruction[0] = currentMotor;
		fullInstruction[1] = currentAction;
		fullInstruction[2] = currentReference;
		fullInstruction[3] = currentChecksum;

		eraseAll();
		return 1;
	}

	eraseAll();
	return 0;
}
