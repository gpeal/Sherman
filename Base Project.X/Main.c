#include "Initialize.h"
#include <plib.h>

//Global Variables
char uartInput = 0;

int main(void)
{
    int motorDutyCycle = 500;
    int motorDirection = 1;
    char motorDutyCycleStr[16];
    initialize();
    setMotor(MOTOR_WHEEL_LEFT, motorDutyCycle, motorDirection);
    SendString(1, "Hello World!");
    while(1)
    {
        if(uartInput == 'd')
        {
            uartInput = 0;
            motorDirection = !motorDirection;
            setMotor(MOTOR_WHEEL_LEFT, motorDutyCycle, motorDirection);
        }
        else if(uartInput == 'f')
        {
            uartInput = 0;
            motorDutyCycle += 100;
            sprintf(motorDutyCycleStr, "%i\n", motorDutyCycle);
            SendString(1, motorDutyCycleStr);
            setMotor(MOTOR_WHEEL_LEFT, motorDutyCycle, motorDirection);
        }
        else if(uartInput == 's')
        {
            uartInput = 0;
            motorDutyCycle -= 100;
            sprintf(motorDutyCycleStr, "%i\n", motorDutyCycle);
            SendString(1,motorDutyCycleStr);
            setMotor(MOTOR_WHEEL_LEFT, motorDutyCycle, motorDirection);
        }
    }
}

//ISRs
void __ISR(_UART_1_VECTOR, ipl2) IntUart1Handler(void)
{
    if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))
    {
        uartInput = UARTGetDataByte(UART1);
        LATAbits.LATA5 = !LATAbits.LATA5;
        INTClearFlag(INT_SOURCE_UART_RX(UART1));
    }

    if(INTGetFlag(INT_SOURCE_UART_TX(UART1)))
    {
        INTClearFlag(INT_SOURCE_UART_TX(UART1));
    }
}