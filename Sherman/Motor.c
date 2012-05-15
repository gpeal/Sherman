#include "Motor.h"
#include "Uart.h"

#define CurrentMotorSpeed MotorActionQueue[MotorActionQueueHeadIndex].speed
#define CurrentMotorAction MotorActionQueue[MotorActionQueueHeadIndex].action
#define CurrentMotorDuration MotorActionQueue[MotorActionQueueHeadIndex].duration

struct MotorAction
{
    char action;
    int duration;
    int speed;
};
struct MotorAction MotorActionQueue[MOTOR_ACTION_QUEUE_SIZE];
int MotorActionQueueHeadIndex = 0, MotorActionQueueTailIndex = 0;
unsigned int CurrentMotorActionEndTime = 0;

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

void UpdateMotors()
{
    int time = ReadCoreTimer();
    if(time > CurrentMotorActionEndTime && CurrentMotorDuration != -1 )
        DequeueMotorAction();
    switch(CurrentMotorAction)
    {
        case MOTOR_ACTION_FORWARD:
            movementForward(CurrentMotorSpeed);
            break;
        case MOTOR_ACTION_BACKWARD:
            movementBackward(CurrentMotorSpeed);
            break;
        case MOTOR_ACTION_TURN_LEFT:
        case MOTOR_ACTION_TURN_LEFT_90:
            movementLeft(CurrentMotorSpeed);
            break;
        case MOTOR_ACTION_TURN_RIGHT:
        case MOTOR_ACTION_TURN_RIGHT_90:
            movementRight(CurrentMotorSpeed);
            break;
        case MOTOR_ACTION_STOP:
            movementStop();
            break;
    }
}

void EnqueueMotorAction(char action)
{
    struct MotorAction newAction;
    newAction.action = action;
    newAction.speed = 700;
    switch(action)
    {
        case MOTOR_ACTION_FORWARD:
            newAction.duration = -1;
            break;
        case MOTOR_ACTION_BACKWARD:
            newAction.duration = -1;
            break;
        case MOTOR_ACTION_STOP:
            newAction.duration = -1;
            break;
        case MOTOR_ACTION_TURN_RIGHT:
            newAction.duration = -1;
            break;
        case MOTOR_ACTION_TURN_LEFT:
            newAction.duration = -1;
            break;
        case MOTOR_ACTION_TURN_LEFT_90:
            newAction.duration = 11000000;
            break;
        case MOTOR_ACTION_TURN_RIGHT_90:
            newAction.duration = 11000000;
            break;
    }
    //dequeue any actions that have unlimited duration
    while(CurrentMotorDuration == -1 && MotorActionQueue > 0)
        DequeueMotorAction();

    MotorActionQueue[MotorActionQueueTailIndex] = newAction;
    MotorActionQueueTailIndex++;
    if(MotorActionQueueTailIndex >= MOTOR_ACTION_QUEUE_SIZE)
        MotorActionQueueTailIndex = 0;
    if(MotorActionQueueSize() == 1)
        CurrentMotorActionEndTime = ReadCoreTimer() + CurrentMotorDuration;
}

void DequeueMotorAction()
{
    IncrementMotorActionQueueHeadIndex();
    if(MotorActionQueueSize() == 0)
            EnqueueMotorAction(MOTOR_ACTION_STOP);
    CurrentMotorActionEndTime = ReadCoreTimer() + CurrentMotorDuration;
}

int MotorActionQueueSize()
{
    int size = MotorActionQueueTailIndex - MotorActionQueueTailIndex;
    //if the tail has looped around, account for that
    if(size < 0)
        size = size + MOTOR_ACTION_QUEUE_SIZE;
    return size;
}

void IncrementMotorActionQueueHeadIndex()
{
    MotorActionQueueHeadIndex++;
    if(MotorActionQueueHeadIndex > MOTOR_ACTION_QUEUE_SIZE)
        MotorActionQueueHeadIndex = 0;
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

void movementStop()
{
    setMotor(MOTOR_WHEEL_LEFT, 500, 0);
    setMotor(MOTOR_WHEEL_RIGHT, 500, 0);
}