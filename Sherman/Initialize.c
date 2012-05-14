#include "Initialize.h"

#define start_pause()  do{int _a; for(_a = 0; _a < 200; ++_a);}while(0)

#define DESIRED_BAUDRATE_NU32 115200 // Baudrate

void initialize()
{
    start_pause();
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
    // disable JTAG to get A4 and A5 back
    DDPCONbits.JTAGEN = 0;
}

void initializePorts()
{
    // set Debug LEDs to output.
    pinMode(A4, OUTPUT);
}

void initializeAnalogIn()
{
    setupAnalogIn(1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void initializeLaser()
{

}

void initializeMotor()
{
    //don't forget to start PWM timer
    setupMotor(MOTOR_WHEEL_RIGHT);
    setupMotor(MOTOR_WHEEL_LEFT);
}

void initializePWM()
{
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
    //setupTimer(3, 1000000, 1);
}

void initializeUart()
{
    setupUart(1, 2);
}

void initializeLCD()
{
    setupLCD();
}
