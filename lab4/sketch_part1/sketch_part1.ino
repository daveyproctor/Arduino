#include "concurrency.h"

void p0 (void)
{
    /* process 0 here */
    while(1) {
        digitalWrite(GREEN, HIGH);
        digitalWrite(GREEN, LOW);
    }
}

void p1 (void)
{
    /* process 1 here */
    while(1) {
        digitalWrite(YELLOW, HIGH);
        digitalWrite(YELLOW, LOW);
    }
}

void p2 (void)
{
    /* process 2 here */
    while(1) {
        digitalWrite(BLUE, HIGH);
        digitalWrite(BLUE, LOW);
    }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");
  delay(1000);

  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  digitalWrite(GREEN, HIGH);
  delay(500);
  digitalWrite(GREEN, LOW);
  delay(500);

  if (process_create (p0, 64) < 0) {
    Serial.println("FAIL");
    return;
  }
  if (process_create (p1, 64) < 0) {
    Serial.println("FAIL");
    return;
  }
  if (process_create (p2, 64) < 0) {
    Serial.println("FAIL");
    return;
  }
  process_start();
}

void loop()
{
  /* if you get here, then all processes are either finished or
     there is deadlock */
  Serial.println("oops");
  int i=0;
  while (1){
    Serial.println(++i);
  };
}

