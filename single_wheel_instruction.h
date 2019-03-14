/*
 * single_wheel_instruction.h
 *
 *  Created on: 6 mars 2019
 *      Author: francois
 */

#ifndef SINGLE_WHEEL_INSTRUCTION_H_
#define SINGLE_WHEEL_INSTRUCTION_H_

#include "stm32f4_discovery.h"

struct {
	uint16_t currentMotor;
	uint16_t currentAction;
	uint16_t currentReference;
	uint16_t currentMaxTick;
	char currentChecksum;
} typedef SingleWheelInstruction;

int single_wheel_handle_next_byte(char byte, SingleWheelInstruction * instruction);

#endif /* SINGLE_WHEEL_INSTRUCTION_H_ */
