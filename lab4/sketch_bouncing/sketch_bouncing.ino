// Much of the code for implementing the bouncing ball display is copied over from lab 3.
#include "concurrency.h"
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


// Each ball has its own position and velocity variables.
int boundaries[2][2] = {{0,0},{SCREEN_WIDTH, SCREEN_HEIGHT}};
int velocity[2] = {1,1};
int position[2] = {16,35};
int velocity2[2] = {1,1};
int position2[2] = {44,68};

volatile int ballSpeed = 3; // Speed of the ping-pong balls on screen

lock_t l; //We need a lock to ensure mutual exclusion of the SPI interface

void p0 (void)
{
  while(1){
  //digitalWrite(2, HIGH);
  //digitalWrite(3, LOW);

  // To be safe, all the ball position calculations and drawings are done within locks.
  lock_acquire(&l);


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

    for (int i = 0; i < 2; i++){
      for (int j = 0; j < 2; j++) {
        display.drawPixel(position[0]+i, position[1]+j, WHITE);
      }
    }
    
    display.display();
    display.clearDisplay();
    delay(2);
    Serial.println("1");
    lock_release(&l);
  }
}


void p1 (void)
{
  while(1){
  //digitalWrite(2, LOW);
  //digitalWrite(3, HIGH);
  lock_acquire(&l);
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
    display.display();
    display.clearDisplay();
    // delay(5);
    lock_release(&l);  
  }
}


void setup()
{
    
    Serial.begin(9600);
    Serial.println("Setup");

    //pinMode(2, OUTPUT);
    //pinMode(3, OUTPUT);

    // Queue test.
    digitalWrite(WHITE, HIGH);
    delay(500);
    digitalWrite(WHITE, LOW);
    delay(500);
    if (queueTest(100) == 1){
        Serial.println("Queue test passed");
    }
    //delay(1000);
    
  // Set up the OLED connection
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Block out memory for the processes.
    if (process_create (p0, 64) < 0) {
      Serial.println("p0 FAIL");
      return;
    }

    
    if (process_create (p1, 64) < 0) {
        Serial.println("p1 FAIL");
        return;
    }
    
    //Initialize locks and start concurrency.
    lock_init(&l);
    process_start();
}

int i = 0;

void loop()
{
   
    /* if you get here, then all processes are either finished or
       there is deadlock */

    // Serial.println("oops"); // This was used for debugging.
    Serial.println(++i); // repeating, yes. gets here interrupts turned off.
    return;
}
