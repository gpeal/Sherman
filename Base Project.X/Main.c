#include "Initialize.h"
#include <plib.h>

int main(void)
{
    int j;
    initialize();
    while(1)
    {
        PORTAbits.RA4 = SendString(1, "Hi!");
        PORTAbits.RA5 = !PORTAbits.RA5;
        for(j = 0; j < 5000000; j++) {}
    }
}

//ISRs
void __ISR(_UART_1_VECTOR, ipl2) IntUart1Handler(void)
{
    // Is this an RX interrupt?
    if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))
    {
        // Clear the RX interrupt Flag
        INTClearFlag(INT_SOURCE_UART_RX(UART1));
    }

    // We don't care about TX interrupt
    if(INTGetFlag(INT_SOURCE_UART_TX(UART1)))
    {
        INTClearFlag(INT_SOURCE_UART_TX(UART1));
    }
}