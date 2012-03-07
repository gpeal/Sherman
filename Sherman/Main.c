#include "Initialize.h"
#include <plib.h>

unsigned int Time = 0;
char TimeFlag500ms=0, TimeFlag1s=0, TimeFlag2s=0;

int main(void)
{
    initialize();
    while(1)
    {
        //check time flags
        if(TimeFlag500ms)
        {
            TimeFlag500ms = 0;
            LATAbits.LATA0 = !LATAbits.LATA0;
        }
        if(TimeFlag1s)
        {
            TimeFlag1s = 0;
            LATAbits.LATA1 = !LATAbits.LATA1;
        }
        if(TimeFlag2s)
        {
            TimeFlag2s = 0;
            LATAbits.LATA2 = !LATAbits.LATA2;
        }
    }
}

void __ISR(_TIMER_1_VECTOR, ipl1) TimerIsr(void)
{
    Time++;
    if(!(Time%5000))
    {
        TimeFlag500ms = 1;
        if(!(Time%10000))
        {
            TimeFlag1s = 1;
            if(!(Time%20000))
            {
                TimeFlag2s = 1;
            }
        }
    }
    mT1ClearIntFlag();
}