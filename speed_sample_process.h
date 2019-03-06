/*
 * speed_sample_process.h
 *
 *  Created on: 3 févr. 2019
 *      Author: Francois
 */

#ifndef SPEED_SAMPLE_PROCESS_H_
#define SPEED_SAMPLE_PROCESS_H_

#include "pwm_module.h"
#include "encoder_module.h"
#include "bridge_module.h"
#include "uart_module.h"
#include "instruction_parser.h"

void mesureSpeeds(void);
void do_speed_sampling(uint32_t p_sampling_frequency);

#endif /* SPEED_SAMPLE_PROCESS_H_ */
