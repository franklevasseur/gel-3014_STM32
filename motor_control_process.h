/*
 * motor_control_process.h
 *
 *  Created on: 3 févr. 2019
 *      Author: Francois
 */

#ifndef MOTOR_CONTROL_PROCESS_H_
#define MOTOR_CONTROL_PROCESS_H_

#include "pwm_module.h"
#include "encoder_module.h"
#include "bridge_module.h"
#include "instruction_buffer.h"
#include "instruction_parser.h"
#include "uart_module.h"

#include "stm32f4_discovery.h"

void do_motor_control(uint32_t p_sampling_frequency);
void controlMotors(void);

#endif /* MOTOR_CONTROL_PROCESS_H_ */
