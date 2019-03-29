#include "concurrency.h"


lock_t l;
int turn;

void p0 (void)
{
    /* process 0 here */
    while(1) {
        // l.aquire();
        digitalWrite(GREEN, HIGH);
        digitalWrite(GREEN, LOW);
    }
}

void p1 (void)
{
    /* process 1 here */
    while(1) {
        digitalWrite(YELLOW, LOW);
        // delay(500);
        digitalWrite(YELLOW, HIGH);
        // delay(500);

        // unsigned long m = 1000;
        // unsigned long n = 2*m;
        // for (unsigned long i = 0; i < n; i+=3){
        //     // Serial.println(i);
        //     digitalWrite(YELLOW, LOW);
        // }
        // for (unsigned long i = 0; i < n; i+=3){
        //     // Serial.println(i);
        //     digitalWrite(YELLOW, HIGH);
        // }
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
  pinMode(WHITE, OUTPUT);

  digitalWrite(WHITE, HIGH);
  delay(500);
  digitalWrite(WHITE, LOW);
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
  // locks

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

