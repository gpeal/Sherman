#include "StateMachine.h"
#include "Motor.h"

int SubState = 0, SubStateStartTime = 0;
extern int time;
int State = STATE_NORMAL;

void ChangeState(int state)
{
    State = state;
    SubState = 0;
    SubStateStartTime = 0;
}

void delegateState(int state)
{
    switch(state)
    {
        case STATE_INITIALIZATION:
            state1();
            break;
    }
}

void state0()
{
    
}

void state1()
{
    switch(SubState)
    {
        case 0:
            setMotor(MOTOR_WHEEL_LEFT, 1000, 2);
            setMotor(MOTOR_WHEEL_RIGHT, 1000, 2);
            if(time - SubStateStartTime > 20000) {
                SubState++;
                SubStateStartTime = time;
            }
            break;
        case 1:
            setMotor(MOTOR_WHEEL_LEFT, 1000, 2);
            setMotor(MOTOR_WHEEL_RIGHT, 1000, 1);
            if(time - SubStateStartTime > 20000) {
                SubState++;
                SubStateStartTime = time;
            }
            break;
        case 2:
            setMotor(MOTOR_WHEEL_LEFT, 1000, 1);
            setMotor(MOTOR_WHEEL_RIGHT, 1000, 2);
            if(time - SubStateStartTime > 20000) {
                SubState++;
                SubStateStartTime = time;
            }
            break;
        case 3:
            setMotor(MOTOR_WHEEL_LEFT, 1000, 1);
            setMotor(MOTOR_WHEEL_RIGHT, 1000, 1);
            if(time - SubStateStartTime > 20000) {
                SubState++;
                SubStateStartTime = time;
            }
            break;
        case 4:
            setMotor(MOTOR_WHEEL_LEFT, 1000, 0);
            setMotor(MOTOR_WHEEL_RIGHT, 1000, 0);
            if(time - SubStateStartTime > 20000) {
                ChangeState(STATE_NORMAL);
            }
            break;
    }
}