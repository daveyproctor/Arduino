void setup() {
  // https://forum.arduino.cc/index.php?topic=441955.0
  DDRD = DDRD | B11111110; // set PORTD pin 0 to input
  PORTD = B00000000;//set all input pins low
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char x = 0; // Initialize a variable for checking to see if conditions changed.
  int i = 0;  // Initialize a counter for debouncing purposes
  while (1) 
  {
    if (x != PIND) // If the Arduino detects a change...
    {
      // Serial.println("Hello");
      i++;    // Iterate the counter 10000 times to give the Arduino enough time to debounce
      if (i == 10000)
      {
        i = 0;  // Reset the counter
        x = PIND; // Set the check variable equal to the input
        Serial.println(B00000001 & PIND); // Print the value of the input.
        // Use a bit mask to correct for values that we don't want to track and might interfere with results.
      }
    }
  }
}
