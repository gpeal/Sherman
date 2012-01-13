#ifndef UART_H
#define UART_H
#include <plib.h>

#define DESIRED_BAUDRATE_NU32 115200 // Baudrate
#define SYS_FREQ 80000000

void setupUart(int uartX, int int_priotity_level_X);
void PutCharacter(UART_MODULE id, const char character);

#endif