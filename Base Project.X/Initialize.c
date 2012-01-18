#include "AnalogIn.h"
#include "Initialize.h"
#include "Uart.h"
#include <plib.h>

#define DESIRED_BAUDRATE_NU32 115200 // Baudrate

void initialize()
{
    initializePic();
    initializePorts();
    initializePWM();
    initializeTimers();
    initializeUart();
    initializeAnalogIn();
}

//currently set to RB2
void initializeAnalogIn()
{
    setupAnalogIn(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void initializePic()
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableSystemMultiVectoredInt();
}

void initializePorts()
{
    //AD1PCFG = 0xFFFD;
    TRISDbits.TRISD1 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISCbits.TRISC13 = 1;
}

void initializePWM()
{
    setupPWM();
}

void initializeTimers()
{
}

void initializeUart()
{
    setupUart(1,2);
}