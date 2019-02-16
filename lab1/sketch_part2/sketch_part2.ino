#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("3+1 + |3-1| is: ");
  Serial.println(testasm( 1, 3));
  Serial.print("4+6 + |4-6| is: ");
  Serial.println(testasm( 4, 6));
  Serial.print("10+18 + |10-18| is: ");
  Serial.println(testasm( 10, 18));
  Serial.print("128+124 + |128-124| is: ");
  Serial.println(testasm(128,124));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
