#include "concurrency.h"

void p1 (void)
{
  return;
  /* process 1 here */
  int i = 0;
  int l = 0;
  int j = 0;
  // asm volatile ("cli\n\t");
  while(1){
    // Serial.println("-p1 Hi from p1");
    digitalWrite(YELLOW, HIGH);
    // continue;
    // digitalWrite(YELLOW, 1);
    // delay(1000);
    // digitalWrite(YELLOW, 0);
    // delay(1000);
    continue;

    i++;
    if (i > 10000){
      i = 0;
      l++;
      if (l > 10000){
        i = 0;
        l = 0;
        // j = 1-j;
        digitalWrite(YELLOW, HIGH);
        digitalWrite(YELLOW, LOW);
      }
    }
  }
  return;
 }

void p2 (void)
{
  return;
  /* process 2 here */
  int i = 0;
  int j = 0;
  while(1){
    // Serial.println("-p2 Hi from p2");
    digitalWrite(BLUE, 1);
    // delay(1000);
    // digitalWrite(BLUE, 0);
    // delay(1000);
    continue;

    i++;
    if (i > 10000){
      i = 0;
      j = 1-j;
      digitalWrite(BLUE, j);
    }
  }
 return;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");
  Serial.println(sizeof(int));
  Serial.println(HIGH);
  Serial.println(LOW);
  delay(1000);
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  digitalWrite(GREEN, HIGH);
  delay(200);
  digitalWrite(GREEN, LOW);
  delay(200);
  // digitalWrite(6, 1);
  // delay(1000);
  // digitalWrite(6, 0);
  // delay(1000);
  // digitalWrite(8, 1);
  // delay(1000);
  // digitalWrite(8, 0);
  // delay(1000);

  // p1();
  // p2();

  if (process_create (p1, 64) < 0) {
    Serial.println("FAIL");
    return;
  }
  if (process_create (p2, 64) < 0) {
    Serial.println("FAIL");
    return;
  }
  process_start();
//   asm volatile ("sei\n\t"); // REQUIRED, WOW! But only once.
  // WHAT WE KNOW:
  // returns once from process_start with interrupts disabled.
  // returns 5 more times into the loop, interrupts enabled.
  // Then functions properly.

  // Serial.println("oops");
}

void loop()
{
  // Serial.println("oops");
  /* if you get here, then all processes are either finished or
     there is deadlock */
  int i=0;
  while (1){
    Serial.println(++i);           // -- exactly 5 times
    // digitalWrite(GREEN, HIGH);  // -- yes
    // digitalWrite(GREEN, LOW);   // -- no, only flash, so loop not iterating.
    // asm volatile ("sei\n\t");   // not necessary
  };
}



// void setup(){
//   Serial.begin(9600);
// }

// void loop(){
//   Serial.println("hi");
// }
