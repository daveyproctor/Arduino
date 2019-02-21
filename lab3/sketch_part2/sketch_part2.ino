// int i = 0;  // The variables i and j are counter variables used for printing out
// int j = 0;  // the recorded brightness of the photoresistor every second.

//Setting the color-selection pins.
int redPin = 10;
int bluePin = 9;
int greenPin = 8;


void setup() {
  // put your setup code here, to run once:
  pinMode (A0, INPUT); // Set this pin to recieve the voltage from the divider.
  pinMode (11, INPUT); // This pin receives the pulse width from the ultrasonic sensor.
  pinMode (13, OUTPUT); // This pin triggers the ultrasonic sensor.
  // The following three pins are for setting the color of the RGB LED.
  pinMode (greenPin, OUTPUT);
  pinMode (redPin, OUTPUT);
  pinMode (bluePin, OUTPUT); 
  
  pinMode (3, OUTPUT); // Set this pin to output our analog value.
  Serial.begin(9600);  // Initialize the Serial Monitor.
}

void loop() {
  // Trigger the pulse to turn on the ultrasonic sensor.
  digitalWrite(13, HIGH);
  delayMicroseconds(10);
  digitalWrite(13, LOW);
  // Read in the pulse it generates and the brightness value of the photocell in volts.
  float distance = pulseIn(11, HIGH); 
  float val = analogRead(A0);
  /*
   * Range of distances it can cover is 2cm to 4m,
   * and the conversion chart it gave us was that 58uS of pulse width
   * is 1cm of length. 400*58 = 23,200uS maximum, 2*58 = 116uS minimum.
   * 
   * From this, we can determine the range of conversion factor
   * and offset needed to convert the value into a 0-255 integer range.
   */
  int dist255 = (distance - 118)*(255/23200);
  // Change the value for we read in from one between 0 and 1000 to one between 0 and 255.
  int val255 = val*255/1000; 
  // Write the brightness value to the pin going into the top of the RGB LED to control its brightness.
  analogWrite(3, val255);
  // Select the color of the pin based on the value of the 255 number.
  // They're active LOW, with green being close, red far, and blue medium.
  if(dist255 <= 85)
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, LOW);    
  }
  else if ((dist255 > 85) && (dist255 <= 170))
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);    
  }
  else if (dist255 > 170)
  {
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);    
  }
  
  Serial.println(distance);
  delay(1000);

  /*
  // Change the value we read in to one between 0 and 255.
  int val255 = val*255/1000; 
  // Write that value to the pin going into the RGB LED to control its brightness.
  analogWrite(3, val255);
  // Check how many milliseconds have passed since printing out the brightness value.  
  i = millis();             
  // If 1 second has passed since printing out the last brightness value, print it out again.  
  /*if ((i-j) >= 1000)       
  {
    Serial.println(val255);
    j = millis();
  }
  */
}
