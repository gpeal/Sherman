#include "AnalogIn.h"
#include <plib.h>

void setupAnalogIn()
{
    AD1PCFG = 0xFFFB;     // PORTB = Digital; RB2 = analog in
    AD1CON1 = 0x0000;     // SAMP bit = 0 ends sampling, starts converting
    AD1CHS  = 0x00020000; // Connect RB2/AN2 as CH0 input
    AD1CSSL = 0;          // no input scan, so don't specify inputs to scan
    AD1CON3 = 0x0002;     // ADC clk period = 6 PB clk periods = 75 ns
    AD1CON2 = 0;          // no input scan

    // don't turn on the ADC until all other configuration is finished!
    AD1CON1SET = 0x8000;  // turn ADC ON, defaults to software sampling
}

int readAnalogIn()
{
    int i;
    AD1CON1SET = 0x0002;  // set SAMP bit.  clears the DONE bit, starts sampling
    for (i=0; i<10; i++); // give it enough time to settle, 200 ns or more
    // a real timing operation would be better!  this would fail if optimized!
    AD1CON1CLR = 0x0002;  // conversion starts when SAMP bit is cleared
    while (!(AD1CON1 & 0x0001));  // check the DONE bit
    return ADC1BUF0;  // when done, you can copy the value from ADC1BUF0
}
