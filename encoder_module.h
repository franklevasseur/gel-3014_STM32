/*
 * motor_module.h
 *
 *  Created on: 31 janv. 2019
 *      Author: Francois
 */

#ifndef ENCODER_MODULE_H_
#define ENCODER_MODULE_H_
#include "stm32f4_discovery.h"

void init_encoders();
void getCounts(int32_t counts[]);
void resetCounts();

#endif /* ENCODER_MODULE_H_ */
