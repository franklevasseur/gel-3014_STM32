#include "instruction_buffer.h"

#define BUFFERSIZE 40

char instructionsBuffer[BUFFERSIZE] = {-1};
int readIndex = 0;
int writeIndex = 0;

char readInstructionBuffer() {

	if (readIndex != writeIndex) {
		char byte = instructionsBuffer[readIndex];
		instructionsBuffer[readIndex] = '\0';
		readIndex = (readIndex + 1) % BUFFERSIZE;
		return byte;
	}
	return 0xFF;
}

void writeInstructionBuffer(char byte) {
	instructionsBuffer[writeIndex] = byte;
	writeIndex = (writeIndex + 1) % BUFFERSIZE;
	if (readIndex == writeIndex) {
		readIndex = (readIndex + 1) % BUFFERSIZE;
	}
}
