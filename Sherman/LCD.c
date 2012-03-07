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

    LCDcommand(0, 0,0,1,1,1,0,0,0); //initialize 2 lines

    //LCDcommand(0, 0,0,1,1,0,0,0,0);
    //LCDcommand(0, 0,0,1,1,0,0,0,0);
    //LCDcommand(0, 0,0,1,1,1,0,0,0);
    //LCDcommand(0, 0,0,0,0,1,0,0,0); //blank screen without clearing

    LCDcommand(0, 0,0,0,0,0,0,0,1); //clear screen
    LCDcommand(0, 0,0,0,0,0,1,1,0); //cursor moves right

    LCDcommand(0, 0,0,0,0,1,1,0,0); //restore screen
    
    LCDWriteString("Hello", 1, 1);
    LCDWriteString("World!", 2, 1);
    LCDClear(0);
    LCDWriteString("a", 1, 1);
    LCDWriteString("b", 2, 1);
}

void LCDWriteChar(char c)
{
    LCDcommand(1, c>>7&1, c>>6&1, c>>5&1, c>>4&1, c>>3&1, c>>2&1, c>>1&1, c&1);
}

void LCDWriteString(char *str, int row, int col)
{
    row--;
    col--;
    LCDcommand(0,1,row,0,0,col>>3&1,col>>2&1,col>>1&1,col&1);

    while(*str)
    {
        LCDWriteChar(*str);
        str++;
    }
}

void LCDClear(int line)
{
    switch(line)
    {
        case 1:
            LCDWriteString("                ", 1, 1);
            break;
        case 2:
            LCDWriteString("                ", 2, 1);
            break;
        default:
            LCDcommand(0,0,0,0,0,0,0,0,1);
    }
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