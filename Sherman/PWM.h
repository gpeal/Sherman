#ifndef PWM_H
#define PWM_H

void setupPWMTimer();
void setupPWM(int outputControlX);
void setDutyCycle(int outputControlX, int dutyCycle);

#endif