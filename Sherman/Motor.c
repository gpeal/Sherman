#include "Motor.h"
#include "Uart.h"

void setupMotor(int motor)
{
    switch(motor)
    {
        case MOTOR_WHEEL_LEFT:
            setupPWM(MOTOR_WHEEL_LEFT_OC1);
            setupPWM(MOTOR_WHEEL_LEFT_OC2);
            break;
        case MOTOR_WHEEL_RIGHT:
            setupPWM(MOTOR_WHEEL_RIGHT_OC1);
            setupPWM(MOTOR_WHEEL_RIGHT_OC2);
            break;
    }
}

void setMotor(int motor, int speed, int direction)
{
    int motorOc1, motorOc2;

    switch(motor)
    {
        case MOTOR_WHEEL_LEFT:
            motorOc1 = MOTOR_WHEEL_LEFT_OC1;
            motorOc2 = MOTOR_WHEEL_LEFT_OC2;
            break;
        case MOTOR_WHEEL_RIGHT:
            motorOc1 = MOTOR_WHEEL_RIGHT_OC1;
            motorOc2 = MOTOR_WHEEL_RIGHT_OC2;
            break;
        default:
            return;
    }

    switch(direction)
    {
        case MOTOR_DIRECTION_LEFT:
            setDutyCycle(motorOc1, 0);
            setDutyCycle(motorOc2, speed);
            break;
        case MOTOR_DIRECTION_RIGHT:
            setDutyCycle(motorOc1, speed);
            setDutyCycle(motorOc2, 0);
            break;
        case MOTOR_DIRECTION_BRAKE:
            setDutyCycle(motorOc1, 1024);
            setDutyCycle(motorOc2, 1024);
            break;
    }
}

void movementForward(int speed)
{
    setMotor(MOTOR_WHEEL_LEFT, speed, 2);
    setMotor(MOTOR_WHEEL_RIGHT, speed, 2);
}

void movementLeft(int speed)
{
    setMotor(MOTOR_WHEEL_LEFT, speed, 2);
    setMotor(MOTOR_WHEEL_RIGHT, speed, 1);
}

void movementRight(int speed)
{
    setMotor(MOTOR_WHEEL_LEFT, speed, 1);
    setMotor(MOTOR_WHEEL_RIGHT, speed, 2);
}

void movementBackward(int speed)
{
    setMotor(MOTOR_WHEEL_LEFT, speed, 1);
    setMotor(MOTOR_WHEEL_RIGHT, speed, 1);
}

void movementBrake()
{
    setMotor(MOTOR_WHEEL_LEFT, 500, 0);
    setMotor(MOTOR_WHEEL_RIGHT, 500, 0);
}