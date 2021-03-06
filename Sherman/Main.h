#ifndef MAIN_H
#define MAIN_H
#include "Motor.h"

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
#define AUTO_BRAKE 0

#define STATE_FIND_CUBES 0
#define STATE_INITIAL_ROUTINE 1
void InitialRoutine();
#define STATE_GO_HOME 2
#define STATE_REMOTE_CONTROL 4
#define STATE_INITIALIZE_NAVIGATION 5
#define STATE_DUMP_CUBES 6
#define STATE_LEAVE_SCORING_ZONE 7

extern struct Position RobotPosition, DeltaRobotPosition;
extern unsigned int Time;

//function prototypes
void PeriodicFunctions();
void delay_ms(int ms);
void ParseUartData();
void ReadAndValidateRangefinders();
void SetStartPosition();
void UpdatePosition();
void ChangeState(int state);
void RemoteControl();
int HomeLocationX(float timeInFuture);
int OpponentHomeLocationX();
void initializeData();
void Initialization();
void StartDumpCubes();
void EndDumpCubes();
void SendDebugInformation();
void NavigateToTarget(struct Position);
void FindCubes();



#endif