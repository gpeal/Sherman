#include "Initialize.h"


#define DESIRED_BAUDRATE_NU32 115200 // Baudrate

void initialize()
{
    initializePic();
    initializePorts();
    initializeAnalogIn();
    initializeLaser();
    initializeMotor();
    initializePWM();
    initializeServo();
    initializeSPI();
    initializeTimers();
    initializeUart();
    initializeI2C();
    //must be after initializeI2C
    initializeCompass();
    initializeLCD();
}

void initializePic()
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableSystemMultiVectoredInt();
}

void initializePorts()
{
    TRISBbits.TRISB4 = 0;
}

void initializeAnalogIn()
{
    //setupAnalogIn(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
}

void initializeLaser()
{

}

void initializeMotor()
{
    //setupMotor(MOTOR_WHEEL_LEFT);
}

void initializePWM()
{
    //setupPWMTimer();
}

void initializeServo()
{
    //setupServo(2, 1);
}

void initializeSPI()
{
    //setupSPIMaster();
}

void initializeTimers()
{
    //used for time
    setupTimer(1, 10000, 1);
}

void initializeUart()
{
    //used for debug
    setupUart(1, 2);
}

void initializeI2C()
{
    setupI2C();
}

void initializeCompass()
{
    setupCompass();
}

void initializeLCD()
{
    setupLCD();
}
