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
    // EEPROM.begin(1);
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
int Puzzle::updateLevel(int newLevel){
  
    level = newLevel;
    return level;
}
int Puzzle::getLevel(){
    // level = EEPROM.read(0);
    return level;
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
    screen -> setTextSize(2);
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
    screen -> setCursor(0, (7*currCursor.dy));
    screen -> setTextSize(1);
    screen -> print(setw(String("vetcon.group/dc32"), 28));
    screen -> setCursor(0, (5*currCursor.dy));
    screen -> setTextSize(1);
    int hexLength = strlen(endpoints[currValue]);
    int dataLength = hexLength / 2;
    byte encryptedData[dataLength];
    hexStringToByteArray(endpoints[currValue], encryptedData, hexLength);
    unsigned char s[256];
    rc4_init(s, (unsigned char*)rc4Key[0], 32);

    // Decrypt the data
    rc4_crypt(s, encryptedData, dataLength);
    encryptedData[dataLength]=0x00;
    // Serial.println("Decrypted: ");
    // Serial.println((char *)encryptedData);
    screen -> print(setw(String((char *)encryptedData), 28));
    printPrize();
    
}
void Puzzle::printPrize(){
    
    for(int i = 0; i<10; i++){
        if(currValue == puzzle_prize[i] && level == i){
            updateLevel((i+1));
            screen -> setCursor(0, (2*currCursor.dy));
            screen -> setTextSize(1);
            
            String winner = String("Unlocked Puzzle ") + String(i+1);
            screen -> print(setw(winner, 28));
            break;
        }else if(currValue == puzzle_prize[i] && level > i){
            screen -> setCursor(0, (2*currCursor.dy));
            screen -> setTextSize(1);
            screen -> print(setw(String("Level Already Unlocked"), 28));
            break;
        }
    }
}
void Puzzle::clearWord(){
    screen -> setTextSize(1);
    screen -> setCursor((0), (5*currCursor.dy));
    screen -> print("                         ");
    screen -> setCursor((0), (6*currCursor.dy));
    screen -> print("                         ");
    screen -> setCursor((0), (7*currCursor.dy));
    screen -> print("                         ");
    screen -> setCursor((0), (2*currCursor.dy));
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
// RC4 implementation
void Puzzle::rc4_init(unsigned char *s, unsigned char *key, int keylen) {
    int i, j = 0, k;
    unsigned char temp;
  
    for (i = 0; i < 256; i++)
        s[i] = i;
    for (i = 0; i < 256; i++) {
        j = (j + s[i] + key[i % keylen]) % 256;
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

void Puzzle::rc4_crypt(unsigned char *s, unsigned char *data, int datalen) {
    int i = 0, j = 0, x;
    unsigned char temp;
  
    for (x = 0; x < datalen; x++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
        data[x] ^= s[(s[i] + s[j]) % 256];
    }
}

// Function to convert a hex character to its integer value
int Puzzle::hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Function to convert a hex string to a byte array
void Puzzle::hexStringToByteArray(const char* hexString, byte* byteArray, int length) {
    for (int i = 0; i < length; i += 2) {
        byteArray[i / 2] = (hexCharToInt(hexString[i]) << 4) + hexCharToInt(hexString[i + 1]);
    }
}
