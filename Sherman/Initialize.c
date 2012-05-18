#include "Initialize.h"

#define start_pause()  do{int _a; for(_a = 0; _a < 200; ++_a);}while(0)

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
    digitalWrite(A4, 1);
    pinMode(A5, OUTPUT);
    pinMode(F3, OUTPUT);
    digitalWrite(F3, 0);
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
    EnqueueMotorAction(MOTOR_ACTION_STOP);
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
    //uart
    pinMode(UART_ARDUINO_RX, INPUT);
    pinMode(UART_ARDUINO_TX, OUTPUT);
    setupUart(2, 1);

    //Xbee
    pinMode(UART_XBEE_RX, INPUT);
    pinMode(UART_XBEE_TX, OUTPUT);
    setupUart(3, 1);
}

void initializeLCD()
{
    setupLCD();
}