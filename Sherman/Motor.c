#include "Motor.h"
#include "Main.h"
#include "Uart.h"
struct MotorAction MotorActionQueue[MOTOR_ACTION_QUEUE_SIZE];
int MotorActionQueueHeadIndex = 0, MotorActionQueueTailIndex = 0;
unsigned int CurrentMotorActionEndTime = 0;
int CurrentLeftMotorSpeed, CurrentLeftMotorDirection;
int CurrentRightMotorSpeed, CurrentRightMotorDirection;
//direction the robot is currently driving in
int Direction = 0;

//position data from main
extern struct Position RobotPosition, DeltaRobotPosition;

extern unsigned int Time;

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


void UpdateMotors()
{
    if((Time > CurrentMotorActionEndTime && CurrentMotorDuration != -1 ) || CurrentMotorDuration == 0)
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
        case MOTOR_ACTION_SLIGHT_LEFT:
            setMotor(MOTOR_WHEEL_LEFT, MOTOR_DEFAULT_SPEED * 0.8, 2);
            setMotor(MOTOR_WHEEL_RIGHT, MOTOR_DEFAULT_SPEED, 2);
            break;
        case MOTOR_ACTION_SLIGHT_RIGHT:
            setMotor(MOTOR_WHEEL_LEFT, MOTOR_DEFAULT_SPEED , 2);
            setMotor(MOTOR_WHEEL_RIGHT, MOTOR_DEFAULT_SPEED * 0.8, 2);
            break;
        case MOTOR_ACTION_OFF:
            break;
    }
}

void EnqueueMotorAction(char action)
{
    int i;
    struct MotorAction newAction;
    newAction.action = action;
    newAction.speed = MOTOR_DEFAULT_SPEED;
    switch(action)
    {
        case MOTOR_ACTION_TURN_LEFT_90:
            newAction.duration = 10000;
            break;
        case MOTOR_ACTION_TURN_RIGHT_90:
            newAction.duration = 10000;
            break;
        case MOTOR_ACTION_SLIGHT_RIGHT:
        case MOTOR_ACTION_SLIGHT_LEFT:
            newAction.duration = 1000;
        default:
            newAction.duration = -1;
            break;
    }
    //dequeue any actions that have unlimited duration
    for(i = MotorActionQueueHeadIndex; i != MotorActionQueueTailIndex; i++)
    {
        if(i >= MOTOR_ACTION_QUEUE_SIZE)
            i = -1;
        if(MotorActionQueue[i].duration == -1)
            MotorActionQueue[i].duration = 0;
    }

    MotorActionQueue[MotorActionQueueTailIndex] = newAction;
    MotorActionQueueTailIndex++;
    if(MotorActionQueueTailIndex >= MOTOR_ACTION_QUEUE_SIZE)
        MotorActionQueueTailIndex = 0;
    if(MotorActionQueueSize() == 1)
        CurrentMotorActionEndTime = Time + CurrentMotorDuration;
}

void PushMotorAction(char action)
{
    MotorActionQueueHeadIndex--;
    if(MotorActionQueueHeadIndex < 0)
        MotorActionQueueHeadIndex = MOTOR_ACTION_QUEUE_SIZE-1;
    EnqueueMotorAction(action);
}

void DequeueMotorAction()
{
    IncrementMotorActionQueueHeadIndex();
    if(MotorActionQueueSize() == 0)
            EnqueueMotorAction(MOTOR_ACTION_STOP);
    CurrentMotorActionEndTime = ReadCoreTimer() + CurrentMotorDuration;
    if(CurrentMotorAction == MOTOR_ACTION_TURN_LEFT_90)
        Direction = (Direction-1);
    if(CurrentMotorAction == MOTOR_ACTION_TURN_RIGHT_90)
        Direction = (Direction+1);
    if(Direction < 0)
        Direction = 3;
    if(Direction > 3)
        Direction = 0;
}

int MotorActionQueueSize()
{
    int size = MotorActionQueueTailIndex - MotorActionQueueHeadIndex;
    //if the tail has looped around, account for that
    if(size < 0)
        size = size + MOTOR_ACTION_QUEUE_SIZE;
    return size;
}

void IncrementMotorActionQueueHeadIndex()
{
    MotorActionQueueHeadIndex++;
    if(MotorActionQueueHeadIndex >= MOTOR_ACTION_QUEUE_SIZE)
        MotorActionQueueHeadIndex = 0;
}

void movementForward(int speed)
{
    setMotor(MOTOR_WHEEL_LEFT, speed, 2);
    setMotor(MOTOR_WHEEL_RIGHT, speed, 2);
    return;
    switch(Direction)
    {
        case 0:
            if(DeltaRobotPosition.X > 0)
            {
                EnqueueMotorAction(MOTOR_ACTION_SLIGHT_LEFT);
                EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            }
            else if(DeltaRobotPosition.X < 0)
            {
                EnqueueMotorAction(MOTOR_ACTION_SLIGHT_RIGHT);
                EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            }
            break;
        case 1:
            if(DeltaRobotPosition.Y > 0)
            {
                EnqueueMotorAction(MOTOR_ACTION_SLIGHT_RIGHT);
                EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            }
            else if(DeltaRobotPosition.Y < 0)
            {
                EnqueueMotorAction(MOTOR_ACTION_SLIGHT_LEFT);
                EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            }
            break;
        case 2:
            if(DeltaRobotPosition.X > 0)
            {
                EnqueueMotorAction(MOTOR_ACTION_SLIGHT_RIGHT);
                EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            }
            else if(DeltaRobotPosition.X < 0)
            {
                EnqueueMotorAction(MOTOR_ACTION_SLIGHT_LEFT);
                EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            }
            break;
        case 3:
            if(DeltaRobotPosition.Y > 0)
            {
                EnqueueMotorAction(MOTOR_ACTION_SLIGHT_LEFT);
                EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            }
            else if(DeltaRobotPosition.Y < 0)
            {
                EnqueueMotorAction(MOTOR_ACTION_SLIGHT_RIGHT);
                EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            }
            break;
    }
}

void movementLeft(int speed)
{
    setMotor(MOTOR_WHEEL_LEFT, speed, 1);
    setMotor(MOTOR_WHEEL_RIGHT, speed, 2);
}

void movementRight(int speed)
{
    setMotor(MOTOR_WHEEL_LEFT, speed, 2);
    setMotor(MOTOR_WHEEL_RIGHT, speed, 1);
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
void setMotor(int motor, int speed, int direction)
{
    int motorOc1, motorOc2;

    switch(motor)
    {
        case MOTOR_WHEEL_LEFT:
            speed = (int)((double)speed * MOTOR_WHEEL_LEFT_SCALE);
            break;
        case MOTOR_WHEEL_RIGHT:
            speed = (int)((double)speed * MOTOR_WHEEL_RIGHT_SCALE);
            break;
    }

    //used for debugging
    switch(motor)
    {
        case MOTOR_WHEEL_LEFT:
            CurrentLeftMotorSpeed = speed;
            CurrentLeftMotorDirection = direction;
            break;
        case MOTOR_WHEEL_RIGHT:
            CurrentRightMotorSpeed = speed;
            CurrentRightMotorDirection = direction;
            break;
    }

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
