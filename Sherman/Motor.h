#ifndef MOTOR_H
#define MOTOR_H

#include "Components.h"
#include "PWM.h"

#define MOTOR_DIRECTION_LEFT 1
#define MOTOR_DIRECTION_RIGHT 2
#define MOTOR_DIRECTION_BRAKE 0

#define MOTOR_ACTION_FORWARD 0
#define MOTOR_ACTION_BACKWARD 1
#define MOTOR_ACTION_STOP 2
#define MOTOR_ACTION_TURN_LEFT 3
#define MOTOR_ACTION_TURN_RIGHT 4
#define MOTOR_ACTION_TURN_LEFT_90 5
#define MOTOR_ACTION_TURN_RIGHT_90 6

#define MOTOR_ACTION_QUEUE_SIZE 128

void setupMotor(int motor);
void setMotor(int motor, int dutyCycle, int direction);

void UpdateMotors();
void EnqueueMotorAction(char action);
void DequeueMotorAction();
int MotorActionQueueSize();
void IncrementMotorActionQueueHeadIndex();

void movementForward(int speed);
void movementBackward(int speed);
void movementStop();
void movementLeft(int speed);
void movementRight(int speed);


#endif