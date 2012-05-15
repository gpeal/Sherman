#ifndef MOTOR_H
#define MOTOR_H

#include "Components.h"
#include "PWM.h"

#define MOTOR_DIRECTION_LEFT 1
#define MOTOR_DIRECTION_RIGHT 2
#define MOTOR_DIRECTION_BRAKE 0


void setupMotor(int motor);
void setMotor(int motor, int dutyCycle, int direction);

// Higher level Commands
void movementForward(int speed);
void movementLeft(int speed);
void movementRight(int speed);
void movementBackward(int speed);
void movementBrake();


#endif