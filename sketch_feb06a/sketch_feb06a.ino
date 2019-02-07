void setup() {
  // https://forum.arduino.cc/index.php?topic=441955.0
  DDRD = B11111110; // set PORTD (digital 7~0) to inputs 
  PORTD = B00000000;//set all input pins high
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print("HI");
  Serial.println(PIND);
  char x = 0;
  int i = 0;
  while (1)
  {
    // Serial.println("Hello");
    if (x != PIND)
    {
      // Serial.println("Hello");
      i++;
      if (i == 10000)
      {
        i = 0;
        x = PIND;
        Serial.println(PIND);
      }
    }
  }
}
