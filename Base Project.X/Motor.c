#include "Motor.h"


void setupMotor(int motor)
{
    setupPWM(motor);
}

void setMotor(int motor, int cycle)
{
    setDutyCycle(motor, cycle);
}