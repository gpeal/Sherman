#include "Initialize.h"
#include <plib.h>
#include <limits.h>
#include "fft.h"

//Global Variables
unsigned int time = 0;
int timeFlag_1ms = 0, timeFlag1ms = 0, timeFlag2ms = 0, timeFlag10ms = 0, timeFlag200ms = 0, timeFlag102_4ms = 0, timeFlag1s = 0, timeFlag5s = 0;
double analogBuffer[1024];
double analogFFT[1024];


int main(void)
{
    int analogValue, i;
    int analogBufferIndex = 0;
    //the sum of the fft in the designated region
    double fftSum;
    initialize();
    laserOn(1);
    sprintf(LCDBuffer, "Initializing...");
    LCDWriteString(LCDBuffer, 1, 1);
    PORTAbits.RA4 = 0;
    PORTAbits.RA5 = 0;
    
    while(1)
    {

        if(timeFlag_1ms)
        {
            timeFlag_1ms = 0;
            analogValue = readAnalogIn(0);
            analogBuffer[analogBufferIndex] = analogValue;
            analogBufferIndex++;
        }

        if(timeFlag1ms)
        {
            timeFlag1ms = 0;
            
        }

        if(timeFlag2ms)
        {
            timeFlag2ms = 0;
            toggleLaser(1);
        }

        if(timeFlag10ms)
        {
            timeFlag10ms = 0; 
        }

        if(timeFlag102_4ms)
        {
            timeFlag102_4ms = 0;
            //scaling = 10? what is that used for
            fft(analogFFT, analogBuffer);
            analogBufferIndex = 0;
        }

        if(timeFlag200ms)
        {
            timeFlag200ms = 0;
            
        }

        if(timeFlag1s)
        {
            timeFlag1s = 0;
            fftSum = 0;
            for(i = 45; i < 55; i++)
                fftSum += analogFFT[i];
            LCDClear(0);
            sprintf(LCDBuffer, "%.0f", fftSum);
            LCDWriteString(LCDBuffer, 1, 1);
        }

        if(timeFlag5s)
        {
            timeFlag5s = 0;
            for(i = 0; i < 512; i++) {
                sprintf(LCDBuffer, "%.0f;", analogFFT[i]);
                SendString(1, LCDBuffer);
            }
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