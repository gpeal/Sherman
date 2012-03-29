#include "laser.h"
#include <plib.h>

void laserOn(int laser)
{
	if(laser == 1)
            PORTDbits.RD0 = 1;
}

void laserOff(int laser)
{
	PORTDbits.RD0 = 0;
}

void toggleLaser(int laser)
{
    PORTDbits.RD0 = !PORTDbits.RD0;
}