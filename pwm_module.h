/*
 * pwm_module.h
 *
 *  Created on: 31 janv. 2019
 *      Author: Francois
 */

#ifndef PWM_MODULE_H_
#define PWM_MODULE_H_

#include "stm32f4_discovery.h"

void init_pwm(uint16_t prescaler, uint16_t period, uint16_t ccr);
void configure_timer_freq(uint16_t prescaler, uint16_t period);

#endif /* PWM_MODULE_H_ */
