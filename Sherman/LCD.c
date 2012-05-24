#include "LCD.h"
#include <plib.h>

#define RS G12
#define RW G13
#define EN G15

#define DB0 E0
#define DB1 E1
#define DB2 E2
#define DB3 E3
#define DB4 E4
#define DB5 E5
#define DB6 E6
#define DB7 E7

void setupLCD()
{

    //Register pins
    pinMode(RS, OUTPUT);
    pinMode(RW, OUTPUT);
    pinMode(EN, OUTPUT);

    //Data pins
    pinMode(DB0, OUTPUT);
    pinMode(DB1, OUTPUT);
    pinMode(DB2, OUTPUT);
    pinMode(DB3, OUTPUT);
    pinMode(DB4, OUTPUT);
    pinMode(DB5, OUTPUT);
    pinMode(DB6, OUTPUT);
    pinMode(DB7, OUTPUT);

    //WAIT 20ns
    wait();

    digitalWrite(EN, 0);

    LCDcommand(0, 0,0,1,1,1,0,0,0); //initialize 2 lines

    //LCDcommand(0, 0,0,1,1,0,0,0,0);
    //LCDcommand(0, 0,0,1,1,0,0,0,0);
    //LCDcommand(0, 0,0,1,1,1,0,0,0);
    //LCDcommand(0, 0,0,0,0,1,0,0,0); //blank screen without clearing

    LCDcommand(0, 0,0,0,0,0,0,0,1); //clear screen
    LCDcommand(0, 0,0,0,0,0,1,1,0); //cursor moves right

    LCDcommand(0, 0,0,0,0,1,1,0,0); //restore screen
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
    digitalWrite(RS, command);

    digitalWrite(DB0, d0);
    digitalWrite(DB1, d1);
    digitalWrite(DB2, d2);
    digitalWrite(DB3, d3);
    digitalWrite(DB4, d4);
    digitalWrite(DB5, d5);
    digitalWrite(DB6, d6);
    digitalWrite(DB7, d7);

    digitalWrite(EN, 1);


    wait();

    digitalWrite(EN, 0);

    wait();

}

void wait()
{
    int i = 0;
    for (i; i<10000; i++)
    {
    }
}