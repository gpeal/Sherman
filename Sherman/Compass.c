#include "Compass.h"
#include "I2C.h"
#include "Components.h"

void setupCompass()
{
    I2CWrite(COMPASS, 0x00, 0x70);
    I2CWrite(COMPASS, 0x01, 0xA0);
    I2CWrite(COMPASS, 0x02, 0x00);
}

struct Orientation readCompass()
{
    unsigned char DXRA, DXRB, DYRA, DYRB, DZRA, DZRB;
    struct Orientation orientation;
    StartI2C1(); // Send the Start Bit (begin of data send)
    IdleI2C1(); // Wait to complete
    
    MasterWriteI2C1 (0x3D); // address
    IdleI2C1();
    while( !I2C1STATbits.ACKSTAT==0 ){}

    MasterWriteI2C1 (0x06); // command line
    IdleI2C1();
    while( !I2C1STATbits.ACKSTAT==0 ){}                                //Wait to complete

    DXRA = MasterReadI2C1();		//Read in a value
    DXRB = MasterReadI2C1();		//Read in a value
    DZRA = MasterReadI2C1();		//Read in a value
    DZRB = MasterReadI2C1();		//Read in a value
    DYRA = MasterReadI2C1();		//Read in a value
    DYRB = MasterReadI2C1();		//Read in a value
    StopI2C1();                                 //Send line stop condition
    IdleI2C1();

    StartI2C1(); // Send the Start Bit (begin of data send)
    IdleI2C1(); // Wait to complete
    
    MasterWriteI2C1 (0x3C); // address
    IdleI2C1();
    while( !I2C1STATbits.ACKSTAT==0 ){}

    MasterWriteI2C1 (0x03); // command line
    IdleI2C1();
    while( !I2C1STATbits.ACKSTAT==0 ){}

    StopI2C1();                                 //Send line stop condition
    IdleI2C1();                                 //Wait co complete
    orientation.X = (DXRA << 8) | DXRB;
    orientation.Y = (DYRA << 8) | DYRB;
    orientation.Z = (DZRA << 8) | DZRB;
    return orientation;
}
