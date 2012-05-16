#include "Initialize.h"
#include <plib.h>
#include <limits.h>
//#include <math.h>
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
#define RANGEFINDER_ERROR_THRESHOLD 2
#define ARENA_WIDTH 72
#define ARENA_LENGTH_0 144
#define ARENA_LENGTH_1 162
#define ARENA_LENGTH_2 180

#define STATE_FIND_CUBES 0
void NavigateToTarget();
#define STATE_INITIALIZATION_ROUTINE 1
void InitialRoutine();
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
struct Position TargetPositions[] = { {12, 0},
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
struct RangefinderReading
{
    int value;
    char valid = 1;
};

#define RANGEFINDER_DATA_BUFFER_SIZE 5
double RangefinderData[4][RANGEFINDER_DATA_BUFFER_SIZE];
double RangefinderDataDelta[4];
char movementDirection = 0;
int movementSpeed = 1024;

//UART
int UARTReadBufferIndex = 0;
char data;

//Other global variables
int State = STATE_FIND_CUBES;
int SubState = 0, SubStateStartTime = 0;
int Cubes = 0;
extern struct MotorAction MotorActionQueue[];
extern int MotorActionQueueHeadIndex;

char map[240][120];

//function prototypes
void PeriodicFunctions();
void delay_ms(int ms);
void ParseUartData();
void ReadRangefinders();
void UpdatePosition();

int main(void)
{
    initialize();
    while(1)
    {
        PeriodicFunctions();
    }
}

void ChangeState(int state)
{
    State = state;
    SubState = 0;
    SubStateStartTime = 0;
}

void InitialRoutine()
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
        /*case 1:
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
            break;*/
        default:
            setMotor(MOTOR_WHEEL_LEFT, 1000, 0);
            setMotor(MOTOR_WHEEL_RIGHT, 1000, 0);
            if(time - SubStateStartTime > 20000) {
                ChangeState(STATE_FIND_CUBES);
            }
            break;
    }
}

void ReadRangefinders()
{
    int i, length, width;
    struct RangefinderReading rawData[4];
    rawData[0].value = UARTReadBuffer[0];
    rawData[1].value = UARTReadBuffer[1];
    rawData[2].value = UARTReadBuffer[2];
    rawData[3].value = UARTReadBuffer[3];
    for(i = 0; i < 4; i++)
    {
        if(rawData[i].value >= 200)
            rawData[i].valid = 0;
        //TODO: mark large deltas invalid
        //if(abs(RangefinderData[i][RANGEFINDER_DATA_BUFFER_SIZE-1] - rawData[i]) >= 5 && RangefinderData[i][RANGEFINDER_DATA_BUFFER_SIZE-1] != 0) {}

        //TODO: uncomment width and height checks
        //move lengthwise
        if(Direction == 0 || Direction == 2)
        {
            if(rawData[RANGEFINDER_FRONT].valid && rawData[RANGEFINDER_BACK].valid)
            {
                length = rawData[RANGEFINDER_FRONT] + rawData[RANGEFINDER_BACK] + 12;
                if(abs(length-ARENA_LENGTH_0) > 10 && abs(length-ARENA_LENGTH_1) > 10 && abs(length-ARENA_LENGTH_2) > 10)
                {
                    //TODO: check if one of the values might be valid
                }
            }
        }
    }
    RangefinderData[0] = RangefinderData[1];
    RangefinderData[1] = RangefinderData[2];
    RangefinderData[2] = RangefinderData[3];
    RangefinderData[3] = RangefinderData[4];
    for(i = 0; i < 4; i++)
    {
        if(rawData[i].valid)
            RangefinderData[i][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[i];
        else

    }

}

void UpdatePosition()
{
    
    


    RangefinderDataDelta[RANGEFINDER_FRONT] = UARTReadBuffer[0] - RangefinderData[RANGEFINDER_FRONT];
    RangefinderDataDelta[RANGEFINDER_RIGHT] = UARTReadBuffer[1] - RangefinderData[RANGEFINDER_RIGHT];
    RangefinderDataDelta[RANGEFINDER_BACK] = UARTReadBuffer[2] - RangefinderData[RANGEFINDER_BACK];
    RangefinderDataDelta[RANGEFINDER_LEFT] = UARTReadBuffer[3] - RangefinderData[RANGEFINDER_LEFT];
    RangefinderData[RANGEFINDER_FRONT] = UARTReadBuffer[0];
    RangefinderData[RANGEFINDER_RIGHT] = UARTReadBuffer[1];
    RangefinderData[RANGEFINDER_BACK] = UARTReadBuffer[2];
    RangefinderData[RANGEFINDER_LEFT] = UARTReadBuffer[3];
    //update position
    int forwardDistance, rightDistance, backDistance, leftDistance;
    forwardDistance = RangefinderData[Direction];
}

void NavigateToTarget()
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
    switch(State)
    {
        case STATE_INITIALIZATION_ROUTINE:
            InitialRoutine();
            break;
        case STATE_FIND_CUBES:
            break;
    }
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
    ReadRangefinders();
    UpdatePosition();

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
    switch(State)
    {
        case STATE_FIND_CUBES:
            NavigateToTarget();
            break;
    }
}

void RunEvery1s()
{
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