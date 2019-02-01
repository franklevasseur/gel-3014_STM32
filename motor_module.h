/*
 * motor_module.h
 *
 *  Created on: 31 janv. 2019
 *      Author: Francois
 */

#ifndef MOTOR_MODULE_H_
#define MOTOR_MODULE_H_
#include "stm32f4_discovery.h"

void init_encoders();
uint32_t getCounts(uint32_t counts[]);
void resetCounts();

#endif /* MOTOR_MODULE_H_ */
