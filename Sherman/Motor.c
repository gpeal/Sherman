#include "Motor.h"

void setupMotor(int motor)
{
    setupPWM(motor);
}

void setMotor(int motor, int dutyCycle, int direction)
{
    //the motors are connected "backwards"
    direction = !direction;
    int motorOc;
    switch(motor)
    {
        case MOTOR_WHEEL_LEFT:
            motorOc = MOTOR_WHEEL_LEFT;
            MOTOR_WHEEL_LEFT_DIRECTION = direction;
            break;
        case MOTOR_WHEEL_RIGHT:
            motorOc = MOTOR_WHEEL_RIGHT;
            MOTOR_WHEEL_RIGHT_DIRECTION = direction;
            break;
        default:
            return;
    }
    setDutyCycle(motorOc, dutyCycle);
}