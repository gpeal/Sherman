#include "Initialize.h"
#include <plib.h>
#include <limits.h>
#include "fft.h"

//Global Variables
unsigned int time = 0;
int timeFlag_1ms = 0, timeFlag1ms = 0, timeFlag2ms = 0, timeFlag10ms = 0, timeFlag200ms = 0, timeFlag102_4ms = 0, timeFlag1s = 0, timeFlag5s = 0;


int main(void)
{
    int analogSum = 0;
    double voltage = 0, distance = 0, analogValue = 0;
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
            analogSum += readAnalogIn(0);
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
            analogValue = analogSum/1000;
            analogSum = 0;
            voltage = ((double)analogValue/1024)*3.3;
            distance = voltage/.0064;
            sprintf(LCDBuffer, "Analog: %f", analogValue);
            LCDClear(0);
            LCDWriteString(LCDBuffer, 1, 1);
            sprintf(LCDBuffer, "Distance: %f", distance);
            LCDWriteString(LCDBuffer, 2, 1);
        }

        if(timeFlag5s)
        {
            timeFlag5s = 0;
        }
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