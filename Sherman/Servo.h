#ifndef SERVO_H
#define SERVO_H
#include "Timer.h"

/*
 * WIRING
 * brown -> ground
 * red -> vcc
 * orange -> pwm
 */

void setupServo();

void setServo(int timer);
void setServoPosition(int servo, int angle);

#endif