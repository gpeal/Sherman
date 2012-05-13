#include "Motor.h"

void setupMotor(int motor)
{
    setupPWM(motor);
}

void setMotor(int motor, int speed, int direction)
{
    int motorOc, control1, control2;

    switch(direction)
    {
        case MOTOR_DIRECTION_RIGHT:
            control1 = 1;
            control2 = 0;
            break;
        case MOTOR_DIRECTION_LEFT:
            control1 = 0;
            control2 = 1;
            break;
        case MOTOR_DIRECTION_BRAKE:
            control1 = 1;
            control2 = 1;
            break;
        default:
            control1 = 1;
            control2 = 1;
            break;
    }

    switch(motor)
    {
        case MOTOR_WHEEL_LEFT:
            motorOc = MOTOR_WHEEL_LEFT_OC;
            MOTOR_WHEEL_LEFT_CONTROL1 = control1;
            MOTOR_WHEEL_LEFT_CONTROL2 = control2;
            break;
        case MOTOR_WHEEL_RIGHT:
            motorOc = MOTOR_WHEEL_RIGHT_OC;
            MOTOR_WHEEL_RIGHT_CONTROL1 = control1;
            MOTOR_WHEEL_RIGHT_CONTROL2 = control2;
            break;
        default:
            return;
    }
    setDutyCycle(motorOc, speed);
}