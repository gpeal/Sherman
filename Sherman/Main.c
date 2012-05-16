#include "Initialize.h"
#include <plib.h>
#include <limits.h>
#include "fft.h"
#include "Uart.h"
#include "Motor.h"
#include "Pin.h"

// Configuring the Device Configuration Registers
// 80Mhz Core/Periph, Pri Osc w/PLL, Write protect Boot Flash
#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#pragma config ICESEL = ICS_PGx2, BWP = OFF
#pragma config FSOSCEN = OFF // to make C13 an IO pin, for the USER switch

//#define DEBUG
#define RANGEFINDER_FRONT 0
#define RANGEFINDER_BACK 1
#define RANGEFINDER_LEFT 2
#define RANGEFINDER_RIGHT 3

#define STATE_TARGET_LOCATION 0
void state_target_location();
#define STATE_INITIALIZATION_ROUTINE 1
void state_initialization_routine();
#define STATE_GOTO_SCORING_ZONE 2
void state_goto_scoring_zone();


//Global Variables
unsigned int time = 0;
char timeFlag_1ms = 0, timeFlag1ms = 0, timeFlag2ms = 0, timeFlag10ms = 0, timeFlag100ms = 0, timeFlag200ms = 0, timeFlag102_4ms = 0, timeFlag_5s = 0, timeFlag1s = 0, timeFlag5s = 0;
char ReadArduino = 0;
//position variables
struct Position {
    double X;
    double Y;
} RobotPosition;
struct Position TargetPositions[] = { {45, 50},
                                      {23, 37},
                                      {23, 15},
                                      {15, 31},
                                      {32, 45},
                                      {45, 36}};
int TargetPositionsIndex = 0;
#define TargetPosition TargetPositions[TargetPositionsIndex]
//the current coordinate value we are trying to reach
//0 for X 1 for Y
int TargetPositionAxis = 0;
//the direction we are facing
extern int Direction;
double RangefinderData[4];
char movementDirection = 0;
int movementSpeed = 1024;

//UART
int UARTReadBufferIndex = 0;
char data;

//Other global variables
int State = STATE_TARGET_LOCATION;
int SubState = 0, SubStateStartTime = 0;
int currentNumberOfCubes = 0;
int Cubes = 0;

char map[240][120];

//function prototypes
void PeriodicFunctions();
void delay_ms(int ms);
void ParseUartData();

int main(void)
{
    initialize();
    digitalWrite(A5, 1);
    EnqueueMotorAction(MOTOR_ACTION_FORWARD);
    while(1)
    {
        PeriodicFunctions();
        switch(State)
        {
            case STATE_INITIALIZATION_ROUTINE:
                state_initialization_routine();
                break;
            case STATE_TARGET_LOCATION:
                state_target_location();
                break;
        }
    }
}

void ChangeState(int state)
{
    State = state;
    SubState = 0;
    SubStateStartTime = 0;
}

void state_initialization_routine()
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
                ChangeState(STATE_TARGET_LOCATION);
            }
            break;
    }
}

void state_target_location()
{
    double robotPositionOnTargetAxis, targetPositionOnTargetAxis, deltaPositionOnTargetAxis;
    double robotPositionOnOtherAxis, targetPositionOnOtherAxis, deltaPositionOnOtherAxis;
    robotPositionOnTargetAxis = TargetPositionAxis == 0 ? RobotPosition.X : RobotPosition.Y;
    targetPositionOnTargetAxis = TargetPositionAxis == 0 ? TargetPosition.X : TargetPosition.Y;
    deltaPositionOnTargetAxis = targetPositionOnTargetAxis - robotPositionOnTargetAxis;
    //reached goal on current axis
    if(deltaPositionOnTargetAxis < 5)
    {
        robotPositionOnOtherAxis = TargetPositionAxis == 0 ? RobotPosition.Y : RobotPosition.X;
        targetPositionOnOtherAxis = TargetPositionAxis == 0 ? TargetPosition.Y : TargetPosition.X;
        deltaPositionOnOtherAxis = targetPositionOnTargetAxis - robotPositionOnTargetAxis;
        //reached goal on both axis
        if(deltaPositionOnOtherAxis < 5)
        {
            if(Cubes >= 4)
                ChangeState(STATE_GOTO_SCORING_ZONE);
            else
                TargetPositionsIndex++;
        }
    }
}

void RunEvery_1ms()
{

}

void RunEvery1ms()
{
    if(State != STATE_INITIALIZATION_ROUTINE)
        UpdateMotors();
}

void RunEvery2ms()
{

}

void RunEvery10ms()
{

}

void RunEvery100ms()
{
}

void RunEvery102_4ms()
{

}

void RunEvery200ms()
{
    //update position
    int forwardDistance, rightDistance, backDistance, leftDistance;
    forwardDistance = RangefinderData[Direction];


#ifdef DEBUG
        //optional send motor over uart
        switch(movementDirection)
        {
            case 0:
                sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 2, movementSpeed, 2, movementSpeed);
                break;
            case 1:
                sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 2, movementSpeed, 1, movementSpeed);
                break;
            case 2:
                sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 1, movementSpeed, 2, movementSpeed);
                break;
            case 3:
                sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 1, movementSpeed, 1, movementSpeed);
                break;
            case 4:
                sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 0, movementSpeed, 0, movementSpeed);
                break;
        }
        SendString(1, UARTBuffer);
#endif
}

void RunEvery_5s()
{

}

void RunEvery1s()
{
    char front, right, back, left;
    front = UARTReadBuffer[0];
    right = UARTReadBuffer[1];
    back = UARTReadBuffer[2];
    left = UARTReadBuffer[3];
    sprintf(UARTWriteBuffer, "%04u %04u %04u %04u\n", front & 0xFF, right & 0xFF, back & 0xFF, left & 0xFF);
    //sprintf(UARTWriteBuffer, "%s\n", UARTReadBuffer);
    SendString(1, UARTWriteBuffer);
}

void RunEvery5s()
{
}

void PeriodicFunctions()
{
    if(timeFlag_1ms)
    {
        timeFlag_1ms = 0;
        RunEvery_1ms();
    }

    if(timeFlag1ms)
    {
        timeFlag1ms = 0;
        RunEvery1ms();
    }

    if(timeFlag2ms)
    {
        timeFlag2ms = 0;
        RunEvery2ms();
    }

    if(timeFlag10ms)
    {
        timeFlag10ms = 0;
        RunEvery10ms();
    }

    if(timeFlag102_4ms)
    {
        timeFlag102_4ms = 0;
        RunEvery102_4ms();
    }

    if(timeFlag100ms)
    {
        timeFlag100ms = 0;
        RunEvery100ms();
    }

    if(timeFlag200ms)
    {
        timeFlag200ms = 0;
        RunEvery200ms();
    }

    if(timeFlag_5s)
    {
        timeFlag_5s = 0;
        RunEvery_5s();
    }
    if(timeFlag1s)
    {
        timeFlag1s = 0;
        RunEvery1s();
    }

    if(timeFlag5s)
    {
        timeFlag5s = 0;
        RunEvery5s();
    }
}

void __ISR(_TIMER_1_VECTOR, ipl1) Timer1Isr(void)
{
    time++;
    if(time%1 < 1)
        timeFlag_1ms = 1;
    if(time%10 < 1)
        timeFlag1ms = 1;
    if(time%20 < 1)
        timeFlag2ms = 1;
    if(time%100 < 1)
        timeFlag10ms = 1;
    if(time%1000 < 1)
        timeFlag100ms = 1;
    if(time%1024 < 1)
        timeFlag102_4ms = 1;
    if(time%2000 < 1)
        timeFlag200ms = 1;
    if(time%5000 < 1)
    {
        timeFlag_5s = 1;
        togglePin(A4);
    }
    if(time%10000 < 1)
        timeFlag1s = 1;
    if(time%50000 < 1)
        timeFlag5s = 1;
    mT1ClearIntFlag();
}


// UART 2 interrupt handler
// it is set at priority level 2
void __ISR(_UART2_VECTOR, ipl2) IntUart2Handler(void)
{
    // Is this an RX interrupt?
    if(mU2RXGetIntFlag())
    {
        data = ReadCharacter(2);
        if(data == '\n')
        {
            UARTReadBufferIndex = 0;
            ReadArduino = 1;
        }
        else if(ReadArduino == 1)
        {
            UARTReadBuffer[UARTReadBufferIndex] = data;
            UARTReadBufferIndex++;
            if(UARTReadBufferIndex >= (ARDUINO_BUFFER_SIZE - 1))
            {
                ReadArduino = 0;
                UARTReadBufferIndex = 0;
            }
        }
        // Clear the RX interrupt Flag
        mU2RXClearIntFlag();
    }

    // We don't care about TX interrupt
    if ( mU2TXGetIntFlag() )
    {
            mU2TXClearIntFlag();
    }
}

void delay_ms(int ms)
{
    int startTime = time;
    while(time < startTime + ms*10);
}