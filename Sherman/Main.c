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
struct Position CubeLocation[] = { {24, 12},
                                   {48, 12},
                                   {6, 30},
                                   {54, 30},
                                   {30, 48},
                                   {42, 48},
                                   {24, 60},
                                   {36, 60},
                                   {48, 60},
                                   {30, 72},
                                   {42, 72}};
struct Position HomeLocation;
int CubeIndex = 0;
#define Cube CubeLocation[CubeIndex]
//the current coordinate value we are trying to reach
//0 for X 1 for Y
int TargetNavigationAxis = 0;
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
int CrashState = 0;
extern struct MotorAction MotorActionQueue[];
extern int MotorActionQueueHeadIndex;
extern int CurrentRightMotorSpeed, CurrentRightMotorDirection;
extern int CurrentLeftMotorSpeed, CurrentLeftMotorDirection;

int main(void)
{
    initialize();
    // Initilize Data in main for convenience of global variable scope.
    initializeData();
    ChangeState(STATE_INITIALIZE_NAVIGATION);
    while(1)
    {
        PeriodicFunctions();
    }
}

void InitializeNavigation()
{
    switch(SubState)
    {
        case 0:
            if(UARTReadBuffer[3] != 0)
            {
                SubState++;
                SubStateStartTime = Time;
            }
            break;
        case 1:
            SetStartPosition();
            ChangeState(STATE_REMOTE_CONTROL);
            break;
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
    memset(UARTReadBuffer, 0, ARDUINO_BUFFER_SIZE);
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

void ReadAndValidateRangefinders()
{
    struct RangefinderReading rawData[4];
    rawData[0].value = UARTReadBuffer[0] & 0xFF;
    rawData[0].valid = 1;
    rawData[1].value = UARTReadBuffer[1] & 0xFF;
    rawData[1].valid = 1;
    rawData[2].value = UARTReadBuffer[2] & 0xFF;
    rawData[2].valid = 1;
    rawData[3].value = UARTReadBuffer[3] & 0xFF;
    rawData[3].valid = 1;
    int i;
    for(i = 0; i < 4; i++)
    {
        if(rawData[i].value >= 200 || rawData[i].value < 1)
            rawData[i].valid = 0;
    }
    // Direction Agnosticity code.
    int rangefinderPlusY, rangefinderMinusY, rangefinderPlusX, rangefinderMinusX;
    switch(Direction)
    {
        case 0:
            rangefinderPlusY = RANGEFINDER_FRONT;
            rangefinderMinusY = RANGEFINDER_BACK;
            rangefinderPlusX = RANGEFINDER_RIGHT;
            rangefinderMinusX = RANGEFINDER_LEFT;
            break;
        case 1:
            rangefinderPlusY = RANGEFINDER_LEFT;
            rangefinderMinusY = RANGEFINDER_RIGHT;
            rangefinderPlusX = RANGEFINDER_FRONT;
            rangefinderMinusX = RANGEFINDER_BACK;
            break;
        case 2:
            rangefinderPlusY = RANGEFINDER_BACK;
            rangefinderMinusY = RANGEFINDER_FRONT;
            rangefinderPlusX = RANGEFINDER_LEFT;
            rangefinderMinusX = RANGEFINDER_RIGHT;
            break;
        case 3:
            rangefinderPlusY = RANGEFINDER_RIGHT;
            rangefinderMinusY = RANGEFINDER_LEFT;
            rangefinderPlusX = RANGEFINDER_BACK;
            rangefinderMinusX = RANGEFINDER_FRONT;
            break;
    }

    //VALIDATE Y

    // No valid data
    /*if (!rawData[rangefinderPlusY].valid && !rawData[rangefinderMinusY].valid)
    {
    }
    // 1 valid data
    else if (rawData[rangefinderPlusY].valid ^ rawData[rangefinderMinusY].valid)
    {
        if(rawData[rangefinderPlusY].valid)
        {
            int delta = abs(rawData[rangefinderPlusY].value - RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE - 1].value);
            if (delta < 4 || abs(delta - 24) < 3 || abs(delta - 48) < 6)
            {
            }
            else
            {
                rawData[rangefinderPlusY].valid = 0;
            }
        }
        else if(rawData[rangefinderMinusY].valid)
        {
            int delta = abs(rawData[rangefinderMinusY].value - RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE - 1].value);
            if (delta < 4 || abs(delta - 24) < 3 || abs(delta - 48) < 6)
            {
            }
            else
            {
                rawData[rangefinderMinusY].valid = 0;
            }
        }
    }
    // All valid Data
    else if (rawData[rangefinderPlusY].valid && rawData[rangefinderMinusY].valid)
    {
        int length = rawData[rangefinderPlusY].value + rawData[rangefinderMinusY].value + 12;
        //the length of the arena adds up correctly
        if(abs(length-ARENA_LENGTH_0) < 10 || abs(length-ARENA_LENGTH_1) < 10 || abs(length-ARENA_LENGTH_2) < 10)
        {
            //mark the further one invalid
            if (rawData[rangefinderPlusY].value < rawData[rangefinderMinusY].value)
            {
                rawData[rangefinderMinusY].valid = 0;
            }
            else
            {
                rawData[rangefinderPlusY].valid = 0;
            }
            //basically check if the delta was small or the size of one or both scoring zones
            //if it is, set the other one to invalid
            int plusDelta = abs(rawData[rangefinderPlusY].value - RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE - 1].value);
            int minusDelta = abs(rawData[rangefinderMinusY].valid - RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE - 1].value);
            if (plusDelta < 4 || abs(plusDelta - 24) < 3 || abs(plusDelta - 48) < 6)
            {
                rawData[rangefinderMinusY].valid =0;
            }
            else if (minusDelta < 4 || abs(minusDelta - 24) < 3 || abs(minusDelta - 48) < 6)
            {
                rawData[rangefinderPlusY].valid =0;
            }
        }
        else
        {
            int meanIndex;
            float meanPlusY = 0;
            float meanMinusY = 0;
            for(meanIndex=RANGEFINDER_DATA_BUFFER_SIZE - 1; meanIndex >= RANGEFINDER_DATA_BUFFER_SIZE - 3; meanIndex--)
            {
                meanPlusY += RangefinderData[rangefinderPlusY][meanIndex].value;
                meanMinusY += RangefinderData[rangefinderMinusY][meanIndex].value;
            }
            meanPlusY = meanPlusY/3;
            meanMinusY = meanMinusY/3;

            if(abs(meanPlusY - rawData[rangefinderPlusY].value) < abs(meanMinusY - rawData[rangefinderMinusY].value))
            {
                rawData[rangefinderMinusY].valid = 0;
            }
            else
            {
                rawData[rangefinderPlusY].valid = 0;
            }
        }
    }


    //VALIDATE X

    // No valid data
    if (!rawData[rangefinderPlusX].valid && !rawData[rangefinderMinusX].valid)
    {
    }
    // 1 valid data
    else if (rawData[rangefinderPlusX].valid ^ rawData[rangefinderMinusX].valid)
    {
        if(rawData[rangefinderPlusX].valid)
        {
            int delta = abs(rawData[rangefinderPlusX].value - RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE - 1].value);
            if (delta > 4)
            {
                rawData[rangefinderPlusX].valid = 0;
            }
        }
        else if(rawData[rangefinderMinusX].valid)
        {
            int delta = abs(rawData[rangefinderMinusX].value - RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE - 1].value);
            if (delta > 4)
            {
                rawData[rangefinderMinusX].valid = 0;
            }
        }
    }
    // All valid Data
    else if (rawData[rangefinderPlusX].valid && rawData[rangefinderMinusX].valid)
    {
        int width = rawData[rangefinderPlusX].value + rawData[rangefinderMinusX].value + 10;
        //the length of the arena adds up correctly
        if(abs(width-ARENA_WIDTH) < 10)
        {
            //mark the further one invalid
            if (rawData[rangefinderPlusX].value < rawData[rangefinderMinusX].value)
            {
                rawData[rangefinderMinusX].valid = 0;
            }
            else
            {
                rawData[rangefinderPlusX].valid = 0;
            }
            //basically check if the delta was small or the size of one or both scoring zones
            //if it is, set the other one to invalid
            int plusDelta = abs(rawData[rangefinderPlusX].value - RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE - 1].value);
            int minusDelta = abs(rawData[rangefinderMinusX].value - RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE - 1].value);
            if (plusDelta < 4)
            {
                rawData[rangefinderMinusX].valid =0;
            }
            else if (minusDelta < 4 || abs(minusDelta - 24) < 3 || abs(minusDelta - 48) < 6)
            {
                rawData[rangefinderPlusX].valid =0;
            }
        }
        else
        {
            int meanIndex;
            float meanPlusX = 0;
            float meanMinusX = 0;
            for(meanIndex=RANGEFINDER_DATA_BUFFER_SIZE - 1; meanIndex >= RANGEFINDER_DATA_BUFFER_SIZE - 3; meanIndex--)
            {
                meanPlusX += RangefinderData[rangefinderPlusX][meanIndex].value;
                meanMinusX += RangefinderData[rangefinderMinusX][meanIndex].value;
            }
            meanPlusX = meanPlusX/3;
            meanMinusX = meanMinusX/3;

            if(abs(meanPlusX - rawData[rangefinderPlusX].value) < abs(meanMinusX - rawData[rangefinderMinusX].value))
            {
                rawData[rangefinderMinusX].valid = 0;
            }
            else
            {
                rawData[rangefinderPlusX].valid = 0;
            }
        }
    }*/


    int shiftIndex;
    for(i = 0; i < 4; i++)
    {
        for (shiftIndex = 0; shiftIndex < RANGEFINDER_DATA_BUFFER_SIZE - 1; shiftIndex++)
        {
            RangefinderData[i][shiftIndex] = RangefinderData[i][shiftIndex + 1];
        }
    }
    for(i = 0; i < 4; i++)
    {
        RangefinderData[i][RANGEFINDER_DATA_BUFFER_SIZE-1] = rawData[i];
    }
}

int OpponentHomeLocationX()
{
    return (ARENA_WIDTH - HomeLocationX(0));
}

//TOTEST
int HomeLocationX(float timeInFuture)
{
    int startingLocationX = 12;
    float speed = 0.8;
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

void SetStartPosition()
{
    RobotPosition.X = RangefinderData[3][RANGEFINDER_DATA_BUFFER_SIZE - 1].value + 6;
    RobotPosition.Y = RangefinderData[2][RANGEFINDER_DATA_BUFFER_SIZE - 1].value + 6;
}

void UpdatePosition()
{
    int rangefinderPlusY, rangefinderMinusY, rangefinderPlusX, rangefinderMinusX;
    //offset used to offset the position to the front center of the robot
    int plusXOffset, minusXOffset, plusYOffset, minusYOffset;
    plusXOffset = -6;
    minusXOffset = 6;
    plusYOffset = -6;
    minusYOffset = 6;
    switch(Direction)
    {
        case 0:
            rangefinderPlusY = RANGEFINDER_FRONT;
            rangefinderMinusY = RANGEFINDER_BACK;
            rangefinderPlusX = RANGEFINDER_RIGHT;
            rangefinderMinusX = RANGEFINDER_LEFT;
            break;
        case 1:
            rangefinderPlusY = RANGEFINDER_LEFT;
            rangefinderMinusY = RANGEFINDER_RIGHT;
            rangefinderPlusX = RANGEFINDER_FRONT;
            rangefinderMinusX = RANGEFINDER_BACK;
            break;
        case 2:
            rangefinderPlusY = RANGEFINDER_BACK;
            rangefinderMinusY = RANGEFINDER_FRONT;
            rangefinderPlusX = RANGEFINDER_LEFT;
            rangefinderMinusX = RANGEFINDER_RIGHT;
            break;
        case 3:
            rangefinderPlusY = RANGEFINDER_RIGHT;
            rangefinderMinusY = RANGEFINDER_LEFT;
            rangefinderPlusX = RANGEFINDER_BACK;
            rangefinderMinusX = RANGEFINDER_FRONT;
            break;
    }

    struct Position newPosition;
    if(RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        newPosition.X = RangefinderData[rangefinderMinusX][RANGEFINDER_DATA_BUFFER_SIZE-1].value + 6;
    }

    else if(RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        newPosition.X = (ARENA_WIDTH - RangefinderData[rangefinderPlusX][RANGEFINDER_DATA_BUFFER_SIZE-1].value) - 6;
    }

    if(RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        newPosition.Y = RangefinderData[rangefinderMinusY][RANGEFINDER_DATA_BUFFER_SIZE-1].value + 6;
        //if(abs(newPosition.X - HomeLocationX(0)) <= 12)
        //    newPosition.Y -= 12;
    }
    else if(RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE-1].valid)
    {
        newPosition.Y = (ARENA_LENGTH_0 - RangefinderData[rangefinderPlusY][RANGEFINDER_DATA_BUFFER_SIZE-1].value) - 6;
        //if(abs(newPosition.X - OpponentHomeLocationX()) <= 12)
        //    newPosition.Y += 12;
    }

    DeltaRobotPosition.X = -RobotPosition.X;
    DeltaRobotPosition.Y = -RobotPosition.Y;
    RobotPosition.X = newPosition.X;
    RobotPosition.Y = newPosition.Y;
    DeltaRobotPosition.X += RobotPosition.X;
    DeltaRobotPosition.Y += RobotPosition.Y;

    /*struct Position deltaPosition;
    deltaPosition.X = RobotPosition.X - newPosition.X;
    deltaPosition.Y = RobotPosition.Y - newPosition.Y;

    if((Direction == 1 || Direction == 3) && abs(deltaPosition.X) < 10)
    {
        RobotPosition.X = newPosition.X;
    }
    else if((Direction == 1 || Direction == 3) && abs(deltaPosition.X) >= 10)
    {
        RobotPosition.X = (int)((double)(2 * RobotPosition.X + newPosition.X)/3);
    }
    else if((Direction == 0 || Direction == 2) && abs(deltaPosition.X) < 3)
    {
        RobotPosition.X = newPosition.X;
    }
    else if((Direction == 0 || Direction == 2) && abs(deltaPosition.X) >= 3)
    {
        RobotPosition.X = (int)((double)(2 * RobotPosition.X + newPosition.X) / 3);
    }

    if((Direction == 1 || Direction == 3) && abs(deltaPosition.Y) < 3)
    {
        RobotPosition.Y = newPosition.Y;
    }
    else if((Direction == 1 || Direction == 3) && abs(deltaPosition.Y) >= 3)
    {
        RobotPosition.Y = (int)((double)(2 * RobotPosition.Y + newPosition.Y) / 3);
    }
    else if((Direction == 0 || Direction == 2) && abs(deltaPosition.Y) < 10)
    {
        RobotPosition.Y = newPosition.Y;
    }
    else if((Direction == 0 || Direction == 2) && abs(deltaPosition.Y) >= 10)
    {
        RobotPosition.Y = (int)((double)(2 * RobotPosition.Y + newPosition.Y) / 3);
    }*/
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
        case 'z':
            EnqueueMotorAction(MOTOR_ACTION_TURN_LEFT_90);
            break;
        case 'c':
            EnqueueMotorAction(MOTOR_ACTION_TURN_RIGHT_90);
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
        case '[':
            StartDumpCubes();
            break;
        case ']':
            EndDumpCubes();
            break;
        case '0':
            ChangeState(STATE_FIND_CUBES);
            break;
    }
    RemoteCommand = '\0';
}

void GoHome()
{
    switch(SubState)
    {
        case 0:
            HomeLocation.X = HomeLocationX(300000);
            HomeLocation.Y = 12;
            SubState++;
            break;
        case 1:
            NavigateToTarget(HomeLocation);
            if(abs(RobotPosition.X - HomeLocation.X) < 3 && abs(RobotPosition.Y - HomeLocation.Y < 3))
            {
                SubState++;
                SubStateStartTime = Time;
            }
            break;
        case 2:
            if(abs(RobotPosition.X - HomeLocationX(Time)) < 6)
                EnqueueMotorAction(MOTOR_ACTION_DRIVE_INTO_SCORING_ZONE);
            if(SubStateStartTime + 400000 > Time)
            {
                SubState++;
                SubStateStartTime = Time;
            }
            break;
        case 3:
            StartDumpCubes();
            delay_ms(30000);
            EndDumpCubes();
            delay_ms(200000);
            StartDumpCubes();
            delay_ms(30000);
            EndDumpCubes();
            delay_ms(200000);
            StartDumpCubes();
            delay_ms(30000);
            EndDumpCubes();
            delay_ms(200000);
            SubState++;
            SubStateStartTime = Time;
        case 4:
            EnqueueMotorAction(MOTOR_ACTION_BACK_OUT_OF_SCORING_ZONE);
            EnqueueMotorAction(MOTOR_ACTION_TURN_LEFT_90);
            EnqueueMotorAction(MOTOR_ACTION_TURN_LEFT_90);
            ChangeState(STATE_FIND_CUBES);
            break;
    }
}

void SendDebugInformation()
{
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

void NavigateToTarget(struct Position target)
{
    double robotPositionOnTargetAxis, targetPositionOnTargetAxis, deltaPositionOnTargetAxis;
    robotPositionOnTargetAxis = TargetNavigationAxis == 0 ? RobotPosition.X : RobotPosition.Y;
    targetPositionOnTargetAxis = TargetNavigationAxis == 0 ? target.X : target.Y;
    deltaPositionOnTargetAxis = targetPositionOnTargetAxis - robotPositionOnTargetAxis;
    //reached goal on current axis
    if(abs(deltaPositionOnTargetAxis) < 3)
    {
        //switch axis
        TargetNavigationAxis = !TargetNavigationAxis;
    }
}

void FindCubes()
{
    
    NavigateToTarget(Cube);
    if(abs(RobotPosition.X - Cube.X) < 3 || abs(RobotPosition.Y - Cube.Y) < 3)
    {
        EnqueueMotorAction(MOTOR_ACTION_DRIVE_OVER_CUBE);
        Cubes++;
    }
    if(Cubes >= 3)
        ChangeState(STATE_GO_HOME);
}

void StartDumpCubes()
{
    digitalWrite(F3, 1);
}

void EndDumpCubes()
{
    digitalWrite(F3, 0);
}

void DumpCubes()
{
    switch(SubState)
    {
    case 0:
        StartDumpCubes();
        if(Time - SubStateStartTime > 30000) {
            SubState++;
            SubStateStartTime = Time;
        }
        break;
    case 1:
        EndDumpCubes();
        if(Time - SubStateStartTime > 30000) {
            ChangeState(STATE_FIND_CUBES); //STATE_LEAVE_SCORING_ZONE
        }
    }
}

void RunEvery1ms()
{
    if(State != STATE_INITIAL_ROUTINE)
        UpdateMotors();
    if(State == STATE_INITIAL_ROUTINE)
        InitialRoutine();
}

void RunEvery200ms()
{
    ReadAndValidateRangefinders(); 
    if(State != STATE_INITIALIZE_NAVIGATION)
        UpdatePosition();
    else
        InitializeNavigation();
    
    if(State == STATE_REMOTE_CONTROL)
        RemoteControl();
    else if(RemoteCommand == '`')
        ChangeState(STATE_REMOTE_CONTROL);

    if(State == STATE_FIND_CUBES)
            FindCubes();

    if(State == STATE_GO_HOME)

    
    SendDebugInformation();

    if(AUTO_BRAKE)
    {
        if((unsigned int)(RangefinderData[RANGEFINDER_FRONT][RANGEFINDER_DATA_BUFFER_SIZE-1].value) < 8)
        {
            PushMotorAction(MOTOR_ACTION_STOP);
            CrashState = 1;
        }
        if(CrashState)
        {
            CrashState = 0;
            DequeueMotorAction();
        }
    }
    
}

void RunEvery2ms(){}
void RunEvery10ms(){}
void RunEvery102_4ms(){}
void RunEvery_5s(){}
void RunEvery1s(){}
void RunEvery5s(){}
void RunEvery_1ms(){}
void RunEvery100ms() {}
void RunEvery2_5m() {}
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