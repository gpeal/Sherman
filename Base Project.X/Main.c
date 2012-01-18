#include "Initialize.h"
#include <plib.h>

int main(void)
{
    int analogValue, j;
    char string[32];
    initialize();
    while(1)
    {
        analogValue = readAnalogIn(5);
        //analogValue *= 4;
        sprintf(string, "Analog: %i\n", analogValue);
        SendString(1, string);
        PORTAbits.RA5 = !PORTAbits.RA5;
        setDutyCycle(analogValue);
        for(j = 0; j < 5000000; j++)
            Nop();
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

void __ISR(_TIMER_1_VECTOR, ipl1) TimerIsr(void)
{
    mT1ClearIntFlag();
}