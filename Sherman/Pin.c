#include "Pin.h"
#include <plib.h>

void pinMode(int pin, char mode)
{
    switch(pin)
    {
        case A0:TRISAbits.TRISA0   = mode;break;
        case A1:TRISAbits.TRISA1   = mode;break;
        case A2:TRISAbits.TRISA2   = mode;break;
        case A3:TRISAbits.TRISA3   = mode;break;
        case A4:TRISAbits.TRISA4   = mode;break;
        case A5:TRISAbits.TRISA5   = mode;break;
        case A6:TRISAbits.TRISA6   = mode;break;
        case A7:TRISAbits.TRISA7   = mode;break;
        case A9:TRISAbits.TRISA9   = mode;break;
        case A10:TRISAbits.TRISA10 = mode;break;
        case A15:TRISAbits.TRISA14 = mode;break;

        case B0:TRISBbits.TRISB0   = mode;break;
        case B1:TRISBbits.TRISB1   = mode;break;
        case B2:TRISBbits.TRISB2   = mode;break;
        case B3:TRISBbits.TRISB3   = mode;break;
        case B4:TRISBbits.TRISB4   = mode;break;
        case B5:TRISBbits.TRISB5   = mode;break;
        case B6:TRISBbits.TRISB6   = mode;break;
        case B7:TRISBbits.TRISB7   = mode;break;
        case B8:TRISBbits.TRISB8   = mode;break;
        case B9:TRISBbits.TRISB9   = mode;break;
        case B10:TRISBbits.TRISB10 = mode;break;
        case B11:TRISBbits.TRISB11 = mode;break;
        case B12:TRISBbits.TRISB11 = mode;break;
        case B13:TRISBbits.TRISB12 = mode;break;
        case B14:TRISBbits.TRISB13 = mode;break;
        case B15:TRISBbits.TRISB14 = mode;break;

        case C1:TRISCbits.TRISC1   = mode;break;
        case C2:TRISCbits.TRISC2   = mode;break;
        case C3:TRISCbits.TRISC3   = mode;break;
        case C4:TRISCbits.TRISC4   = mode;break;
        case C13:TRISCbits.TRISC12 = mode;break;
        case C14:TRISCbits.TRISC13 = mode;break;
        case C15:TRISCbits.TRISC14 = mode;break;

        case D0:TRISDbits.TRISD0   = mode;break;
        case D1:TRISDbits.TRISD1   = mode;break;
        case D2:TRISDbits.TRISD2   = mode;break;
        case D3:TRISDbits.TRISD3   = mode;break;
        case D4:TRISDbits.TRISD4   = mode;break;
        case D5:TRISDbits.TRISD5   = mode;break;
        case D6:TRISDbits.TRISD6   = mode;break;
        case D7:TRISDbits.TRISD7   = mode;break;
        case D8:TRISDbits.TRISD8   = mode;break;
        case D9:TRISDbits.TRISD9   = mode;break;
        case D10:TRISDbits.TRISD10 = mode;break;
        case D11:TRISDbits.TRISD11 = mode;break;
        case D12:TRISDbits.TRISD11 = mode;break;
        case D13:TRISDbits.TRISD12 = mode;break;
        case D14:TRISDbits.TRISD13 = mode;break;
        case D15:TRISDbits.TRISD14 = mode;break;

        case E0:TRISEbits.TRISE0   = mode;break;
        case E1:TRISEbits.TRISE1   = mode;break;
        case E2:TRISEbits.TRISE2   = mode;break;
        case E3:TRISEbits.TRISE3   = mode;break;
        case E4:TRISEbits.TRISE4   = mode;break;
        case E5:TRISEbits.TRISE5   = mode;break;
        case E6:TRISEbits.TRISE6   = mode;break;
        case E7:TRISEbits.TRISE7   = mode;break;
        case E8:TRISEbits.TRISE8   = mode;break;
        case E9:TRISEbits.TRISE9   = mode;break;

        case F0:TRISFbits.TRISF0   = mode;break;
        case F1:TRISFbits.TRISF1   = mode;break;
        case F2:TRISFbits.TRISF2   = mode;break;
        case F3:TRISFbits.TRISF3   = mode;break;
        case F4:TRISFbits.TRISF4   = mode;break;
        case F5:TRISFbits.TRISF5   = mode;break;
        case F8:TRISFbits.TRISF8   = mode;break;
        case F13:TRISFbits.TRISF12 = mode;break;
        case F14:TRISFbits.TRISF13 = mode;break;
    }
}

void digitalWrite(int pin, char value)
{
    switch(pin)
    {
        case A0:LATAbits.LATA0   = value;break;
        case A1:LATAbits.LATA1   = value;break;
        case A2:LATAbits.LATA2   = value;break;
        case A3:LATAbits.LATA3   = value;break;
        case A4:LATAbits.LATA4   = value;break;
        case A5:LATAbits.LATA5   = value;break;
        case A6:LATAbits.LATA6   = value;break;
        case A7:LATAbits.LATA7   = value;break;
        case A9:LATAbits.LATA9   = value;break;
        case A10:LATAbits.LATA10 = value;break;
        case A15:LATAbits.LATA14 = value;break;

        case B0:LATBbits.LATB0   = value;break;
        case B1:LATBbits.LATB1   = value;break;
        case B2:LATBbits.LATB2   = value;break;
        case B3:LATBbits.LATB3   = value;break;
        case B4:LATBbits.LATB4   = value;break;
        case B5:LATBbits.LATB5   = value;break;
        case B6:LATBbits.LATB6   = value;break;
        case B7:LATBbits.LATB7   = value;break;
        case B8:LATBbits.LATB8   = value;break;
        case B9:LATBbits.LATB9   = value;break;
        case B10:LATBbits.LATB10 = value;break;
        case B11:LATBbits.LATB11 = value;break;
        case B12:LATBbits.LATB11 = value;break;
        case B13:LATBbits.LATB12 = value;break;
        case B14:LATBbits.LATB13 = value;break;
        case B15:LATBbits.LATB14 = value;break;

        case C1:LATCbits.LATC1   = value;break;
        case C2:LATCbits.LATC2   = value;break;
        case C3:LATCbits.LATC3   = value;break;
        case C4:LATCbits.LATC4   = value;break;
        case C13:LATCbits.LATC12 = value;break;
        case C14:LATCbits.LATC13 = value;break;
        case C15:LATCbits.LATC14 = value;break;

        case D0:LATDbits.LATD0   = value;break;
        case D1:LATDbits.LATD1   = value;break;
        case D2:LATDbits.LATD2   = value;break;
        case D3:LATDbits.LATD3   = value;break;
        case D4:LATDbits.LATD4   = value;break;
        case D5:LATDbits.LATD5   = value;break;
        case D6:LATDbits.LATD6   = value;break;
        case D7:LATDbits.LATD7   = value;break;
        case D8:LATDbits.LATD8   = value;break;
        case D9:LATDbits.LATD9   = value;break;
        case D10:LATDbits.LATD10 = value;break;
        case D11:LATDbits.LATD11 = value;break;
        case D12:LATDbits.LATD11 = value;break;
        case D13:LATDbits.LATD12 = value;break;
        case D14:LATDbits.LATD13 = value;break;
        case D15:LATDbits.LATD14 = value;break;

        case E0:LATEbits.LATE0   = value;break;
        case E1:LATEbits.LATE1   = value;break;
        case E2:LATEbits.LATE2   = value;break;
        case E3:LATEbits.LATE3   = value;break;
        case E4:LATEbits.LATE4   = value;break;
        case E5:LATEbits.LATE5   = value;break;
        case E6:LATEbits.LATE6   = value;break;
        case E7:LATEbits.LATE7   = value;break;
        case E8:LATEbits.LATE8   = value;break;
        case E9:LATEbits.LATE9   = value;break;

        case F0:LATFbits.LATF0   = value;break;
        case F1:LATFbits.LATF1   = value;break;
        case F2:LATFbits.LATF2   = value;break;
        case F3:LATFbits.LATF3   = value;break;
        case F4:LATFbits.LATF4   = value;break;
        case F5:LATFbits.LATF5   = value;break;
        case F8:LATFbits.LATF8   = value;break;
        case F13:LATFbits.LATF12 = value;break;
        case F14:LATFbits.LATF13 = value;break;
    }
}

int digitalRead(int pin)
{
    switch(pin)
    {
        case A0:return PORTAbits.RA0;
        case A1:return PORTAbits.RA1;
        case A2:return PORTAbits.RA2;
        case A3:return PORTAbits.RA3;
        case A4:return PORTAbits.RA4;
        case A5:return PORTAbits.RA5;
        case A6:return PORTAbits.RA6;
        case A7:return PORTAbits.RA7;
        case A9:return PORTAbits.RA9;
        case A10:return PORTAbits.RA10;
        case A15:return PORTAbits.RA14;

        case B0:return PORTBbits.RB0;
        case B1:return PORTBbits.RB1;
        case B2:return PORTBbits.RB2;
        case B3:return PORTBbits.RB3;
        case B4:return PORTBbits.RB4;
        case B5:return PORTBbits.RB5;
        case B6:return PORTBbits.RB6;
        case B7:return PORTBbits.RB7;
        case B8:return PORTBbits.RB8;
        case B9:return PORTBbits.RB9;
        case B10:return PORTBbits.RB10;
        case B11:return PORTBbits.RB11;
        case B12:return PORTBbits.RB11;
        case B13:return PORTBbits.RB12;
        case B14:return PORTBbits.RB13;
        case B15:return PORTBbits.RB14;

        case C1:return PORTCbits.RC1;
        case C2:return PORTCbits.RC2;
        case C3:return PORTCbits.RC3;
        case C4:return PORTCbits.RC4;
        case C13:return PORTCbits.RC12;
        case C14:return PORTCbits.RC13;
        case C15:return PORTCbits.RC14;

        case D0:return PORTDbits.RD0;
        case D1:return PORTDbits.RD1;
        case D2:return PORTDbits.RD2;
        case D3:return PORTDbits.RD3;
        case D4:return PORTDbits.RD4;
        case D5:return PORTDbits.RD5;
        case D6:return PORTDbits.RD6;
        case D7:return PORTDbits.RD7;
        case D8:return PORTDbits.RD8;
        case D9:return PORTDbits.RD9;
        case D10:return PORTDbits.RD10;
        case D11:return PORTDbits.RD11;
        case D12:return PORTDbits.RD11;
        case D13:return PORTDbits.RD12;
        case D14:return PORTDbits.RD13;
        case D15:return PORTDbits.RD14;

        case E0:return PORTEbits.RE0;
        case E1:return PORTEbits.RE1;
        case E2:return PORTEbits.RE2;
        case E3:return PORTEbits.RE3;
        case E4:return PORTEbits.RE4;
        case E5:return PORTEbits.RE5;
        case E6:return PORTEbits.RE6;
        case E7:return PORTEbits.RE7;
        case E8:return PORTEbits.RE8;
        case E9:return PORTEbits.RE9;

        case F0:return PORTFbits.RF0;
        case F1:return PORTFbits.RF1;
        case F2:return PORTFbits.RF2;
        case F3:return PORTFbits.RF3;
        case F4:return PORTFbits.RF4;
        case F5:return PORTFbits.RF5;
        case F8:return PORTFbits.RF8;
        case F13:return PORTFbits.RF12;
        case F14:return PORTFbits.RF13;
    }
}

void analogWrite(int pin, int value)
{

}

int analogRead(int pin)
{

}