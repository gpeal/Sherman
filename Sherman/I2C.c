#include "I2C.h"
#include "components.h"
#include <plib.h>

void setupI2C() {
    OpenI2C1( I2C_EN, BRG_VAL );
    OpenI2C2( I2C_EN, BRG_VAL );
    TRISAbits.TRISA14 = 0;
    TRISAbits.TRISA15 = 0;
}

void I2CWrite(int component, unsigned char cmd, unsigned char data)
{
    unsigned char address;
    switch(component)
    {
        case DAC:
            address = DAC_ADDRESS;
            break;
    }
    // start the I2C communication
    StartI2C1(); // Send the Start Bit (begin of data send)
    IdleI2C1(); // Wait to complete

    // write the address of the chip, defined by pins AD0 and AD1 on the MAX518
    MasterWriteI2C1 ((address << 1) | 0); // address
    IdleI2C1();
    while( !I2C1STATbits.ACKSTAT==0 ){}

    // write the command to tell the MAX518 to change its output on output 0
    MasterWriteI2C1 (cmd); // command line
    IdleI2C1();
    while( !I2C1STATbits.ACKSTAT==0 ){}

    // wite the value to put on the output
    MasterWriteI2C1(data); // output
    IdleI2C1();
    while( !I2C1STATbits.ACKSTAT==0 ){}

    // end the I2C communication
    StopI2C1(); // end of data send
    IdleI2C1(); // Wait to complete
}
