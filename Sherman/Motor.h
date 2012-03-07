#ifndef MOTOR_H
#define MOTOR_H

#include "Components.h"
#include "PWM.h"


void setupMotor(int motor);
void setMotor(int motor, int dutyCycle, int direction);


#endif