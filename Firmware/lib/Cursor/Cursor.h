/*
Created By: James Campbell (aka. Soups or Soups71)
Last Updated: 22JUL24
*/

/*
Cursor Class

This class is used for the *imaginary* cursor on the screen.
The Dx and Dy values are for the text size of 2;

*/

#ifndef Cursor_h
#define Cursor_h
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
class Cursor{
    public:
        int option = 0;
        int dy = 16;
        int dx = 12;
        int xPos = 0;
        int yPos = 0;
        const int xPos_max = 12;
        const int yPos_max = 8;
        Cursor();


};
#endif