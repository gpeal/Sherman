#include "Initialize.h"
#include "Uart.h"
#include <plib.h>

#define DESIRED_BAUDRATE_NU32 115200 // Baudrate

void initialize()
{
    initializePic();
    initializePorts();
    initializeTimers();
    initializeUart();
}

void initializePic()
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableSystemMultiVectoredInt();
}

void initializePorts()
{
    TRISDbits.TRISD1 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISCbits.TRISC13 = 1;
    AD1PCFGbits.PCFG0 = 0;
}

void initializeTimers()
{
}

void initializeUart() {
    setupUart(1,2);
}