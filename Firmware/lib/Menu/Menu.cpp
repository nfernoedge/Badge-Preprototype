
/*
Created By: James Campbell (aka. Soups or Soups71)
Last Updated: 22JUL24
*/

#include "Menu.h"

Menu::Menu(Adafruit_ST7735 *tftScreen, String usrTitle, FunctionStruct* usrChoices, int usrNumChoices, int usrIfMenu){
    choices = usrChoices;
    screen = tftScreen;
    title = usrTitle;
    numChoices = usrNumChoices;
    ifMenu = usrIfMenu;
}

// Print Menu that has options to select from
void Menu::printMenu(){
    
    screen -> setCursor(0,0);
    screen -> fillScreen(bgColor);
    screen -> setTextColor(ST77XX_WHITE, bgColor);
    screen -> setTextSize(2);
    screen -> println(title);
    screen -> println();
    for(int i = 0; i<numChoices; i++){
        screen -> print("  ");
        screen -> println(choices[i].name);
    }

    updateOption(0);
}
// Print just plain text under the title
void Menu::printText(){
    currLineNum = 0;
    screen -> setCursor(0,0);
    screen -> fillScreen(bgColor);
    screen -> setTextColor(ST77XX_WHITE, bgColor);
    screen -> setTextSize(2);
    screen -> println(title);
    screen -> println();
    screen -> setTextSize(1);
    for(int i = 0; i<numChoices; i++){
        screen -> println(setw(choices[i].name, 26));
    }
}
// Set the star on the correct option on the screen
int Menu::updateOption(int reqOption){
    
    screen -> setTextColor(ST77XX_WHITE, bgColor);
    screen -> setTextSize(2);

    for(int i = 2; i<currCursor.yPos_max; i++){
        screen -> setCursor(0, i*currCursor.dy );
        screen -> print(" ");
    }
    // if(reqOption >= numChoices){
    //     reqOption = 0;
    // }
    // if(reqOption < 0){
    //     reqOption = numChoices-1;
    // }

    // Comment out if you want scroll
    if(reqOption >= numChoices){
        reqOption = numChoices-1;
    }
    if(reqOption < 0){
        reqOption = 0;
    }
    if(reqOption ==0  and currCursor.option == numChoices-1){
        currLineNum=0;
    }else{
        if(reqOption-currLineNum>5){
        downScroll(2,0);
        }
        if(reqOption-currLineNum<0){
            upScroll(2,0);
        }
    }
    screen -> setCursor(0, ((reqOption-currLineNum)+2)*currCursor.dy );
    screen -> print("*");
    currCursor.option = reqOption;
    return 0;

}
// Move the * up to the higher option;
int Menu::upChoice(){
    updateOption(currCursor.option-1);
    return 0;
}
// Move the * down to the lower option;
int Menu::downChoice(){
    updateOption(currCursor.option+1);
    return 0;
}

// This just centers the text
String Menu::setw(String inputStr, int width){
    int origLength = inputStr.length();
    int whitespace = width - origLength;
    // This is purposefully lossy
    if(whitespace<=0){
        return inputStr;
    }
    int front = whitespace/2;
    String formatted = "";
    for(int i = 0; i<front; i++){
        formatted+=" ";
    }
    formatted+= inputStr;
    return formatted;
}
// Used to scroll up through text if there are not options to select from. Does not wrap
int Menu::upScroll(int textsize, int width){
    screen -> setCursor(0,0);
    screen -> fillScreen(bgColor);
    screen -> setTextColor(ST77XX_WHITE, bgColor);
    screen -> setTextSize(2);
    screen -> println(title);
    screen -> println();
    screen -> setTextSize(textsize);


    currLineNum--;
    if(currLineNum<0){
        currLineNum =0;
    }
    for(int i = currLineNum; i<(numChoices); i++){
        if(width){

        screen -> println(setw(choices[i].name, 26));
        }else{
                screen -> print("  ");
                screen -> println(choices[i].name);
        }
    }
    return 0;
}

// Used to scroll down through text if there are not options to select from. Does not wrap
int Menu::downScroll(int textsize, int width){
    screen -> setCursor(0,0);
    screen -> fillScreen(bgColor);
    screen -> setTextColor(ST77XX_WHITE, bgColor);
    screen -> setTextSize(2);
    screen -> println(title);
    screen -> println(); 
    screen -> setTextSize(textsize);
    currLineNum++;
    
    if(currLineNum >= numChoices){
        currLineNum = numChoices-1;
    }
    for(int i = currLineNum; i<(numChoices); i++){
        if(width){

        screen -> println(setw(choices[i].name, 26));
        }else{
                screen -> print("  ");
                screen -> println(choices[i].name);
        }
    }
    return 0;
}