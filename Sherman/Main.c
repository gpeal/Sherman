#include "Initialize.h"
#include <plib.h>
#include <limits.h>
#include "fft.h"
#include "Uart.h"
#include "StateMachine.h"
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


//Global Variables
unsigned int time = 0;
char timeFlag_1ms = 0, timeFlag1ms = 0, timeFlag2ms = 0, timeFlag10ms = 0, timeFlag100ms = 0, timeFlag200ms = 0, timeFlag102_4ms = 0, timeFlag_5s = 0, timeFlag1s = 0, timeFlag5s = 0;

//position variables
struct Position {
    double X;
    double Y;
};
struct Position currentPosition, targetPosition;
struct Position targetPositions[] = { {45, 50},
                                      {23, 37},
                                      {23, 15},
                                      {15, 31},
                                      {32, 45},
                                      {45, 36}};
int targetPositionsIndex = 0;
double RangefinderData[4];
double RangefinderDataBuffer[10];
int RangefinderDataBufferIndex = 0;
char movementDirection = 0;
int movementSpeed = 1024;

char data = -1;

//Other global variables
extern int State;
int currentNumberOfCubes = 0;

char map[240][120];

//function prototypes
void PeriodicFunctions();
void delay_ms(int ms);

int main(void)
{
    char data;
    initialize();
    digitalWrite(A5, 1);
    while(1)
    {
        PeriodicFunctions();

        if(DataRdyUART2())
        {
            ReadString(2);
            SendString(1, UARTReadBuffer);
        }

    }
}

void delay_ms(int ms)
{
    int startTime = time;
    while(time < startTime + ms*10);
}

void RunEvery_1ms()
{

}

void RunEvery1ms()
{
    if(State != STATE_INITIALIZATION)
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
    SendString(2, "Hello from PIC!\n");
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
/*void __ISR(_UART2_VECTOR, ipl2) IntUart2Handler(void)
{
    // Is this an RX interrupt?
    if(mU2RXGetIntFlag())
    {
        // Clear the RX interrupt Flag
        mU2RXClearIntFlag();
        data = (char)ReadUART2();
        while(BusyUART2());*/ /* Wait till the UART transmitter is free. */
        /*SendCharacter(1, data);
    }

    // We don't care about TX interrupt
    if ( mU2TXGetIntFlag() )
    {
            mU2TXClearIntFlag();
    }
}*/