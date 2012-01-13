#ifndef PICINIT_H
#define PICINIT_H

#define SYS_FREQ 80000000

void initialize();
void initializePic();
void initializePorts();
void initializeTimers();
void initializeUart();

int setupTimer(int timer, int frequency, int priority);

#endif