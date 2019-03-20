/*
 * all_wheels_instruction.h
 *
 *  Created on: 19 mars 2019
 *      Author: francois
 */

#ifndef ALL_WHEELS_INSTRUCTION_H_
#define ALL_WHEELS_INSTRUCTION_H_

#include "stm32f4_discovery.h"

struct {
	uint16_t actions[4];
	uint16_t speeds[4];
	uint16_t max_ticks[4];
	char checksum;
} typedef AllWheelsInstruction;

int all_wheels_handle_next_byte(char byte, AllWheelsInstruction * instruction);

#endif /* ALL_WHEELS_INSTRUCTION_H_ */
