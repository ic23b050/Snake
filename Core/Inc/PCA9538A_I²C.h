/*
 * PCA9538A_I²C.h
 *
 *  Created on: Jan 16, 2025
 *      Author: Mudassar1
 */

#ifndef INC_PCA9538A_I_C_H_
#define INC_PCA9538A_I_C_H_

#define JOYSTICK_I2C_ADDR 0x70

#include "stdint.h"

typedef struct {
    int x;
    int y;
    int button;
} JoystickState;

JoystickState readJoystick();
void updateDirectionWithJoystick();


#endif /* INC_PCA9538A_I_C_H_ */
