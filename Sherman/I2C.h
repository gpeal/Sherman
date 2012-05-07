#ifndef I2C_H
#define I2C_h

#define SYS_FREQ 80000000
#define I2CBAUD 100000 // 100 kHz clock
#define BRG_VAL ((SYS_FREQ/2/I2CBAUD)-2)

void setupI2C();
//send a -1 for data if you only need to send 1 byte
void I2CWrite(int component, unsigned char cmd, unsigned char data);

unsigned char I2CRead(int component, int cmd);

#endif