#ifndef ANALOG_IN_H
#define ANALOG_IN_H

void setupAnalogIn(int port0, int port1, int port2, int port3, int port4, int port5, int port6, int port7, int port8, int port9, int port10, int port11, int port12, int port13, int port14, int port15);

//This reads the input by port Index, not port ID; if you open 1 port, no matter what ID it has, you must read it using index 0.
int readAnalogIn(int portIndex);

#endif