#ifndef SPI_H
#define SPI_H

#include <plib.h>

void setupSPIMaster();
void setupSPISlave();
char readSPI();
void WriteSPI(char c);

#endif