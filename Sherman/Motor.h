#ifndef MOTOR_H
#define MOTOR_H

#include "Components.h"
#include "PWM.h"

#define MOTOR_DIRECTION_LEFT 0
#define MOTOR_DIRECTION_RIGHT 1
#define MOTOR_DIRECTION_BRAKE -1


void setupMotor(int motor);
void setMotor(int motor, int dutyCycle, int direction);


#endif