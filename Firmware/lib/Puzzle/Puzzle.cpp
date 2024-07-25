/*
Created By: James Campbell (aka. Soups or Soups71)
Last Updated: 22JUL24
*/

#include "Puzzle.h"
Puzzle::Puzzle(Adafruit_ST7735 *tftScreen, String usrTitle){
    screen = tftScreen;
    title = usrTitle;
    currValue = 0;
    currValString = String();
}

void Puzzle::printNewGame(){
    screen -> setCursor(0,0);
    screen -> fillScreen(ST77XX_BLACK);
    screen -> setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    screen -> setTextSize(2);
    screen -> println(title);
    screen -> println();
    screen -> setCursor((5*currCursor.dx), (3*currCursor.dy));
    setw(4);
    screen->println(currValString);
    screen -> setCursor((5*currCursor.dx), (4*currCursor.dy));
    screen -> print("^");

}

int Puzzle::moveCarrotRight(){

    screen -> setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    screen -> setTextSize(2);
    screen -> setCursor((carrotIdx+5)*currCursor.dx, (4)*currCursor.dy );
    screen -> print(" ");

    carrotIdx++;
    if(carrotIdx>3){
        carrotIdx = 3;
        printWord();
    }
    screen -> setCursor((carrotIdx+5)*currCursor.dx, (4)*currCursor.dy );
    screen -> print("^");
    return 0;

}
int Puzzle::moveCarrotLeft(){    
    screen -> setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    clearWord();
    screen -> setTextSize(2);
    screen -> setCursor((carrotIdx+5)*currCursor.dx, (4)*currCursor.dy );
    screen -> print(" ");

    carrotIdx--;
    if(carrotIdx<0){
        carrotIdx = 0;
    }
    screen -> setCursor((carrotIdx+5)*currCursor.dx, (4)*currCursor.dy );
    screen -> print("^");
    return 0;
}
int Puzzle::upChoice(){
    clearWord();
    updateVal(1);
    return 0;
}
int Puzzle::downChoice(){
    clearWord();
    updateVal(-1);
    return 0;
}
int Puzzle::updateVal(int increase){
    /*
    0 is the thousands place
    1 is the hundreds
    2 is the tenths
    3 is the ones
    */
   
   switch (carrotIdx)
   {
    case 0:
        if(!((currValString.charAt(carrotIdx)=='0' && increase == -1)||  (currValString.charAt(carrotIdx) == '9' && increase ==1))){
            currValue+=1000*increase;
        }
        break;
    case 1:
        if(!((currValString.charAt(carrotIdx)=='0' && increase == -1)||  (currValString.charAt(carrotIdx) == '9' && increase ==1))){
            currValue+=100*increase;
        }
        break;
    case 2:
        if(!((currValString.charAt(carrotIdx)=='0' && increase == -1)||  (currValString.charAt(carrotIdx) == '9' && increase ==1))){
            currValue+=10*increase;
        }
        break;
    case 3:
        if(!((currValString.charAt(carrotIdx)=='0' && increase == -1)||  (currValString.charAt(carrotIdx) == '9' && increase ==1))){
            currValue+=increase;
        }
        break;
    default:
        break;
   }
   Serial.println(currValString);
   screen -> setTextSize(2);
    screen -> setCursor((5*currCursor.dx), (3*currCursor.dy));
    setw(4);
    screen->println(currValString);
    return 0;
}
int Puzzle::setw(int width){
    currValString = String(currValue);
    while (currValString.length() < width) {
        currValString = "0" + currValString;
    }
    return 0;
}
void Puzzle::printWord(){
    screen -> setCursor(0, (5*currCursor.dy));
    screen -> setTextSize(1);
    screen -> print(setw(String("vetconactual.com/dc32/"), 28));
    screen -> setCursor(0, (6*currCursor.dy));
    screen -> setTextSize(1);
    screen -> print(setw(String(endpoints[currValue]), 28));
    Serial.println(setw(String(endpoints[currValue]), 26));
    
}
void Puzzle::clearWord(){
    screen -> setCursor((0), (5*currCursor.dy));
    screen -> setTextSize(1);
    screen -> print("                         ");
    screen -> setCursor((0), (6*currCursor.dy));
    screen -> print("                         ");
    screen -> setTextSize(2);
}
String Puzzle::setw(String inputStr, int width){
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