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

void do_motor_control(void);
void controlMotors(void);

#endif /* MOTOR_CONTROL_PROCESS_H_ */
