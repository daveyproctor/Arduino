#include "concurrency.h"
// enum Colors {BLUE=6, YELLOW=8, GREEN=10, WHITE=12};

lock_t l;

void p0 (void)
{
    /* process 0 here */
    while(1) {
        lock_acquire(&l);
        digitalWrite(GREEN, HIGH);
        digitalWrite(GREEN, LOW);
        lock_release(&l);
    }
}

void p1 (void)
{
    /* process 1 here */
    while(1) {
        lock_acquire(&l);
        digitalWrite(YELLOW, HIGH);
        digitalWrite(YELLOW, LOW);
        lock_release(&l);
    }
}

void p2 (void)
{
    /* process 2 here */
    while(1) {
        lock_acquire(&l);
        digitalWrite(BLUE, HIGH);
        digitalWrite(BLUE, LOW);
        lock_release(&l);
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Setup");

    pinMode(BLUE, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(WHITE, OUTPUT);

    digitalWrite(WHITE, HIGH);
    delay(500);
    digitalWrite(WHITE, LOW);
    delay(500);
    if (queueTest(100) == 1){
        Serial.println("Queue test passed");
    }
    delay(1000);

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

