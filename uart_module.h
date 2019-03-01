/*
 * uart_module.h
 *
 *  Created on: 4 févr. 2019
 *      Author: Francois
 */

#ifndef UART_MODULE_H_
#define UART_MODULE_H_

#include "instruction_buffer.h"
#include "stm32f4_discovery.h"

void uart_write(char p_data);
void init_uart(uint32_t baudrate);
void transmit_sample(int16_t sample);

#endif /* UART_MODULE_H_ */
