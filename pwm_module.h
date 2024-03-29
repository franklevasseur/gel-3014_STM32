/*
 * pwm_module.h
 *
 *  Created on: 31 janv. 2019
 *      Author: Francois
 */

#ifndef PWM_MODULE_H_
#define PWM_MODULE_H_

#include "stm32f4_discovery.h"

void configure_pwm_af(void);
void configure_pwm_freq(uint16_t prescaler, uint16_t period);
void configure_pwm_ccr(int channel, uint16_t ccr);
void start_pwm(void);

#endif /* PWM_MODULE_H_ */
