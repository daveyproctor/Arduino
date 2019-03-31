#include "concurrency.h"

lock_t l;

int turn = -1;

void p0 (void)
{
    /* process 0 here */
    // Simple terminating process
    digitalWrite(GREEN, HIGH);
    return;
}

unsigned long j = 0;
void p1 (void)
{
    /* process 1 here */
    while(1) {
        digitalWrite(YELLOW, HIGH);
        digitalWrite(YELLOW, LOW);
        if (++j == 100000){
            break; // termintates light off
        }
    }
    // chain last process
    if (process_create (p2, 64) < 0) {
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
}

unsigned long k = 0;
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
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    if (process_create (p1, 64) < 0) {
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    process_start();
}

void loop()
{
}

