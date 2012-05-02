#include "Initialize.h"
#include <plib.h>
#include <limits.h>
#include "fft.h"
#include "I2C.h"

//Global Variables
unsigned int time = 0;
int timeFlag_1ms = 0, timeFlag1ms = 0, timeFlag2ms = 0, timeFlag10ms = 0, timeFlag200ms = 0, timeFlag102_4ms = 0, timeFlag1s = 0, timeFlag5s = 0;

int main(void)
{
    unsigned char dacVoltage = 0;
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
            sprintf(LCDBuffer, "Data bit: %i", dacVoltage);
            LCDWriteString(LCDBuffer, 1, 1);

            I2CWrite(DAC, dacVoltage);
            dacVoltage = dacVoltage == 0 ? 255 : 0;
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