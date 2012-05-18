#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <plib.h>
#include "Pin.h"

#define MOTOR_WHEEL_RIGHT 1
//OC1
#define MOTOR_WHEEL_RIGHT_OC1 3
#define MOTOR_WHEEL_RIGHT_OC2 4
#define MOTOR_WHEEL_RIGHT_SCALE 1

#define MOTOR_WHEEL_LEFT 2
#define MOTOR_WHEEL_LEFT_OC1 1
#define MOTOR_WHEEL_LEFT_OC2 2
#define MOTOR_WHEEL_LEFT_SCALE 0.95

//Map
#define MAP_EMPTY   0
#define MAP_ROBOT   1
#define MAP_CUBE    2
#define MAP_LINE    3
#define MAP_TARGET  4

//I2C
#define ARDUINO 0
#define ARDUINO_ADDRESS 0x4

#define UART_ARDUINO_RX F4
#define UART_ARDUINO_TX F5

#define UART_XBEE_RX G7
#define UART_XBEE_TX G8


#endif