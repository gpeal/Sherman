#include "Motor.h"

void setupMotor(int motor)
{
    setupPWM(motor);
}

void setMotor(int motor, int dutyCycle, int direction)
{
    int motorOc;
    switch(motor)
    {
        case MOTOR_WHEEL_LEFT:
            motorOc = MOTOR_WHEEL_LEFT_OC;
            MOTOR_WHEEL_LEFT_DIRECTION = direction;
            break;
        case MOTOR_WHEEL_RIGHT:
            motorOc = MOTOR_WHEEL_RIGHT_OC;
            MOTOR_WHEEL_RIGHT_DIRECTION = direction;
            break;
        default:
            return;
    }
    setDutyCycle(motorOc, dutyCycle);
}