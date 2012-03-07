#include "Uart.h"

int setupUart(int id, int int_priotity_level_X) {
  unsigned int pbClk;
  INT_SOURCE int_uXrx;
  INT_VECTOR int_uart_X_vector;
  UART_MODULE uartX;
  // Configure the system performance
  pbClk = SYSTEMConfigPerformance(SYS_FREQ);

  switch(id)
  {
      case 1:
          uartX = UART1;
          int_uXrx = INT_U1RX;
          int_uart_X_vector = INT_UART_1_VECTOR;
          break;
      case 2:
          uartX = UART2;
          int_uXrx = INT_U2RX;
          int_uart_X_vector = INT_UART_2_VECTOR;
          break;
      case 3:
          uartX = UART3;
          int_uXrx = INT_U3RX;
          int_uart_X_vector = INT_UART_3_VECTOR;
          break;
      case 4:
          uartX = UART4;
          int_uXrx = INT_U4RX;
          int_uart_X_vector = INT_UART_4_VECTOR;
          break;
      default:
          uartX = UART1;
          int_uXrx = INT_U1RX;
          int_uart_X_vector = INT_UART_1_VECTOR;
          break;
  }

  switch(int_priotity_level_X)
  {
      case 0:
        int_priotity_level_X = INT_PRIORITY_DISABLED;
        break;
      case 1:
          int_priotity_level_X = INT_PRIORITY_LEVEL_1;
          break;
      case 2:
          int_priotity_level_X = INT_PRIORITY_LEVEL_2;
          break;
      case 3:
          int_priotity_level_X = INT_PRIORITY_LEVEL_3;
          break;
      case 4:
          int_priotity_level_X = INT_PRIORITY_LEVEL_4;
          break;
      case 5:
          int_priotity_level_X = INT_PRIORITY_LEVEL_5;
          break;
      case 6:
          int_priotity_level_X = INT_PRIORITY_LEVEL_6;
          break;
      case 7:
          int_priotity_level_X = INT_PRIORITY_LEVEL_6;
          break;
      default:
          int_priotity_level_X = INT_PRIORITY_LEVEL_1;
          break;
  }

  UARTConfigure(uartX, UART_ENABLE_PINS_TX_RX_ONLY);
  UARTSetFifoMode(uartX, UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY);
  UARTSetLineControl(uartX, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
  UARTSetDataRate(uartX, pbClk, DESIRED_BAUDRATE_NU32);
  UARTEnable(uartX, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

  // Configure UART1 RX Interrupt
  INTEnable(int_uXrx, INT_ENABLED);
  INTSetVectorPriority(int_uart_X_vector, int_priotity_level_X);
  INTSetVectorSubPriority(int_uart_X_vector, INT_SUB_PRIORITY_LEVEL_0);
  return 1;
}

// Put a character over the serial port, called by WriteString
int SendCharacter(int id, const char character)
{
    UART_MODULE uartX;
    switch(id)
    {
        case 1:
            uartX = UART1;
            break;
        case 2:
            uartX = UART2;
            break;
        case 3:
            uartX = UART3;
            break;
        case 4:
            uartX = UART4;
            break;
        default:
            uartX = UART1;
    }
    while(!UARTTransmitterIsReady(uartX));
    UARTSendDataByte(uartX, character);
    while(!UARTTransmissionHasCompleted(uartX));
}

// Write a string over the serial port
int SendString(int id, const char *string)
{
    UART_MODULE uartX;
    switch(id)
    {
        case 1:
            uartX = UART1;
            break;
        case 2:
            uartX = UART2;
            break;
        case 3:
            uartX = UART3;
            break;
        case 4:
            uartX = UART4;
            break;
        default:
            uartX = UART1;
    }
    while(*string != '\0')
    {
      while(!UARTTransmitterIsReady(uartX));
      UARTSendDataByte(uartX, (char) *string);
      string++;
      while(!UARTTransmissionHasCompleted(uartX));
    }
    return 1;
}














