#include "Initialize.h"
#include <plib.h>

//Global Variables

int main(void)
{
    int voltage, i;
    initialize();
    laserOn(1);
    sprintf(LCDBuffer, "Initializing");
    LCDWriteString(LCDBuffer, 1, 1);
    while(1)
    {
        voltage = readAnalogIn(0);
        sprintf(LCDBuffer, "Voltage: %d", voltage);
        LCDClear(0);
        LCDWriteString(LCDBuffer, 1, 1);
        for(i = 0; i < 8000000; i++) {}
    }
}