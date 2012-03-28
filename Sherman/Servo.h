#ifndef SERVO_H
#define SERVO_H
#include "Timer.h"

void setupServo();

void setServo(int timer);
void setServoPosition(int servo, int angle);

#endif