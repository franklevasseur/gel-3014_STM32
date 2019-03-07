#ifndef INSTRUCTION_PARSER_H_
#define INSTRUCTION_PARSER_H_

#include "stm32f4_discovery.h"
#include "uart_module.h"
#include "single_wheel_instruction.h"

struct {
	int instructionType;
	SingleWheelInstruction singleWheelInstruction;
} typedef InstructionContainer;

#define INSTRUCTION_BLOCK_ALL 1
#define INSTRUCTION_SINGLE 2
#define INSTRUCTION_ALLWHEELS 3

int parseInstruction(char byte, InstructionContainer * fullInstruction);

#endif /* INSTRUCTION_PARSER_H_ */
