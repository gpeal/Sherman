#include "Initialize.h"


#define DESIRED_BAUDRATE_NU32 115200 // Baudrate

void initialize()
{
    initializePic();
    initializePorts();
    initializeTimers();
    //initializePWM();
    //initializeMotor();
    //initializeUart();
}

void initializeLCD()
{
}

//currently set to RB2
void initializeAnalogIn()
{
}

void initializePic()
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableSystemMultiVectoredInt();
}

void initializePorts()
{
    TRISAbits.TRISA0 = 0;
    PORTAbits.RA0 = 0;
    TRISAbits.TRISA1 = 0;
    PORTAbits.RA1 = 0;
    TRISAbits.TRISA2 = 0;
    PORTAbits.RA2 = 0;
}

void initializePWM()
{
}

void initializeTimers()
{
    setupTimer(1, 10000, 1);
}

void initializeUart()
{
}

void initializeMotor()
{
}