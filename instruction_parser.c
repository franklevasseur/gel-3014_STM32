#include "instruction_parser.h"

int instructionHasStarted = 0;
int instructionType = 0;
int waitingForEndingByte = 0;

SingleWheelInstruction currentSingleWheelInstruction = {0};
AllWheelsInstruction currentAllWheelsInstruction = {0};

int isValidFirstByte(char byte) {
	return byte == '$'; // Make it RAINNNNNNN
}

int isValidType(char byte) {
	return byte == 0x01 || byte == 0x02 || byte == 0x03;
}

int isValidEndingByte(char byte) {
	return byte == '.';
}

void eraseAll() {
	instructionHasStarted = 0;
	instructionType = 0;
	waitingForEndingByte = 0;
	memset(&currentSingleWheelInstruction, 0, sizeof(currentSingleWheelInstruction));
	memset(&currentAllWheelsInstruction, 0, sizeof(currentAllWheelsInstruction));
}

int parseInstruction(char byte, InstructionContainer * fullInstruction) {

	if (!instructionHasStarted) {
		if (isValidFirstByte(byte)) {
			instructionHasStarted = 1;
		} else {
			eraseAll();
			uart_write(0x0);
		}
		return 0;
	}

	if (!instructionType) {
		if(isValidType(byte)) {
			instructionType = byte;
		} else {
			eraseAll();
			uart_write(0x0);
		}
		return 0;
	}

	if (instructionType == 1) {
		if (isValidEndingByte(byte)) {
			fullInstruction -> instructionType = 1;
			eraseAll();
			uart_write(0x1); // On renvoit un ackknowledge quand on a reçu la commande pour break
			return 1;
		}
		eraseAll();
		uart_write(0x0);
		return 0;
	}

	if (instructionType == 2) {
		if (waitingForEndingByte) {
			if (isValidEndingByte(byte)) {
				memcpy(&(fullInstruction -> singleWheelInstruction),
						&currentSingleWheelInstruction, sizeof(currentSingleWheelInstruction));
				fullInstruction -> instructionType = 2;
				eraseAll();
				uart_write(0x0);
				return 1;
			}

			eraseAll();
			uart_write(0x0);
			return 0;
		}

		int status = single_wheel_handle_next_byte(byte, &currentSingleWheelInstruction);
		if (status == -1) {
			eraseAll();
			uart_write(0x0);
		} else if (status == 1) {
			waitingForEndingByte = 1;
		}
		return 0;
	}

	if (instructionType == 3) {
		if (waitingForEndingByte) {
			if (isValidEndingByte(byte)) {
				memcpy(&(fullInstruction -> allWheelInstruction),
						&currentAllWheelsInstruction, sizeof(currentAllWheelsInstruction));
				fullInstruction -> instructionType = 3;
				eraseAll();
				uart_write(0x0);
				return 1;
			}

			eraseAll();
			uart_write(0x0);
			return 0;
		}

		int status = all_wheels_handle_next_byte(byte, &currentAllWheelsInstruction);
		if (status == -1) {
			eraseAll();
			uart_write(0x0);
		} else if (status == 1) {
			waitingForEndingByte = 1;
		}
		return 0;
	}
}
