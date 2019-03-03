/* 
A method of implementing analogRead(A0) below is detailed 
in the README under the "analogRead() Implementation" section
*/

int i = 0;  // The variables i and j are counter variables used for printing out
int j = 0;  // the recorded brightness of the photoresistor every second.

void setup() {
  // put your setup code here, to run once:
  pinMode (A0, INPUT); // Set this pin to recieve the voltage from the divider.
  pinMode (3, OUTPUT); // Set this pin to output our analog value.
  Serial.begin(9600);  // Initialize the Serial Monitor.
}

void loop() {
  // Read in the voltage value from the divider.
  // Testing revealed to us that this value ranges from 0 to 1000 when read in by the Arduino.
  float val = analogRead(A0); 
  // Change the value we read in to one between 0 and 255.
  int val255 = val*255/1000; 
  // Write that value to the pin going into the RGB LED to control its brightness.
  analogWrite(3, val255);
  // Check how many milliseconds have passed since printing out the brightness value.  
  i = millis();             
  // If 1 second has passed since printing out the last brightness value, print it out again.  
  if ((i-j) >= 1000)       
  {
    Serial.println(val255);
    j = millis();
  }
}
