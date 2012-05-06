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
    I2CWrite(COMPASS, 0x06, -1);
    DXRA = I2CRead(COMPASS);
    DXRB = I2CRead(COMPASS);
    DZRA = I2CRead(COMPASS);
    DZRB = I2CRead(COMPASS);
    DYRA = I2CRead(COMPASS);
    DYRB = I2CRead(COMPASS);
    I2CWrite(COMPASS, 0x03, -1);
    orientation.X = DXRA << 8 | DXRB;
    orientation.Y = DYRA << 8 | DYRB;
    orientation.Z = DZRA << 8 | DZRB;
    return orientation;
}
