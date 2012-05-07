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
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
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
    setupMotor(MOTOR_WHEEL_LEFT);
    setupMotor(MOTOR_WHEEL_RIGHT);
}

void initializePWM()
{
    //needed for motor
    setupPWMTimer();
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
    setupTimer(1, 10000, 1);
}

void initializeUart()
{
    setupUart(1, 2);
}

void initializeLCD()
{
    //setupLCD();
}
