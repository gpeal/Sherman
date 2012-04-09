#include "Initialize.h"
#include <plib.h>
#include <limits.h>
#include "fft.h"
#include "Motor.h"

//Global Variables
unsigned int time = 0;
int timeFlag_1ms = 1, timeFlag1ms = 1, timeFlag2ms = 1, timeFlag10ms = 1, timeFlag200ms = 1, timeFlag102_4ms = 1, timeFlag1s = 1, timeFlag5s = 1;
double analogBuffer[1024];
double analogFFT[1024];


int main(void)
{
    int directionState = 0;
    
    initialize();

    while(1)
    {

        if(timeFlag_1ms)
        {
            timeFlag_1ms = 0;
        }

        if(timeFlag1ms)
        {
            timeFlag1ms = 0;
        }

        if(timeFlag2ms)
        {
            timeFlag2ms = 0;
        }

        if(timeFlag10ms)
        {
            timeFlag10ms = 0; 
        }

        if(timeFlag102_4ms)
        {
            timeFlag102_4ms = 0;
        }

        if(timeFlag200ms)
        {
            timeFlag200ms = 0;
        }

        if(timeFlag1s)
        {
            timeFlag1s = 0;
            LATAbits.LATA4 = !LATAbits.LATA4;
        }

        if(timeFlag5s)
        {
            timeFlag5s = 0;
            LATAbits.LATA5 = !LATAbits.LATA5;
            switch(directionState) {
                case 0:
                    setMotor(MOTOR_WHEEL_RIGHT, 1000, 1);
                    setMotor(MOTOR_WHEEL_LEFT, 1000, 1);
                    LCDClear(0);
                    LCDWriteString("Forward", 1, 1);
                    break;
                case 1:
                    setMotor(MOTOR_WHEEL_RIGHT, 1000, 0);
                    setMotor(MOTOR_WHEEL_LEFT, 1000, 0);
                    LCDClear(0);
                    LCDWriteString("Backward", 1, 1);
                    break;
                case 2:
                    setMotor(MOTOR_WHEEL_RIGHT, 0, 1);
                    setMotor(MOTOR_WHEEL_LEFT, 1000, 1);
                    LCDClear(0);
                    LCDWriteString("Forward Right", 1, 1);
                    break;
                case 3:
                    setMotor(MOTOR_WHEEL_RIGHT, 0, 1);
                    setMotor(MOTOR_WHEEL_LEFT, 1000, 0);
                    LCDClear(0);
                    LCDWriteString("Backward Right", 1, 1);
                    break;
                case 4:
                    setMotor(MOTOR_WHEEL_RIGHT, 1000, 1);
                    setMotor(MOTOR_WHEEL_LEFT, 0, 1);
                    LCDClear(0);
                    LCDWriteString("Forward Left", 1, 1);
                    break;
                case 5:
                    setMotor(MOTOR_WHEEL_RIGHT, 1000, 0);
                    setMotor(MOTOR_WHEEL_LEFT, 0, 1);
                    LCDClear(0);
                    LCDWriteString("Backward Left", 1, 1);
                    break;
            }
            directionState++;
            if (directionState > 5)
                directionState = 0;
        }
    }
}

void __ISR(_TIMER_1_VECTOR, ipl3) Timer1Isr(void)
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
    if(time%1024 < 1)
        timeFlag102_4ms = 1;
    if(time%2000 < 1)
        timeFlag200ms = 1;
    if(time%10000 < 1)
        timeFlag1s = 1;
    if(time%50000 < 1)
        timeFlag5s = 1;
    mT1ClearIntFlag();
}