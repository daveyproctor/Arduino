#include "testasm.h"

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.print("The third Fibonacci number is ");
  Serial.println(testasm(3));
  Serial.print("The fifth Fibonacci number is ");
  Serial.println(testasm(5));
  Serial.print("The seventh Fibonacci number is ");
  Serial.println(testasm(7));
  Serial.print("The tenth Fibonacci number is ");
  Serial.println(testasm(10));
  Serial.print("The sixteenth Fibonacci number is ");
  Serial.println(testasm(16));
  pinMode (13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite (13, HIGH);
  delay(400);
  digitalWrite (13, LOW);
  delay(400);
}
