/*
 * bridge_module.h
 *
 *  Created on: 3 févr. 2019
 *      Author: Francois
 */

#ifndef BRIDGE_MODULE_H_
#define BRIDGE_MODULE_H_

#define BLOCK 1
#define CW 2
#define CCW 3

void init_bridge_pins(void);
void set_motor_action(int motor, int action);

#endif /* BRIDGE_MODULE_H_ */
