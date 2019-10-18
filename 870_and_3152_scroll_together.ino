/*
 * This is a test sketch combining (4) Adafruit 8x8 Mini Matrix (PID 870) and (3) Adafruit 8x16 Mini Matrix (PID 3152)
 * The 870's are addressed 0x70, 0x71, 0x72, & 0x73
 * The 3152's are addressed 0x74, 0x75, 0x76
 * 
 * This is expandable to a full (4) 870's and (4) 3152's
 * 
 * Notes are on the side of each line to explain what it does and what it modifies
 */
//============  Includes/Defines  ============
#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

#define MATRICES_8x8 4                                                // Number of 870 modules
#define MATRICES_16x8 3                                               // Number of 3152 modules
#define data_time_delay 50                                            // Lower number speeds up scroll, higher number slows down

Adafruit_8x8matrix matrix8[MATRICES_8x8];
Adafruit_8x16minimatrix matrix16[MATRICES_16x8];

//============  Variables  ============
// -------- Matrix Display Variables --------
char* msg[] = {"Message # 0     .","Message # 1     .","Message # 2     .","Message # 3     ."};
int numOfMsg = 4;                                                     // This is the number of messages in the above array
int msgChoice = 0;                                                    // Which message in the array to display. 0 = Message # 0.
int runCount = 0;                                                     // How many times has the entire message scrolled

// -------- Scroll Setup Data --------
int modules = MATRICES_8x8 + MATRICES_16x8;                           // Total number of Modules
int matrices = MATRICES_8x8 + (MATRICES_16x8*2);                      // Number of physical 8x8 LED matrix
int offset = 12;                                                      // Start offset. 12 => msg begins @ 3rd 3152. 10 => msg begins @ 2nd 3152. 8 => msg begins @ 1st 3152

//============  Setup Function  ============
void setup() {  
  // -------- Setup 8x8 --------
  for(uint8_t c = 0; c < MATRICES_8x8; c++) {                         // Setup for the 870 8x8 Mini Matrices
    matrix8[c].begin(0x70 + c);                                       // Address modules 0x70-0x73
    matrix8[c].clear();                                               // Clear all data on module (turn all leds off)
    matrix8[c].setTextSize(1);                                        // Sets text size to 8 pixels tall (1 matrix tall)
    matrix8[c].setTextWrap(false);                                    // we dont want text to wrap so it scrolls nicely
    matrix8[c].setRotation(0);                                        // 0 = 4 pin Connector @ Top; 1 = 4 pin Connector @ Left; 2 = 4 pin Connector @ Bottom; 3 = 4 pin Connector @ Right when looking at the matrix side of the board
  }

  // -------- Setup 16x8 --------
  for(uint8_t d = 0; d < MATRICES_16x8; d++) {                        // Setup for the 3152 16x8 Mini Matrices
    matrix16[d].begin(0x74 + d);                                      // Address modules starting at 0x74
    matrix16[d].clear();                                              // Clear all data on module (turn all leds off)
    matrix16[d].setTextSize(1);                                       // Sets text size to 8 pixels tall (1 matrix tall) so we have our matrix set at 16 long by 8 tall
    matrix16[d].setTextWrap(false);                                   // we dont want text to wrap so it scrolls nicely
    matrix16[d].setRotation(1);                                       // 0 = I2C connection @ top left (16 tall, 8 wide); 1 = I2C connection @ top right (8 tall, 16 wide); 2 = I2C connection @ bottom right; 3 = I2C connection @ bottom left when looking at the matrix side of the board
  }
}

//============  Loop Function  ============
void loop() {
  scroll(msg[msgChoice]);                                             // "scroll()" is the fucntion.  "msg[]" is the array where the words are stored, "msgChoice" is which message do we want from the array.
}

//------ Scrolling Message ------
void scroll(char* text) {  
  int scrollPositions = (strlen(text) * 8);// - (8 * offset);
  for (int x = (offset * matrices); x >= -scrollPositions; x--) {     // "x" has to equal the total of *ALL* modules, both 8x8 and 16x8, times 8
    for(uint8_t m = 0; m <  (modules); m++) {                         // Got to make sure "m" steps through every single module
      if(m < MATRICES_8x8){                                           // This starts the 870 control section. If "m" is bewteen 0 and 3, direct the code to work on the 8x8 modules
        matrix8[m].clear();                                                                 
        matrix8[m].setCursor((x - (m * 8)), 0);
        matrix8[m].print(text);
        matrix8[m].writeDisplay();
      } else {                                                        // If "m" is 4 or higher, now send code to the 3152 modules
        matrix16[m - MATRICES_8x8].clear();                           // "m-MATRICES_8x8" this is done because when we initialized the 3152 modules, we created the array at 0, 1, 2, etc.
        matrix16[m - MATRICES_8x8].setCursor((x - (m * 16)), 0);      // By using the "m-MATRICES_8x8", we adjust the array pointer to address the correct 3152 modules
        matrix16[m - MATRICES_8x8].print(text);
        matrix16[m - MATRICES_8x8].writeDisplay();
      }
    }
    delay(data_time_delay);                                           // pause.  I have not found a way to make this work on a "millis()" basis.  The only way so far has been using a hard "delay()" line
  }
  runCount++;                                                         // Message counter.  Each time the entire message passes, this will count up.
  if (runCount > 2){                                                  // "msgCount > 2" gives 3 times before moving on to the next message
    if (msgChoice < numOfMsg) {                                       // Makes sure that as we count up, we don't go higher than the number of messages
      msgChoice++;
    } else {
      msgChoice = 0;
    }
    runCount = 0;
  }
}
