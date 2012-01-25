#include "Initialize.h"
#include <plib.h>

int direction = 0;

int main(void)
{
    initialize();
    while(1)
    {
    }
}

//ISRs
void __ISR(_UART_1_VECTOR, ipl2) IntUart1Handler(void)
{
    if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))
    {
        direction = !direction;
        UARTGetDataByte(UART1);
        INTClearFlag(INT_SOURCE_UART_RX(UART1));
    }

    if(INTGetFlag(INT_SOURCE_UART_TX(UART1)))
    {
        INTClearFlag(INT_SOURCE_UART_TX(UART1));
    }
}