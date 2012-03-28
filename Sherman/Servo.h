#ifndef SERVO_H
#define SERVO_H
#include "Timer.h"

void setupServo();

void setServo(int timer, int outputControl);
void setServoPosition(int outputControl, int angle);

#endif