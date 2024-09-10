/*
Created By: James Campbell (aka. Soups or Soups71)
Last Updated: 22JUL24
*/

#include <Arduino.h>

#include <AnimatedGIF.h>     // Used to play the GIFs
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include "Menu.h"
#include "Puzzle.h"
// Constants
#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 128

#define TFT_CS       44
#define TFT_RST      6
#define TFT_DC       43
#define TFT_MOSI     9
#define TFT_SCLK     7 

#define SD_CS        5

#define up_button    4
#define down_button  3
#define left_button  1
#define right_button 2

#define rotation     3
#define EEPROM_SIZE 2
// These get updated in the initSDCard function
int afImgNum = 1;
int armyImgNum = 1;
int cgImgNum = 1;
int marineImgNum = 1;
int navyImgNum = 1;
int spaceImgNum = 1;

// Init Checks
void initSDCard();
void initFlash();

// Functions for the playing of the GIFs 
void * GIFOpenFile(const char *fname, int32_t *pSize);
void GIFCloseFile(void *pHandle);
int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen);
int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition);
void GIFDraw(GIFDRAW *pDraw);
void drawImg(char* imgName);

// Functions for different menu options
void main_first();
void main_second();
void main_third();
void main_fourth();
void temp();
void gif_cg();
void gif_af();
void gif_navy();
void gif_marines();
void gif_army();
void gif_space();
void prizeGif();
void resetPuzzle();
// Function to avoid debounce
short buttonPressed(int button);

// Screen object, allows for the drawing of text
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Variables to prevent bounce on the buttons when pressed
unsigned long lastButtonPress = millis();
unsigned long debounceDelay = 250;

// Tracks whether we are in the puzzle or not
int inGame = 0;

// Objects for the GIF player
AnimatedGIF gif;
File f;


int numMainChoices = 4;
FunctionStruct mainChoices[] = {
  {String("GIFs"), main_first},
  {String("Puzzle"), main_second},
  {String("Prizes"), main_third},
  {String("About"), main_fourth}
};

int numAboutChoices = 6;
FunctionStruct aboutChoices[] = {
  {String("Created By:"), temp},
  {String("@neutrino3306"), temp},
  {String("@cklarmann"), temp},
  {String("@nferno2"), temp},
  {String("@.kybr."), temp},
  {String("@soups71"), temp}
};
int numGifChoices = 6;
FunctionStruct gifChoices[] = {
  {String("Coast Guard"), gif_cg},
  {String("Air Force"), gif_af},
  {String("Navy"), gif_navy},
  {String("Marines"), gif_marines},
  {String("Army"), gif_army},
  {String("Space Force"), gif_space}
};
int numPrizeChoices = 9;
FunctionStruct prizeChoices[] = {
  {String("Prize 1"), prizeGif},
  {String("Prize 2"), prizeGif},
  {String("Prize 3"), prizeGif},
  {String("Prize 4"), prizeGif},
  {String("Prize 5"), prizeGif},
  {String("Prize 6"), prizeGif},
  {String("Prize 7"), prizeGif},
  {String("Prize 8"), prizeGif},
  {String("Reset"), resetPuzzle}
};
int numErrorChoices = 1;
FunctionStruct errorChoices[] = {
  {String("An error has been \nidentified with the\nSD Card!\nPlease check that the\nsd-card is properly\ninserted. Reboot\nthe device to\ntry again...\n\n\nThanks Crowdstrike..."), temp}
};

// Menus used for the interface
Menu mainMenu = Menu(&tft, String(" VETCON 2024 "), mainChoices, numMainChoices, 1);
Menu aboutMenu = Menu(&tft, String("    About    "), aboutChoices, numAboutChoices, 0);
Menu gifMenu = Menu(&tft, String  ("     GIF     "), gifChoices, numGifChoices, 1);
Menu prizeMenu = Menu(&tft, String("   Prizes"), prizeChoices, numPrizeChoices, 1);
Menu errorMenu = Menu(&tft, String("    BSOD!    "), errorChoices, numErrorChoices, 0);
Puzzle puzzleGame = Puzzle(&tft, String("    PUZZLE"));

// Pointers to track state of the menues
Menu* currentMenu = &mainMenu;
Menu* prevMenu = &mainMenu;


void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello, from @Soups71 and the VETCON team :)");
  EEPROM.begin(2);
  if(EEPROM.read(1)!=1){
    initFlash();
  }
  errorMenu.bgColor = ST77XX_BLUE;
  // Seed the random number generator forso we pick different GIFs each reboot 
  randomSeed(analogRead(0));
  
  tft.initR(INITR_BLACKTAB);
  // Sets the screen to be rotated in the right direction.
  // If upside down; set to 1 instead of 3
  tft.setRotation(3);
  // Set the button inputs
  pinMode(up_button, INPUT);
  pinMode(down_button, INPUT);
  pinMode(left_button, INPUT);
  pinMode(right_button, INPUT);

  // This is for the GIF Player
  // If the colors look inverted; use BIG_ENDIAN_PIXELS
  gif.begin(LITTLE_ENDIAN_PIXELS);

  // Check if the SD card is correctly inserted and can be opened.
  // Also, gets the correct number of gifs for each service
  // Function takes appox. 2 sec to run on 90 gifs
  initSDCard();
  // Read level information stored on EEPROM
  puzzleGame.level = EEPROM.read(0);
  puzzleGame.getCustColors(puzzleGame.randint, puzzleGame.menu_colors);
  // If everything has worked to this point, we can show the main menu
  mainMenu.printMenu();

}

void loop() {
  if(!inGame){
    if(buttonPressed(up_button)){
      if(currentMenu -> ifMenu){
        currentMenu -> upChoice();
      }else{
        //scroll
        currentMenu -> upScroll(1, 1);
      }
    }else if(buttonPressed(down_button)){
      if(currentMenu -> ifMenu){
        currentMenu -> downChoice();
      }else{
        //scroll
        currentMenu -> downScroll(1, 1);
      }
    }else if(buttonPressed(left_button)){
      currentMenu = prevMenu;
      prevMenu = &mainMenu;
      currentMenu -> printMenu();

    }else if(buttonPressed(right_button)){
      currentMenu -> choices[currentMenu->currCursor.option].func();
    }
  }else{
    if(buttonPressed(up_button)){
     
      puzzleGame.upChoice();
      
    }else if(buttonPressed(down_button)){
      puzzleGame.downChoice();
    }else if(buttonPressed(left_button)){
      if(puzzleGame.carrotIdx ==0){
        puzzleGame.currValue =0;
        currentMenu = prevMenu;
        prevMenu = &mainMenu;
        currentMenu -> printMenu();
        inGame = 0;
      }else{
        puzzleGame.moveCarrotLeft();
      }

    }else if(buttonPressed(right_button)){
        puzzleGame.moveCarrotRight();
        EEPROM.write(0, puzzleGame.getLevel());
        EEPROM.commit();

    }

  }

}

// Opens the GIF File for reading
void * GIFOpenFile(const char *fname, int32_t *pSize)
{
  f = SD.open(fname);
  if (f)
  {
    *pSize = f.size();
    return (void *)&f;
  }
  if(SD.status()){
      errorMenu.printText();
      while (1);
    }
  return NULL;
}
// Closes the GIF file to avoid any problems with the filesystem
// Also dereferences the f object.
void GIFCloseFile(void *pHandle)
{
  File *f = static_cast<File *>(pHandle);
  if(SD.status()){
      errorMenu.printText();
      while (1);
    }
  if (f != NULL)
     f->close();
} 

// Reads the file from the sdCard
int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
    int32_t iBytesRead;
    iBytesRead = iLen;
    File *f = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
       iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
    if (iBytesRead <= 0)
       return 0;
    if(SD.status()){
      errorMenu.printText();
      while (1);
    }
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    
    pFile->iPos = f->position();
    return iBytesRead;
}
// Go to a specfic position in the file
int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{ 
  int i = micros();
  File *f = static_cast<File *>(pFile->fHandle);
  if(SD.status()){
      errorMenu.printText();
      while (1);
    }
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i = micros() - i;
  return pFile->iPos;
} /* GIFSeekFile() */

// Draw a line of image directly on the LCD
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y, iWidth;

    iWidth = pDraw->iWidth;
    if (iWidth + pDraw->iX > DISPLAY_WIDTH)
       iWidth = DISPLAY_WIDTH - pDraw->iX;
    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line
    if (y >= DISPLAY_HEIGHT || pDraw->iX >= DISPLAY_WIDTH || iWidth < 1)
       return; 
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
      for (x=0; x<iWidth; x++)
      {
        if (s[x] == pDraw->ucTransparent)
           s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }

    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency) // if transparency used
    {
      uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
      int x, iCount;
      pEnd = s + iWidth;
      x = 0;
      iCount = 0; // count non-transparent pixels
      while(x < iWidth)
      {
        c = ucTransparent-1;
        d = usTemp;
        while (c != ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent) // done, stop
          {
            s--; // back up to treat it like transparent
          }
          else // opaque
          {
             *d++ = usPalette[c];
             iCount++;
          }
        } // while looking for opaque pixels
        if (iCount) // any opaque pixels?
        {
          tft.startWrite();
          tft.setAddrWindow(pDraw->iX+x, y, iCount, 1);
          tft.writePixels(usTemp, iCount, false, false);
          tft.endWrite();
          x += iCount;
          iCount = 0;
        }
        // no, look for a run of transparent pixels
        c = ucTransparent;
        while (c == ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent)
             iCount++;
          else
             s--; 
        }
        if (iCount)
        {
          x += iCount; // skip these
          iCount = 0;
        }
      }
    }
    else
    {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x=0; x<iWidth; x++)
        usTemp[x] = usPalette[*s++];
      tft.startWrite();
      tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
      tft.writePixels(usTemp, iWidth, false, false);
      tft.endWrite();
    }
}

void drawImg(char* imgName){
  if(!SD.begin(5))
  {
    errorMenu.printText();
    while(1);
  }

  if (gif.open(imgName, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw))
  {
    GIFINFO gi;
    // Loop the gif forever
    while(true){
      // Loop through the frames of the gid to play
      while (gif.playFrame(true, NULL))
      {
        // If the left button is pressed; Then we know to stop playing the gif
        if(buttonPressed(left_button)){
          gif.close();
          return;
        }
      }
      gif.reset();
    }
    gif.close();
  }
  else
  {
    errorMenu.printText();
    while (1);
  }
}

// Enter the GIF Menu
void main_first(){
  currentMenu = &gifMenu;
  prevMenu = &mainMenu;
  currentMenu->printMenu();
}
// Enter the puzzle game
void main_second(){
  inGame = 1;
  puzzleGame.printNewGame();
}
// Enter the puzzle game
void main_third(){
  currentMenu = &prizeMenu;
  prevMenu = &mainMenu;
  currentMenu->printMenu();
}
// Enter the About Menu
void main_fourth(){ 
  currentMenu = &aboutMenu;
  prevMenu = &mainMenu;
  currentMenu->printText();
}


/*
The GIF functions use the following process:

1. Pic a random number within the range of availible images
2. Create the full filename. File name for the gifs MUST be <servicename>##.gif
3. The image is then played on the screen until the user presses the left button
4. Sends the user back to the GIF Menu once they exit the gif.

*/

// CG was selected from the GIF Menu
void gif_cg(){
  int image_number = random(1, cgImgNum);
  char imgName[128];
  sprintf(imgName, "/coastguard%d.gif", image_number);
  drawImg(imgName);
  currentMenu -> printMenu();
  currentMenu -> updateOption(0);
}
// Air Force was selected from the GIF Menu
void gif_af(){
  int image_number = random(1, afImgNum);
  char imgName[128];
  sprintf(imgName, "/airforce%d.gif", image_number);
  drawImg(imgName);
  currentMenu -> printMenu();
  currentMenu -> updateOption(1);
}
// Navy was selected from the GIF Menu
void gif_navy(){
  int image_number = random(1, navyImgNum);
  char imgName[128];
  sprintf(imgName, "/navy%d.gif", image_number);
  drawImg(imgName);
  currentMenu -> printMenu();
  currentMenu -> updateOption(2);
}
// Marine was selected from the GIF Menu
void gif_marines(){
  int image_number = random(1, marineImgNum);
  char imgName[128];
  sprintf(imgName, "/marine%d.gif", image_number);
  drawImg(imgName);
  currentMenu -> printMenu();
  currentMenu -> updateOption(3);
}
// Army was selected from the GIF Menu
void gif_army(){
  int image_number = random(1, armyImgNum);
  char imgName[128];
  sprintf(imgName, "/army%d.gif", image_number);
  drawImg(imgName);
  currentMenu -> printMenu();
  currentMenu -> updateOption(4);
}
// Space Force was selected from the GIF Menu
void gif_space(){
  int image_number = random(1, armyImgNum);
  char imgName[128];
  sprintf(imgName, "/spaceforce%d.gif", image_number);
  drawImg(imgName);
  
  currentMenu -> printMenu();
  currentMenu -> updateOption(5);
}

// Function to eliminate bounce of the button
short buttonPressed(int button){
  if(!digitalRead(button)&&(millis()-lastButtonPress)>debounceDelay){
    lastButtonPress = millis();
    return 1;
  }else{
    return 0;
  }
}

// Check the SD card is properly connected and dynamically determine the number of gifs for each branch.
void initSDCard(){
  File root;
  if (!SD.begin(SD_CS)) {
    errorMenu.printText();
    while(1);
  }
  root = SD.open("/");

  File entry =  root.openNextFile();
  while (entry) {
    if(String(entry.name()).startsWith("coastguard")){
      cgImgNum++;
    }else if(String(entry.name()).startsWith("airforce")){
      afImgNum++;
    }else if(String(entry.name()).startsWith("marine")){
      marineImgNum++;
    }else if(String(entry.name()).startsWith("navy")){
      navyImgNum++;
    }else if(String(entry.name()).startsWith("army")){
      armyImgNum++;
    }else if(String(entry.name()).startsWith("spaceforce")){
      spaceImgNum++;
    }
    entry =  root.openNextFile();
  }
}

void temp(){
  // Legit temp function to avoid crashing
};
void prizeGif(){
int currentChoice = prizeMenu.currCursor.option; // This will be 0->numItems-1, but levels are 

  if(currentChoice<puzzleGame.getLevel()){
    char imgName[128];
    sprintf(imgName, "/challenge%d.gif", currentChoice+1);
    drawImg(imgName);
    currentMenu -> printMenu();
    currentMenu -> updateOption(currentChoice);
  }
  

}
void initFlash(){
  EEPROM.write(0, puzzleGame.updateLevel(0));
  int firstStart = 1;
  EEPROM.write(1, firstStart);
  EEPROM.commit();
}

void resetPuzzle(){
  EEPROM.write(0, puzzleGame.updateLevel(0));
  EEPROM.commit();
  currentMenu -> printMenu();
}