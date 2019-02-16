void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello, world!");
  //loop();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 10; i++)
  {
    delay(100);
    Serial.println("Hello, world!");
  }
}
