#include "concurrency.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// enum Colors {BLUE=6, YELLOW=8, GREEN=10, WHITE=12};



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

int boundaries[2][2] = {{0,0},{SCREEN_WIDTH, SCREEN_HEIGHT}};
int velocity1[2] = {1,1};
int position1[2] = {16,35};
int velocity2[2] = {1,1};
int position2[2] = {20,42};

volatile int ballSpeed = 3; // Speed of the ping-pong ball on screen

lock_t l;

void p0 (void)
{
    /* process 0 here */
    for(;;) {
lock_acquire(&l);  // Display Ball Bounce
    if (position1[0]-2 < 0){
      // Win?
      velocity1[0] = 1;
    }
    if (position1[0]+2 > SCREEN_WIDTH){
      // Win?
      velocity1[0] = -1;
    }
    if (position1[1]-2 < 0){
      velocity1[1] = 1;
    }
    if (position1[1]+2 > SCREEN_HEIGHT){
      velocity1[1] = -1;
    }
    // Update position based on velocity
    for (int i = 0; i < 2; i++){
      position1[i] += velocity1[i] * (ballSpeed);
    }
    // Draw ball
    //lock_acquire(&l);
    for (int i = 0; i < 2; i++){
      for (int j = 0; j < 2; j++) {
        display.drawPixel(position1[0]+i, position1[1]+j, WHITE);
      }
    }
    display.display();
    display.clearDisplay();
    delay(5);
  lock_release(&l);
    }
}

/*
void p1 (void)
{
    // process 1 here 
    while(1) {
    
   // Display Ball Bounce
    if (position2[0]-2 < 0){
      // Win?
      velocity2[0] = 1;
    }
    if (position2[0]+2 > SCREEN_WIDTH){
      // Win?
      velocity2[0] = -1;
    }
    if (position2[1]-2 < 0){
      velocity2[1] = 1;
    }
    if (position2[1]+2 > SCREEN_HEIGHT){
      velocity2[1] = -1;
    }
    // Update position based on velocity
    for (int i = 0; i < 2; i++){
      position2[i] += velocity2[i] * (ballSpeed);
    }
    // Draw ball
    for (int i = 0; i < 2; i++){
      for (int j = 0; j < 2; j++) {
        display.drawPixel(position2[0]+i, position2[1]+j, WHITE);
      }
    }
    lock_acquire(&l);
    display.display();
    display.clearDisplay();
    delay(5);
    lock_release(&l);
    }
}
*/

/*
void p2 (void)
{
    // process 2 here
    while(1) {
        lock_acquire(&l);
        digitalWrite(BLUE, HIGH);
        digitalWrite(BLUE, LOW);
        lock_release(&l);
    }
}
*/

void setup()
{
    Serial.begin(9600);
    Serial.println("Setup");

    //pinMode(BLUE, OUTPUT);
    //pinMode(YELLOW, OUTPUT);
    //pinMode(GREEN, OUTPUT); 
    pinMode(WHITE, OUTPUT);

    digitalWrite(WHITE, HIGH);
    delay(500);
    digitalWrite(WHITE, LOW);
    delay(500);
    if (queueTest(100) == 1){
        Serial.println("Queue test passed");
    }
    delay(1000);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

    if (process_create (p0, 500) < 0) {
      Serial.println("FAIL");
      return;
    }
    /*
    if (process_create (p1, 256) < 0) {
      Serial.println("FAIL");
      return;
    }
    */
    /*
    if (process_create (p2, 64) < 0) {
      Serial.println("FAIL");
      return;
    }
    */
    lock_init(&l);
    process_start();
}

int i=0;

void loop()
{
    /* if you get here, then all processes are either finished or
       there is deadlock */
    Serial.println("oops");
    Serial.println(++i); // repeating, yes. gets here interrupts turned off.
    return;
}
