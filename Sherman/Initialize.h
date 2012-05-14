#ifndef PICINIT_H
#define PICINIT_H

#include <plib.h>

#include "LCD.h"
#include "Components.h"
#include "Uart.h"
#include "AnalogIn.h"
#include "Laser.h"
#include "Pin.h"

#define SYS_FREQ 80000000

void initialize();
void initializeAnalogIn();
void initializePic();
void initializeLaser();
void initializePorts();
void initializePWM();
void initializeTimers();
void initializeUart();
void initializeLCD();
void initializeMotor();
void initializeSPI();
void initializeServo();

#endif