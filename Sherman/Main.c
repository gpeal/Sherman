#include "Main.h"
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

//Global Variables
unsigned int Time = 0;
char TimeFlag_1ms = 0, TimeFlag1ms = 0, TimeFlag2ms = 0, TimeFlag10ms = 0, TimeFlag100ms = 0, TimeFlag200ms = 0, TimeFlag102_4ms = 0, TimeFlag_5s = 0, TimeFlag1s = 0, TimeFlag5s = 0;
char TimeFlag2_5m = 0;
char ReadArduino = 0;
char RemoteCommand = '\0';
//position variables
struct Position RobotPosition, DeltaRobotPosition;
struct Position CubeLocation[] = { {30, 48},
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
    unsigned int value;
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
int State, StateStartTime = 0;
int SubState = 0, SubStateStartTime = 0;
int Cubes = 0;
extern struct MotorAction MotorActionQueue[];
extern int MotorActionQueueHeadIndex;
extern int CurrentRightMotorSpeed, CurrentRightMotorDirection;
extern int CurrentLeftMotorSpeed, CurrentLeftMotorDirection;

int main(void)
{
    int i, j;
    initialize();
    // Initilize Data in main for convenience of global variable scope.
    initializeData();
    ChangeState(STATE_REMOTE_CONTROL);
    while(1)
    {
        PeriodicFunctions();
    }
}

void ChangeState(int state)
{
    State = state;
    SubState = 0;
    SubStateStartTime = Time;
    StateStartTime = Time;
}

void initializeData()
{
    // RangefinderData
    int i, j;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 5; j++)
        {
           RangefinderData[i][j].value = 213;
        }
    }
}

void InitialRoutine()
{
    switch(SubState)
    {
        case 0:
            EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            SubState++;
        case 1:
            if(0) { //Time - SubStateStartTime > 20000) {
                SubState++;
                SubStateStartTime = Time;
            }
            break;
        default:
            setMotor(MOTOR_WHEEL_LEFT, 1000, 0);
            setMotor(MOTOR_WHEEL_RIGHT, 1000, 0);
            if(Time - SubStateStartTime > 20000) {
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
        if(rawData[i].value >= 200 || rawData[i].value < 1)
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
        else
            RangefinderData[i][RANGEFINDER_DATA_BUFFER_SIZE-1] = RangefinderData[i][RANGEFINDER_DATA_BUFFER_SIZE-2];
    }


    if(AUTO_BRAKE)
        if((unsigned int)(RangefinderData[RANGEFINDER_FRONT][RANGEFINDER_DATA_BUFFER_SIZE-1].value) < 8)
            EnqueueMotorAction(MOTOR_ACTION_STOP);

    /*if(!rawData[0].valid)
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
            };
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
        RangefinderData[3][RANGEFINDER_DATA_BUFFER_SIZE-1] = RangefinderData[3][RANGEFINDER_DATA_BUFFER_SIZE-2];*/
}

int OpponentHomeLocationX()
{
    return (ARENA_WIDTH - HomeLocationX(0));
}

//TOTEST
int HomeLocationX(float timeInFuture)
{
    int startingLocationX = 12;
    float speed = 12.76;
    int arenaLimits = ARENA_WIDTH - 24;
    int distanceTraveled = speed * (Time + timeInFuture);
    int finalLocationX = startingLocationX;
    while (1)
    {
        if ((distanceTraveled - arenaLimits) > 0)
        {
            distanceTraveled -= arenaLimits;
            finalLocationX += arenaLimits;
        }
        else
        {
            finalLocationX += distanceTraveled;
            break;
        }

        if ((distanceTraveled - arenaLimits) > 0)
        {
            distanceTraveled -= arenaLimits;
            finalLocationX -= arenaLimits;
        }
        else
        {
            finalLocationX -= distanceTraveled;
            break;
        }
    }
    return finalLocationX;
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

void RemoteControl()
{
    switch(RemoteCommand)
    {
        case 'w':
            EnqueueMotorAction(MOTOR_ACTION_FORWARD);
            break;
        case 's':
            EnqueueMotorAction(MOTOR_ACTION_BACKWARD);
            break;
        case 'a':
            EnqueueMotorAction(MOTOR_ACTION_TURN_LEFT);
            break;
        case 'd':
            EnqueueMotorAction(MOTOR_ACTION_TURN_RIGHT);
            break;
        case 'e':
            EnqueueMotorAction(MOTOR_ACTION_SLIGHT_RIGHT);
            break;
        case 'q':
            EnqueueMotorAction(MOTOR_ACTION_SLIGHT_LEFT);
            break;
        case ' ':
            EnqueueMotorAction(MOTOR_ACTION_STOP);
            break;
    }
    RemoteCommand = '\0';
}

void NavigateToTarget()
{
    double robotPositionOnTargetAxis, targetPositionOnTargetAxis, deltaPositionOnTargetAxis;
    double robotPositionOnOtherAxis, targetPositionOnOtherAxis, deltaPositionOnOtherAxis;
    robotPositionOnTargetAxis = TargetPositionAxis == 0 ? RobotPosition.X : RobotPosition.Y;
    targetPositionOnTargetAxis = TargetPositionAxis == 0 ? Cube.X : Cube.Y;
    deltaPositionOnTargetAxis = targetPositionOnTargetAxis - robotPositionOnTargetAxis;
    //reached goal on current axis
    if(abs(deltaPositionOnTargetAxis) < 3)
    {
        //switch axis
        TargetPositionAxis = !TargetPositionAxis;
        
        robotPositionOnOtherAxis = TargetPositionAxis == 0 ? RobotPosition.X : RobotPosition.Y;
        targetPositionOnOtherAxis = TargetPositionAxis == 0 ? Cube.X : Cube.Y;
        deltaPositionOnOtherAxis = targetPositionOnTargetAxis - robotPositionOnTargetAxis;
        //reached goal on both axis
        if(abs(deltaPositionOnOtherAxis) < 3)
        {
            if(Cubes >= 4)
                ChangeState(STATE_GOTO_SCORING_ZONE);
            else
            {
                CubeIndex++;
                TargetPositionAxis = 0;
            }
        }
    }
}

void RunEvery_1ms()
{
    switch(State)
    {
        case STATE_INITIAL_ROUTINE:
            InitialRoutine();
            break;
        case STATE_FIND_CUBES:
            break;
    }
}

void RunEvery1ms()
{
    //if(State != STATE_INITIAL_ROUTINE)
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
    if(State == STATE_REMOTE_CONTROL)
        RemoteControl();
    else if(RemoteCommand == '`')
        ChangeState(STATE_REMOTE_CONTROL);
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
        //sprintf(UARTWriteBuffer, "%03i%03i%03i%03i", UARTReadBuffer[0]&0xFF, UARTReadBuffer[1]&0xFF, UARTReadBuffer[2]&0xFF, UARTReadBuffer[3]&0xFF);
        sprintf(UARTWriteBuffer, "%03i%03i%03i%03i", RangefinderData[0][4].value, RangefinderData[1][4].value, RangefinderData[2][4].value, RangefinderData[3][4].value);
        SendString(3, UARTWriteBuffer);
        sprintf(UARTWriteBuffer, "%03i%03i%03i%03i%1i%1i%1i\n", RobotPosition.X, RobotPosition.Y, Cube.X, Cube.Y, Direction, State, CurrentMotorAction);
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

void RunEvery2_5m()
{
    //ChangeState(STATE_GOTO_SCORING_ZONE);
}

void PeriodicFunctions()
{
    if(TimeFlag_1ms)
    {
        TimeFlag_1ms = 0;
        RunEvery_1ms();
    }

    if(TimeFlag1ms)
    {
        TimeFlag1ms = 0;
        RunEvery1ms();
    }

    if(TimeFlag2ms)
    {
        TimeFlag2ms = 0;
        RunEvery2ms();
    }

    if(TimeFlag10ms)
    {
        TimeFlag10ms = 0;
        RunEvery10ms();
    }

    if(TimeFlag102_4ms)
    {
        TimeFlag102_4ms = 0;
        RunEvery102_4ms();
    }

    if(TimeFlag100ms)
    {
        TimeFlag100ms = 0;
        RunEvery100ms();
    }

    if(TimeFlag200ms)
    {
        TimeFlag200ms = 0;
        RunEvery200ms();
    }

    if(TimeFlag_5s)
    {
        TimeFlag_5s = 0;
        RunEvery_5s();
    }
    if(TimeFlag1s)
    {
        TimeFlag1s = 0;
        RunEvery1s();
    }

    if(TimeFlag5s)
    {
        TimeFlag5s = 0;
        RunEvery5s();
    }
    if(TimeFlag2_5m)
    {
        TimeFlag2_5m = 0;
        RunEvery2_5m();
    }
}

void delay_ms(int ms)
{
    int startTime = Time;
    while(Time < startTime + ms*10);
}

void __ISR(_TIMER_1_VECTOR, ipl1) Timer1Isr(void)
{
    Time++;
    if(Time%1 < 1)
        TimeFlag_1ms = 1;
    if(Time%10 < 1)
        TimeFlag1ms = 1;
    if(Time%20 < 1)
        TimeFlag2ms = 1;
    if(Time%100 < 1)
        TimeFlag10ms = 1;
    if(Time%1000 < 1)
        TimeFlag100ms = 1;
    if(Time%1024 < 1)
        TimeFlag102_4ms = 1;
    if(Time%2000 < 1)
        TimeFlag200ms = 1;
    if(Time%5000 < 1)
    {
        TimeFlag_5s = 1;
        togglePin(A4);
    }
    if(Time%10000 < 1)
        TimeFlag1s = 1;
    if(Time%50000 < 1)
        TimeFlag5s = 1;
    if(Time%1500000)
        TimeFlag2_5m = 1;
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

// UART 3 interrupt handler
// it is set at priority level 2
void __ISR(_UART_3_VECTOR, ipl1) IntUart3Handler(void)
{
    // Is this an RX interrupt?
    if(INTGetFlag(INT_SOURCE_UART_RX(UART3)))
    {
        RemoteCommand = ReadCharacter(3);
        togglePin(A5);
        // Clear the RX interrupt Flag
       INTClearFlag(INT_SOURCE_UART_RX(UART3)); 
    }

    // We don't care about TX interrupt
    if(INTGetFlag(INT_SOURCE_UART_TX(UART3))) {
        INTClearFlag(INT_SOURCE_UART_TX(UART3));
    }
}