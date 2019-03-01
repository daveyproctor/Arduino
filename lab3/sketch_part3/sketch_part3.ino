

/**************************************************************************
 This is from an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using SPI to communicate
 4 or 5 pins are required to interface.

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Comment out above, uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);
*/

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

// Rotary
// Help for the rotary encoding comes from the following source:
// http://arduinotronics.blogspot.com/2013/09/using-rotary-encoder-with-arduino.html

// These two pins will tell us the meaning of the rotary encoder.
#define rotaryPinA 2
#define rotaryPinB 3

// The pattern of high and low when turning the encoder goes as follows:
// Clockwise (CW): A-high, B-high, A-low, B-low
// Counter-clockwise (CCW): B-high, A-high, B-low, A-low


volatile int ballSpeed = 3; // Speed of the ping-pong ball on screen
volatile unsigned int dialPos = 0; // Dial counter
unsigned int dialLastPos = 0; // Manages Change
static boolean rotating = 0; // Manages debouncing
boolean A_set = 0; // More stuff for debouncing
boolean B_set = 0;

// These are the boundaries for our speed.
const int MAXSPEED = 12;
const int MINSPEED = 0;
const int UNITSPEED = 1;

// Go:
void setup() {
  // Rotary Encoder
  // Sets the modes of our pins and pullup resistors for debouncing purposes.
  pinMode(rotaryPinA, INPUT_PULLUP);
  pinMode(rotaryPinB, INPUT_PULLUP);
  // Initialize the code to detect interrupts on both of the rotary pins.
  attachInterrupt(digitalPinToInterrupt(rotaryPinA), A_First, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryPinB), B_First, CHANGE);
  Serial.begin(9600);

  // Display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
//  display.clearDisplay();
//  display.display();

  int boundaries[2][2] = {{0,0},{SCREEN_WIDTH, SCREEN_HEIGHT}};
  int velocity[2] = {1,1};
  int position[2] = {16,35};

  int botHeight = SCREEN_HEIGHT/3;
  int botPosn[2] = {0,0};
  int botSpeed = 3;
  int botVel[2] = {1,1};

  int decisionTime = 0;
  
  while (true){
    // Rotary
    rotating = 1; // Reset the debouncer
    if (dialLastPos != dialPos)
    {
      dialLastPos = dialPos;
    }
    Serial.println(ballSpeed);
    
    display.setCursor(10, 0);
    display.println(F("scroll"));


    // Display
    // Ball
    // Bounce
    if (position[0]-2 < 0){
      // Win?
      velocity[0] = 1;
    }
    if (position[0]+2 > SCREEN_WIDTH){
      // Win?
      velocity[0] = -1;
    }
    if (position[1]-2 < 0){
      velocity[1] = 1;
    }
    if (position[1]+2 > SCREEN_HEIGHT){
      velocity[1] = -1;
    }
    // Update position based on velocity
    for (int i = 0; i < 2; i++){
      position[i] += velocity[i] * (ballSpeed);
    }
    // Draw ball
    for (int i = 0; i < 2; i++){
      for (int j = 0; j < 2; j++) {
        display.drawPixel(position[0]+i, position[1]+j, WHITE);
      }
    }

    decisionTime += 1;
    // Bots
    for (int i = 0; i < 1; i++){
      if (decisionTime == 8) {
        decisionTime = 0;
        if (botPosn[i] < 0 || botPosn[i]+botHeight < position[1]){
          // Start going up
          botVel[i] = 1; // -botVel[i];
        }
        else if (botPosn[i]+botHeight > SCREEN_HEIGHT || botPosn[i] > position[1]){
          // Start going down
          botVel[i] = -1;
        }  
      }
      
      botPosn[i] += botVel[i] * botSpeed;
      for (int j = 0; j < botHeight; j++){
        display.drawPixel(1+i*(SCREEN_WIDTH-3), botPosn[i]+j, WHITE);
      }
    }
    
    display.display();
    display.clearDisplay();
    delay(5);
  }
  
}

void loop() {
}


// Rotary
// Interrupt on a change in A

void A_First(){
  if (rotating) delay(1); // A bit of debouncing
  if(digitalRead(rotaryPinA) != A_set) // Did the value on the pin actually change, or was it a fluke?
  {
    A_set = !(A_set); 
    // If not a fluke, and if A is leading B (clockwise rotation),
    // iterate ballSpeed up one within its limits.
    if (A_set && !(B_set))
    {
        if(ballSpeed < MAXSPEED)
        {
          ballSpeed += UNITSPEED;
        }
    }
    rotating = 0;
  }
}

// Interrupt on a change in B
// Essentially, do the same thing as A, but iterate down instead of up,
// as the rotary would be turning in the opposite direction

void B_First(){
  if (rotating) delay(1); 
  if(digitalRead(rotaryPinB) != B_set)
  {
    B_set = !(B_set); 
    // If not a fluke, and if A is leading B (clockwise rotation),
    // iterate ballSpeed down within its limits.
    if (B_set && !(A_set))
    {
        if(ballSpeed > MINSPEED)
        {
          ballSpeed -= UNITSPEED;
        }
    }
    rotating = 0;
  }
}
