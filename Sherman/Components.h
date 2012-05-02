#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <plib.h>

//1 refers to OC1
#define MOTOR_WHEEL_LEFT 1
//OC1
#define MOTOR_WHEEL_LEFT_OC 1
#define MOTOR_WHEEL_LEFT_DIRECTION LATDbits.LATD5

#define MOTOR_WHEEL_RIGHT 2
#define MOTOR_WHEEL_RIGHT_OC 2
#define MOTOR_WHEEL_RIGHT_DIRECTION LATDbits.LATD6

//I2C components
#define DAC 1
#define DAC_ADDRESS 0b0101100
#define DAC_CMD_CHANGE 0

#endif