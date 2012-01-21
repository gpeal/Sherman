#include "LCD.h"
#include <plib.h>

#define RS LATGbits.LATG12
#define RW LATGbits.LATG13
#define EN LATGbits.LATG15

#define DB0 LATEbits.LATE0
#define DB1 LATEbits.LATE1
#define DB2 LATEbits.LATE2
#define DB3 LATEbits.LATE3
#define DB4 LATEbits.LATE4
#define DB5 LATEbits.LATE5
#define DB6 LATEbits.LATE6
#define DB7 LATEbits.LATE7

void setupLCD()
{
    
    //Register pins
    TRISGbits.TRISG12 = 0;
    TRISGbits.TRISG13 = 0;
    TRISGbits.TRISG15 = 0;

    //Data pins
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    //WAIT 20ns
    wait();

    EN = 0;

    LCDcommand(0, 0,0,1,1,0,0,0,0);
    LCDcommand(0, 0,0,1,1,0,0,0,0);
    LCDcommand(0, 0,0,1,1,0,0,0,0);
    LCDcommand(0, 0,0,1,1,1,0,0,0);
    LCDcommand(0, 0,0,0,0,1,0,0,0);
    LCDcommand(0, 0,0,0,0,0,0,0,1);
    LCDcommand(0, 0,0,0,0,0,1,1,0);
    LCDcommand(0, 0,0,0,0,1,1,0,0);
    
    LCDcommand(1, 0,1,0,0,0,0,0,1);
    LCDcommand(1, 0,1,0,0,0,0,1,0);
}

void LCDcommand(int command, int d7, int d6, int d5, int d4, int d3, int d2, int d1, int d0)
{
    //0 for command, 1 for data.
    RS = command;
    
    DB0 = d0;
    DB1 = d1;
    DB2 = d2;
    DB3 = d3;
    DB4 = d4;
    DB5 = d5;
    DB6 = d6;
    DB7 = d7;

    EN = 1;

    wait();

    EN = 0;

    wait();

}

void wait()
{
    int i = 0;
    for (i; i<10000; i++)
    {
    }
}