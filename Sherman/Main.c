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

#define DEBUG
#define RANGEFINDER_FRONT 0
#define RANGEFINDER_RIGHT 1
#define RANGEFINDER_BACK 2
#define RANGEFINDER_LEFT 3
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
#define STATE_DRIVE_PARALLEL 3
void DriveParallel();


//Global Variables
unsigned int time = 0;
char timeFlag_1ms = 0, timeFlag1ms = 0, timeFlag2ms = 0, timeFlag10ms = 0, timeFlag100ms = 0, timeFlag200ms = 0, timeFlag102_4ms = 0, timeFlag_5s = 0, timeFlag1s = 0, timeFlag5s = 0;
char ReadArduino = 0;
//position variables
struct Position {
    int X;
    int Y;
};
struct Position RobotPosition, DeltaRobotPosition;
struct Position CubeLocation[] = { {12, 15},
                                      {23, 37},
                                      {23, 15},
                                      {15, 31},
                                      {32, 45},
                                      {45, 36}};
int CubeIndex = 0;
#define Cube CubeLocation[CubeIndex]
//the current coordinate value we are trying to reach
//0 for X 1 for Y
int TargetPositionAxis = 0;
//the direction we are facing
extern int Direction;
struct RangefinderReading
{
    int value;
    char valid;
};

#define RANGEFINDER_DATA_BUFFER_SIZE 5
struct RangefinderReading RangefinderData[4][RANGEFINDER_DATA_BUFFER_SIZE];
double RangefinderDataDelta[4];
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
extern int CurrentRightMotorSpeed, CurrentRightMotorDirection;
extern int CurrentLeftMotorSpeed, CurrentLeftMotorDirection;

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
    //used if there is no valid data and we have to interpolate it
    int interpolationDirection, interpolationIndex;
    struct RangefinderReading rawData[4];
    rawData[0].value = UARTReadBuffer[0] & 0xFF;
    rawData[0].valid = 1;
    rawData[1].value = UARTReadBuffer[1] & 0xFF;
    rawData[1].valid = 1;
    rawData[2].value = UARTReadBuffer[2] & 0xFF;
    rawData[2].valid = 1;
    rawData[3].value = UARTReadBuffer[3] & 0xFF;
    rawData[3].valid = 1;
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
                length = rawData[RANGEFINDER_FRONT].value + rawData[RANGEFINDER_BACK].value + 12;
                if(abs(length-ARENA_LENGTH_0) > 10 && abs(length-ARENA_LENGTH_1) > 10 && abs(length-ARENA_LENGTH_2) > 10)
                {
                    //TODO: check if one of the values might be valid
                }
            }
        }
    }
    for(i = 0; i < 4; i++)
    {
        RangefinderData[i][0] = RangefinderData[i][1];
        RangefinderData[i][1] = RangefinderData[i][2];
        RangefinderData[i][2] = RangefinderData[i][3];
        RangefinderData[i][3] = RangefinderData[i][4];
    }
    for(i = 0; i < 4; i++)
    {
        if(rawData[i].valid)
            RangefinderData[i][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[i];
    }
    
    if(!rawData[0].valid)
    {
        //the back ragefinder was valid, add the opposite of the rear rangefinder delta
        if(rawData[2].valid)
        {
            rawData[0].value = RangefinderData[0][RANGEFINDER_DATA_BUFFER_SIZE-2].value
                    - ( RangefinderData[2][RANGEFINDER_DATA_BUFFER_SIZE-2].value -  RangefinderData[2][RANGEFINDER_DATA_BUFFER_SIZE-3].value );
            RangefinderData[0][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[0];
        }
        else
        {
            if(CurrentMotorAction == MOTOR_ACTION_FORWARD)
            {
                rawData[0].value -= 5/5;
                RangefinderData[0][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[0];
            }
            else if(CurrentMotorAction == MOTOR_ACTION_BACKWARD)
            {
                rawData[0].value += 5/5;
                RangefinderData[0][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[0];
            }5;
        }

    }

    if(!rawData[1].valid)
        RangefinderData[1][RANGEFINDER_DATA_BUFFER_SIZE-1] = RangefinderData[1][RANGEFINDER_DATA_BUFFER_SIZE-2];

    if(!rawData[2].valid)
    {
        //the front ragefinder was valid, add the opposite of the front rangefinder delta
        if(rawData[0].valid)
        {
            rawData[2].value = RangefinderData[2][RANGEFINDER_DATA_BUFFER_SIZE-2].value
                    - ( RangefinderData[0][RANGEFINDER_DATA_BUFFER_SIZE-2].value -  RangefinderData[0][RANGEFINDER_DATA_BUFFER_SIZE-3].value );
            RangefinderData[2][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[2];
        }
        else
        {
            if(CurrentMotorAction == MOTOR_ACTION_FORWARD)
            {
                rawData[2].value += 5/5;
                RangefinderData[2][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[2];
            }
            else if(CurrentMotorAction == MOTOR_ACTION_BACKWARD)
            {
                rawData[2].value -= 5/5;
                RangefinderData[2][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[2];
            }
        }
    }
    if(!rawData[3].valid)
        RangefinderData[3][RANGEFINDER_DATA_BUFFER_SIZE-1] = RangefinderData[3][RANGEFINDER_DATA_BUFFER_SIZE-2];
}

void UpdatePosition()
{
    int rangefinderPlusY, rangefinderMinusY, rangefinderPlusX, rangefinderMinusX;
    //offset used to offset the position to the front center of the robot
    int plusXOffset, minusXOffset, plusYOffset, minusYOffset;
    switch(Direction)
    {
        case 0:
            rangefinderPlusY = RANGEFINDER_FRONT;
            rangefinderMinusY = RANGEFINDER_BACK;
            rangefinderPlusX = RANGEFINDER_RIGHT;
            rangefinderMinusX = RANGEFINDER_LEFT;
            plusXOffset = -6;
            minusXOffset = 6;
            plusYOffset = 0;
            minusYOffset = 12;
            break;
        case 1:
            rangefinderPlusY = RANGEFINDER_LEFT;
            rangefinderMinusY = RANGEFINDER_RIGHT;
            rangefinderPlusX = RANGEFINDER_FRONT;
            rangefinderMinusX = RANGEFINDER_BACK;
            plusXOffset = 0;
            minusXOffset = 12;
            plusYOffset = -6;
            minusYOffset = 6;
            break;
        case 2:
            rangefinderPlusY = RANGEFINDER_BACK;
            rangefinderMinusY = RANGEFINDER_FRONT;
            rangefinderPlusX = RANGEFINDER_LEFT;
            rangefinderMinusX = RANGEFINDER_RIGHT;
            plusXOffset = -6;
            minusXOffset = 6;
            plusYOffset = -12;
            minusYOffset = 0;
            break;
        case 3:
            rangefinderPlusY = RANGEFINDER_RIGHT;
            rangefinderMinusY = RANGEFINDER_LEFT;
            rangefinderPlusX = RANGEFINDER_BACK;
            rangefinderMinusX = RANGEFINDER_FRONT;
            plusXOffset = -12;
            minusXOffset = 0;
            plusYOffset = -6;
            minusYOffset = 6;
            break;
    }
    //TODO: take into account the farther value
    if(RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE-1].valid && RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        //offeset the reading to the center of the robot
        DeltaRobotPosition.Y = -RobotPosition.Y;
        if(RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE-1].value < RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE - 1].value)
            RobotPosition.Y = RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE-1].value + minusYOffset;
        else
            RobotPosition.Y = ARENA_LENGTH_0 - RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE-1].value + plusYOffset;
        DeltaRobotPosition.Y += RobotPosition.Y;
    }
    else if(RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        DeltaRobotPosition.Y = -RobotPosition.Y;
        RobotPosition.Y = RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE-1].value + minusYOffset;
        DeltaRobotPosition.Y += RobotPosition.Y;
    }
    else if(RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        DeltaRobotPosition.Y = -RobotPosition.Y;
        RobotPosition.Y = ARENA_LENGTH_0 - RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE-1].value + plusYOffset;
        DeltaRobotPosition.Y += RobotPosition.Y;
    }
    
    if(RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE-1].valid && RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        DeltaRobotPosition.X = -RobotPosition.X;
        //offeset the reading to the center of the robot
        if(RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE-1].value < RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE - 1].value)
            RobotPosition.X = RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE-1].value + minusXOffset;
        else
            RobotPosition.X = ARENA_WIDTH - RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE-1].value + plusXOffset;
        DeltaRobotPosition.X += RobotPosition.X;
    }
    else if(RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        DeltaRobotPosition.X = -RobotPosition.X;
        RobotPosition.X = RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE-1].value + minusXOffset;
        DeltaRobotPosition.X += RobotPosition.X;
    }
    else if(RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        DeltaRobotPosition.X = -RobotPosition.X;
        RobotPosition.X = ARENA_WIDTH - RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE-1].value + plusXOffset;
        DeltaRobotPosition.X += RobotPosition.X;
    }

}

void DriveParallel()
{
    int delta = RangefinderData[RANGEFINDER_FRONT][RANGEFINDER_DATA_BUFFER_SIZE-1].value - RangefinderData[RANGEFINDER_FRONT][RANGEFINDER_DATA_BUFFER_SIZE-2].value;
    //don't crash!
    if(delta > 0)
    {
        EnqueueMotorAction(MOTOR_ACTION_SLIGHT_RIGHT);
        return;
    }
    if(delta < 0)
    {
        EnqueueMotorAction(MOTOR_ACTION_SLIGHT_LEFT);
    }
    else
        EnqueueMotorAction(MOTOR_ACTION_FORWARD);
    
    if(RangefinderData[0][4].value < 4)
        EnqueueMotorAction(MOTOR_ACTION_STOP);
}

void NavigateToTarget()
{
    double robotPositionOnTargetAxis, targetPositionOnTargetAxis, deltaPositionOnTargetAxis;
    double robotPositionOnOtherAxis, targetPositionOnOtherAxis, deltaPositionOnOtherAxis;
    robotPositionOnTargetAxis = TargetPositionAxis == 0 ? RobotPosition.X : RobotPosition.Y;
    targetPositionOnTargetAxis = TargetPositionAxis == 0 ? Cube.X : Cube.Y;
    deltaPositionOnTargetAxis = targetPositionOnTargetAxis - robotPositionOnTargetAxis;
    //reached goal on current axis
    if(deltaPositionOnTargetAxis < 5)
    {
        robotPositionOnOtherAxis = TargetPositionAxis == 0 ? RobotPosition.Y : RobotPosition.X;
        targetPositionOnOtherAxis = TargetPositionAxis == 0 ? Cube.Y : Cube.X;
        deltaPositionOnOtherAxis = targetPositionOnTargetAxis - robotPositionOnTargetAxis;
        //reached goal on both axis
        if(deltaPositionOnOtherAxis < 5)
        {
            if(Cubes >= 4)
                ChangeState(STATE_GOTO_SCORING_ZONE);
            else
                CubeIndex++;
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
    if(State == STATE_DRIVE_PARALLEL)
        DriveParallel();

#ifdef DEBUG
        //optional send motor over uart
        sprintf(UARTWriteBuffer, "%1i%04i%1i%04i", CurrentLeftMotorDirection, CurrentLeftMotorSpeed, CurrentRightMotorDirection, CurrentRightMotorSpeed);
        SendString(3, UARTWriteBuffer);
        sprintf(UARTWriteBuffer, "%03i%03i%03i%03i", UARTReadBuffer[0]&0xFF, UARTReadBuffer[1]&0xFF, UARTReadBuffer[2]&0xFF, UARTReadBuffer[3]&0xFF);
        //sprintf(UARTWriteBuffer, "%03i%03i%03i%03i", RangefinderData[0][4].value, RangefinderData[1][4].value, RangefinderData[2][4].value, RangefinderData[3][4].value);
        SendString(3, UARTWriteBuffer);
        sprintf(UARTWriteBuffer, "%03i%03i%03i%03i\n", RobotPosition.X, RobotPosition.Y, CubeLocation[CubeIndex].X, CubeLocation[CubeIndex].Y);
        SendString(3, UARTWriteBuffer);
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
    togglePin(F3);
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