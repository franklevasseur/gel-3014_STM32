/*
 * instruction_parser.h
 *
 *  Created on: 4 f�vr. 2019
 *      Author: Francois
 */

#ifndef INSTRUCTION_PARSER_H_
#define INSTRUCTION_PARSER_H_

#include "stm32f4_discovery.h"
#include "uart_module.h"

int parseInstruction(char byte, uint16_t fullInstruction[5]);

#endif /* INSTRUCTION_PARSER_H_ */
