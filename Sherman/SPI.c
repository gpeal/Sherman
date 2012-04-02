#include "SPI.h"

void setupSPISlave()
{
    TRISCbits.TRISC4 = 1;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD9 = 0;
    TRISDbits.TRISD10 = 1;
    //Clears config register
    SPI1CON = 0;
    //Clears receive buffer
    int rData = SPI1BUF;
    //Clears any existing event (rx / tx/ fault interrupt)
    IFS0CLR = 0x03800000;
    //Clears overflow
    SPI1STATCLR = 0x40;      
    //Enables the SPI channel (channel, master mode enable | use 8 bit mode | turn on, clock divider)
    // divide fpb by 1024, configure the I/O ports.
    SpiChnOpen(1, SPI_CON_SLVEN | SPI_CON_MODE8 | SPI_CON_ON, 1024);   
}

void setupSPIMaster()
{
   //Clears receive buffer
   int rData = SPI1BUF;
   //Clears any existing event (rx / tx/ fault interrupt)
   IFS0CLR = 0x03800000;
   //Clears overflow
   SPI1STATCLR = 0x40;      
   //Enables the SPI channel (channel, master mode enable | use 8 bit mode | turn on, clock divider)
   // divide fpb by 1024, configure the I/O ports
   SpiChnOpen(1, SPI_CON_MSTEN | SPI_CON_MODE8 | SPI_CON_ON, 1024);
}

char readSPI()
{
    char c = -1;
    if(!SpiChnRxBuffEmpty(1))
        c = SpiChnGetC(1);
    return c;
}

void WriteSPI(char c)
{
    int receive, i;
    //send character
    putcSPI1(c);
    //wait
    for(i = 0; i < 1000000; i++) {}
    ///Clear interrupt flags (Tx / Rx buffers empty)
    SpiChnClrIntFlags(1); 
    //Read SP1BUF (dummy read)
    receive = SPI1BUF;
    //Write SP1BUF- sets Tx flag, if not done read will not clock
    SPI1BUF = 0x0;
    //Generates clock and reads SDO
    receive = getcSPI1();
    
}