#ifndef LCD_H
#define LCD_H

void setupLCD();
void LCDcommand(int command, int d7, int d6, int d5, int d4, int d3, int d2, int d1, int d0);
void LCDWriteChar(char c);
void LCDWriteString(char* str);
void wait();

#endif