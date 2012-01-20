#ifndef UART_H
#define UART_H
#include <plib.h>
#include "Initialize.h"

#define DESIRED_BAUDRATE_NU32 115200 // Baudrate

// Opens the uart module assigned to uartX (1 will open uart1)
// Assigns that uart module an interrupt with priority int_priotity_level_X
int setupUart(int uartX, int int_priotity_level_X);

// Sends a single character over the desired uart module (1 will send over uart1)
int SendCharacter(int id, const char character);

// Sends a string over the desired uart module (1 will send over uart1)
int SendString(int id, const char *string);

//ISR TEMPLATE
/*
void __ISR(_UART_1_VECTOR, ipl2) IntUart1Handler(void)
{
    if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))
    {
        INTClearFlag(INT_SOURCE_UART_RX(UART1));
    }

    if(INTGetFlag(INT_SOURCE_UART_TX(UART1)))
    {
        INTClearFlag(INT_SOURCE_UART_TX(UART1));
    }
}
*/
#endif