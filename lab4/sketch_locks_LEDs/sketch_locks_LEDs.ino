#include "concurrency.h"
// enum Colors {BLUE=6, YELLOW=8, GREEN=10, WHITE=12};

lock_t l;

int turn = -1;

void p0 (void)
{
    /* process 0 here */
    while(1) {
        lock_acquire(&l);
        // atomicity of things within lock:
        turn = 0;
        digitalWrite(GREEN, HIGH);
        digitalWrite(GREEN, LOW);
        if (turn != 0){
            digitalWrite(WHITE, HIGH);
        }
        lock_release(&l);
    }
}

unsigned long j = 0;
void p1 (void)
{
    /* process 1 here */
    while(1) {
        lock_acquire(&l);
        turn = 1;
        digitalWrite(YELLOW, HIGH);
        digitalWrite(YELLOW, LOW);
        if (turn != 1){
            digitalWrite(WHITE, HIGH);
        }
        lock_release(&l);
    }
}

unsigned long k = 0;
void p2 (void)
{
    /* process 2 here */
    while(1) {
        lock_acquire(&l);
        turn = 2;
        digitalWrite(BLUE, HIGH);
        digitalWrite(BLUE, LOW);
        if (turn != 2){
            digitalWrite(WHITE, HIGH);
        }
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

    lock_init(&l);
    if (process_create (p0, 64) < 0) {
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    if (process_create (p1, 64) < 0) {
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    if (process_create (p2, 64) < 0) {
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    process_start();
}

void loop()
{
}

