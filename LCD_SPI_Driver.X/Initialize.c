#include "Initialize.h"


#define DESIRED_BAUDRATE_NU32 115200 // Baudrate

void initialize()
{
    initializePic();
    initializePorts();
    initializePWM();
    initializeMotor();
    initializeUart();
    initializeLCD();
    initializeSPI();
}

void initializeLCD()
{
    setupLCD();
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
    TRISAbits.TRISA4 = 0;
    PORTAbits.RA4 = 0;
    TRISAbits.TRISA5 = 0;
    PORTAbits.RA5 = 0;
}

void initializePWM()
{
    //setupPWMTimer();
}

void initializeTimers()
{
}

void initializeUart()
{
    //setupUart(1, 2);
}

void initializeMotor()
{
}

void initializeSPI()
{
    setupSPISlave();
}