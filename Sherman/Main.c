#include "Initialize.h"
#include <plib.h>

//Global Variables
char uartInput = 0;

int main(void)
{
    int angle = 0;
    int direction = 1;
    double i;
    initialize();

    while(1)
    {
        LCDClear(0);
        angle += direction;
        if (angle >= 180)
            direction = -1; 
        else if (angle <= 0)
            direction = 1;

        setServoPosition(1, angle);
        sprintf(LCDBuffer, "Angle: %d", angle);
        LCDWriteString(LCDBuffer, 1, 1);
        PORTAbits.RA4 = !PORTAbits.RA4;
        for(i = 1; i < 8000; i++) {}
    }

}