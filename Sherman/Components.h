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

//Map
#define MAP_EMPTY   0
#define MAP_ROBOT   1
#define MAP_CUBE    2
#define MAP_LINE    3
#define MAP_TARGET  4


#endif