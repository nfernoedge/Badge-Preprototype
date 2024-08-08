/*
Created By: James Campbell (aka. Soups or Soups71)
Last Updated: 22JUL24
*/

#ifndef Menu_h
#define Menu_h
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h>
#include "Cursor.h"


typedef void (*FunctionPointer)();

typedef struct {
    String name;
    FunctionPointer func;
} FunctionStruct;



class Menu{
    public:
        Adafruit_ST7735 *screen;
        Cursor currCursor;
        String title;
        int numChoices;
        int ifMenu;
        FunctionStruct* choices;
        int16_t bgColor = ST77XX_BLACK;
        Menu(Adafruit_ST7735 *tftScreen, String usrTitle, FunctionStruct* usrChoices, int usrNumChoices, int usrIfMenu);


        void printMenu();
        void printText();
        int updateOption(int reqOption);
        int upChoice();
        int downChoice();
        int upScroll(int textsize, int width);
        int downScroll(int textsize, int width);
        String setw(String inputStr, int width);

    private:
        int currLineNum = 0;

};
#endif