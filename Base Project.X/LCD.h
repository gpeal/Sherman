#ifndef LCD_H
#define LCD_H

void setupLCD();
void LCDcommand(int command, int d7, int d6, int d5, int d4, int d3, int d2, int d1, int d0);
//Write a character to the cursor position in the LCD
void LCDWriteChar(char c);
//Write a string to the LCD starting at this position
//row and col are 1 based
void LCDWriteString(char* str, int row, int col);
//Clears the designated line in the LCD
//0 clears both lines
void LCDClear(int line);
void wait();

#endif