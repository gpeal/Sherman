#include "Initialize.h"
#include <plib.h>

//Global Variables
int main(void)
{
    char spiChar = 'a';
    double i;
    initialize();
    LCDClear(0);
    LCDWriteString("Initializing", 1, 1);
    while(1)
    {
        LATAbits.LATA4 = !LATAbits.LATA4;
        spiChar = readSPI();
        if(spiChar != -1)
        {
            sprintf(LCDBuffer, "Reading: %c", spiChar);
            LCDClear(0);
            LCDWriteString(LCDBuffer, 1, 1);
            LATAbits.LATA5 = !LATAbits.LATA5;
        }
        for(i = 0; i < 1000000; i++) {}
    }
}