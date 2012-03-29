#include "PWM.h"
#include "LCD.h"
#include <plib.h>

int timerTicks = 0;
int servoPulseWidth[] = {150, 150, 150, 150, 150, 150};

void setupServo()
{
    //priority 3 for no particular reason (purposefully not 1)
    setupTimer(2, 10000, 3);
}

void setServoPosition(int servo, int angle)
{
    //the /9.7 is a scalar that made the servo respond to ~0-180 degrees
    int pulseWidth = (double)angle/180*2*100/9.7+5;
    servoPulseWidth[servo] = pulseWidth;
}


void __ISR(_TIMER_2_VECTOR, ipl3) Timer2Isr(void)
{
    timerTicks++;

    //reset timerTicks every 20ms
    if (timerTicks > 200)
        timerTicks = 0;

    //uncomment this and set the port to any digital out porto enable the servo
    
    if (timerTicks < servoPulseWidth[1])
        PORTDbits.RD0 = 1;
    else
        PORTDbits.RD0 = 0;
    
    mT2ClearIntFlag();
}