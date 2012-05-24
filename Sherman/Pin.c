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
        case A14:TRISAbits.TRISA14 = mode;break;
        case A15:TRISAbits.TRISA15 = mode;break;

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
        case B12:TRISBbits.TRISB12 = mode;break;
        case B13:TRISBbits.TRISB13 = mode;break;
        case B14:TRISBbits.TRISB14 = mode;break;
        case B15:TRISBbits.TRISB15 = mode;break;

        case C1:TRISCbits.TRISC1   = mode;break;
        case C2:TRISCbits.TRISC2   = mode;break;
        case C3:TRISCbits.TRISC3   = mode;break;
        case C4:TRISCbits.TRISC4   = mode;break;
        case C12:TRISCbits.TRISC12 = mode;break;
        case C13:TRISCbits.TRISC13 = mode;break;
        case C14:TRISCbits.TRISC14 = mode;break;
        case C15:TRISCbits.TRISC15 = mode;break;

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
        case D12:TRISDbits.TRISD12 = mode;break;
        case D13:TRISDbits.TRISD13 = mode;break;
        case D14:TRISDbits.TRISD14 = mode;break;
        case D15:TRISDbits.TRISD15 = mode;break;

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
        case F12:TRISFbits.TRISF12 = mode;break;
        case F13:TRISFbits.TRISF13 = mode;break;

        case G0:TRISGbits.TRISG0   = mode;break;
        case G1:TRISGbits.TRISG1   = mode;break;
        case G2:TRISGbits.TRISG2   = mode;break;
        case G3:TRISGbits.TRISG3   = mode;break;
        case G6:TRISGbits.TRISG6   = mode;break;
        case G7:TRISGbits.TRISG7   = mode;break;
        case G8:TRISGbits.TRISG8   = mode;break;
        case G9:TRISGbits.TRISG9   = mode;break;
        case G12:TRISGbits.TRISG12 = mode;break;
        case G13:TRISGbits.TRISG13 = mode;break;
        case G14:TRISGbits.TRISG14 = mode;break;
        case G15:TRISGbits.TRISG15 = mode;break;

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
        case A14:LATAbits.LATA14 = value;break;
        case A15:LATAbits.LATA15 = value;break;

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
        case B12:LATBbits.LATB12 = value;break;
        case B13:LATBbits.LATB13 = value;break;
        case B14:LATBbits.LATB14 = value;break;
        case B15:LATBbits.LATB15 = value;break;

        case C1:LATCbits.LATC1   = value;break;
        case C2:LATCbits.LATC2   = value;break;
        case C3:LATCbits.LATC3   = value;break;
        case C4:LATCbits.LATC4   = value;break;
        case C12:LATCbits.LATC12 = value;break;
        case C13:LATCbits.LATC13 = value;break;
        case C14:LATCbits.LATC14 = value;break;
        case C15:LATCbits.LATC15 = value;break;

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
        case D12:LATDbits.LATD12 = value;break;
        case D13:LATDbits.LATD13 = value;break;
        case D14:LATDbits.LATD14 = value;break;
        case D15:LATDbits.LATD15 = value;break;

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
        case F12:LATFbits.LATF12 = value;break;
        case F13:LATFbits.LATF13 = value;break;

        case G0:LATGbits.LATG0   = value;break;
        case G1:LATGbits.LATG1   = value;break;
        case G2:LATGbits.LATG2   = value;break;
        case G3:LATGbits.LATG3   = value;break;
        case G6:LATGbits.LATG6   = value;break;
        case G7:LATGbits.LATG7   = value;break;
        case G8:LATGbits.LATG8   = value;break;
        case G9:LATGbits.LATG9   = value;break;
        case G12:LATGbits.LATG12 = value;break;
        case G13:LATGbits.LATG13 = value;break;
        case G14:LATGbits.LATG14 = value;break;
        case G15:LATGbits.LATG15 = value;break;
    }
}

void togglePin(int pin)
{
    digitalWrite(pin, !digitalRead(pin));
}

int digitalRead(int pin)
{
    int value;
    switch(pin)
    {
        case A0:value = PORTAbits.RA0;break;
        case A1:value = PORTAbits.RA1;break;
        case A2:value = PORTAbits.RA2;break;
        case A3:value = PORTAbits.RA3;break;
        case A4:value = PORTAbits.RA4;break;
        case A5:value = PORTAbits.RA5;break;
        case A6:value = PORTAbits.RA6;break;
        case A7:value = PORTAbits.RA7;break;
        case A9:value = PORTAbits.RA9;break;
        case A10:value = PORTAbits.RA10;break;
        case A14:value = PORTAbits.RA14;break;
        case A15:value = PORTAbits.RA15;break;

        case B0:value = PORTBbits.RB0;break;
        case B1:value = PORTBbits.RB1;break;
        case B2:value = PORTBbits.RB2;break;
        case B3:value = PORTBbits.RB3;break;
        case B4:value = PORTBbits.RB4;break;
        case B5:value = PORTBbits.RB5;break;
        case B6:value = PORTBbits.RB6;break;
        case B7:value = PORTBbits.RB7;break;
        case B8:value = PORTBbits.RB8;break;
        case B9:value = PORTBbits.RB9;break;
        case B10:value = PORTBbits.RB10;break;
        case B11:value = PORTBbits.RB11;break;
        case B12:value = PORTBbits.RB12;break;
        case B13:value = PORTBbits.RB13;break;
        case B14:value = PORTBbits.RB14;break;
        case B15:value = PORTBbits.RB15;break;

        case C1:value = PORTCbits.RC1;break;
        case C2:value = PORTCbits.RC2;break;
        case C3:value = PORTCbits.RC3;break;
        case C4:value = PORTCbits.RC4;break;
        case C12:value = PORTCbits.RC12;break;
        case C13:value = PORTCbits.RC13;break;
        case C14:value = PORTCbits.RC14;break;
        case C15:value = PORTCbits.RC15;break;

        case D0:value = PORTDbits.RD0;break;
        case D1:value = PORTDbits.RD1;break;
        case D2:value = PORTDbits.RD2;break;
        case D3:value = PORTDbits.RD3;break;
        case D4:value = PORTDbits.RD4;break;
        case D5:value = PORTDbits.RD5;break;
        case D6:value = PORTDbits.RD6;break;
        case D7:value = PORTDbits.RD7;break;
        case D8:value = PORTDbits.RD8;break;
        case D9:value = PORTDbits.RD9;break;
        case D10:value = PORTDbits.RD10;break;
        case D11:value = PORTDbits.RD11;break;
        case D12:value = PORTDbits.RD12;break;
        case D13:value = PORTDbits.RD13;break;
        case D14:value = PORTDbits.RD14;break;
        case D15:value = PORTDbits.RD15;break;

        case E0:value = PORTEbits.RE0;break;
        case E1:value = PORTEbits.RE1;break;
        case E2:value = PORTEbits.RE2;break;
        case E3:value = PORTEbits.RE3;break;
        case E4:value = PORTEbits.RE4;break;
        case E5:value = PORTEbits.RE5;break;
        case E6:value = PORTEbits.RE6;break;
        case E7:value = PORTEbits.RE7;break;
        case E8:value = PORTEbits.RE8;break;
        case E9:value = PORTEbits.RE9;break;

        case F0:value = PORTFbits.RF0;break;
        case F1:value = PORTFbits.RF1;break;
        case F2:value = PORTFbits.RF2;break;
        case F3:value = PORTFbits.RF3;break;
        case F4:value = PORTFbits.RF4;break;
        case F5:value = PORTFbits.RF5;break;
        case F8:value = PORTFbits.RF8;break;
        case F12:value = PORTFbits.RF12;break;
        case F13:value = PORTFbits.RF13;break;

        case G0:value = PORTGbits.RG0;break;
        case G1:value = PORTGbits.RG1;break;
        case G2:value = PORTGbits.RG2;break;
        case G3:value = PORTGbits.RG3;break;
        case G6:value = PORTGbits.RG6;break;
        case G7:value = PORTGbits.RG7;break;
        case G8:value = PORTGbits.RG8;break;
        case G9:value = PORTGbits.RG9;break;
        case G12:value = PORTGbits.RG12;break;
        case G13:value = PORTGbits.RG13;break;
        case G14:value = PORTGbits.RG14;break;
        case G15:value = PORTGbits.RG15;break;
    }
    return value;
}

void analogWrite(int pin, int value)
{

}

int analogRead(int pin)
{

}