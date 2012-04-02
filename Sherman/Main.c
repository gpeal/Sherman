#include "Initialize.h"
#include <plib.h>
#include <limits.h>

//Global Variables
unsigned int time = 0;
int timeFlag1ms = 0, timeFlag10ms = 0, timeFlag200ms = 0, timeFlag1s = 0;
int main(void)
{
    initialize();
    
    while(1)
    {

        if(timeFlag1ms)
        {
            timeFlag1ms = 0;

        }

        if(timeFlag10ms)
        {
            timeFlag10ms = 0;
            
        }

        if(timeFlag200ms)
        {
            timeFlag200ms = 0;
        }

        if(timeFlag1s)
        {
            timeFlag1s = 0;
        }
    }
}

void __ISR(_TIMER_1_VECTOR, ipl1) Timer1Isr(void)
{
    time++;
    if(time%1 < 1)
        timeFlag1ms = 1;
    if(time%10 < 1)
        timeFlag10ms = 1;
    if(time%200 < 1)
        timeFlag200ms = 1;
    if(time%1000 < 1)
        timeFlag1s = 1;
    mT1ClearIntFlag();
}