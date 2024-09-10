/*
Created By: James Campbell (aka. Soups or Soups71)
Last Updated: 22JUL24
*/

#ifndef Puzzle_h
#define Puzzle_h
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h>
// #include <EEPROM.h>
#include "Cursor.h"
#include "words.h"
#include "string.h"



class Puzzle{
    public:

        Adafruit_ST7735 *screen;
        Cursor currCursor;
        String title;
        int numChoices;
        int currValue;
        String currValString;
        int carrotIdx;
        int level;
        uint32_t menu_colors[4] = {0x659d14, 0x454017, 0xCda3a6, 0x0e13b7};
        uint32_t randint[8] = {0xFE36ED73, 0x08A56F4E, 0xB2338793, 0x1C57E305, 0x49FFED3A, 0x8A394B25, 0x55F6894A, 0x85ED4062};
        Puzzle(Adafruit_ST7735 *tftScreen, String usrTitle);

        void printNewGame();
        void printText();
        int updateLevel(int level);
        int getLevel();
        int moveCarrotRight();
        int moveCarrotLeft();
        int upChoice();
        int downChoice();
        int updateVal(int increase);
        int setw(int width);
        String setw(String inputStr, int width);
        void printWord();
        void clearWord();
        void randWait();
        void dispColor();
        void getCustColors(uint32_t *v, const uint32_t  *colors);
        void rc4_init(unsigned char *s, unsigned char *key, int keylen);
        void rc4_crypt(unsigned char *s, unsigned char *data, int datalen);
        int hexCharToInt(char c);
        void hexStringToByteArray(const char* hexString, byte* byteArray, int length);

};
#endif
