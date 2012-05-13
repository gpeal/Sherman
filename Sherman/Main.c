#include "Initialize.h"
#include <plib.h>
#include <limits.h>
#include "fft.h"
#include "Uart.h"

// Configuring the Device Configuration Registers
// 80Mhz Core/Periph, Pri Osc w/PLL, Write protect Boot Flash
#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#pragma config ICESEL = ICS_PGx2, BWP = OFF
#pragma config FSOSCEN = OFF // to make C13 an IO pin, for the USER switch

#define DEBUG


//Global Variables
unsigned int time = 0;
char movementDirection = 0;
int movementSpeed = 500;
char timeFlag_1ms = 0, timeFlag1ms = 0, timeFlag2ms = 0, timeFlag10ms = 0, timeFlag100ms = 0, timeFlag200ms = 0, timeFlag102_4ms = 0, timeFlag0_5s = 0, timeFlag1s = 0, timeFlag5s = 0;

int main(void)
{
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

        if(timeFlag100ms)
        {
            timeFlag100ms = 0;
        }

        if(timeFlag200ms)
        {
            timeFlag200ms = 0;
        }

        if(timeFlag0_5s)
        {
            timeFlag0_5s = 0;
#ifdef DEBUG
            //optional send motor over uart
            switch(movementDirection)
            {
                case 0:
                    sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 2, movementSpeed, 2, movementSpeed);
                    SendString(1, UARTBuffer);
                case 1:
                    sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 2, movementSpeed, 1, movementSpeed);
                    SendString(1, UARTBuffer);
                case 2:
                    sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 1, movementSpeed, 2, movementSpeed);
                    SendString(1, UARTBuffer);
                case 3:
                    sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 1, movementSpeed, 1, movementSpeed);
                    SendString(1, UARTBuffer);
                case 4:
                    sprintf(UARTBuffer,"%1i%04i%1i%04i\n", 0, movementSpeed, 0, movementSpeed);
                    SendString(1, UARTBuffer);
            }
#endif
        }
        if(timeFlag1s)
        {
            timeFlag1s = 0;
            switch(movementDirection)
            {
                case 0:
                    movementForward(movementSpeed);
                    break;
                case 1:
                    movementLeft(movementSpeed);
                    break;
                case 2:
                    movementRight(movementSpeed);
                    break;
                case 3:
                    movementBackward(movementSpeed);
                    break;
                case 4:
                    movementBrake();
                    break;
            }
            movementDirection = (movementDirection+1)%5;
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
    if(time%1000 < 1)
        timeFlag100ms = 1;
    if(time%2000 < 1)
        timeFlag200ms = 1;
    if(time%5000 < 1)
    {
        LATAbits.LATA4 = !LATAbits.LATA4;
        timeFlag0_5s = 1;
    }
    if(time%10000 < 1)
        timeFlag1s = 1;
    if(time%50000 < 1)
        timeFlag5s = 1;
    mT1ClearIntFlag();
}