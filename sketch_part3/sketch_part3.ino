// Though we don't need to use the pinMode or digitalWrite functions,
// indicating which pins are clock and data is necessary for the shiftOut function
// that we need to communicate with the registers.
int clockPin = 11;
int dataPin = 13;
// We also need a pin with which to track interrupts. It'll be active LOW.
int interruptPin = 2;
int state = 0;

// Below is how we structure the data to send to the shift register
// A number is a list of row values needed to display the digit
// accompanied by the value necessary to turn on each column
// according to the principles of persistence of vision.
struct LCDNumber
{
  byte rowVal[5];
};

byte colList[5] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF};

struct LCDNumber numbers[10] = {
  {.rowVal = {0x3E, 0x51, 0x49, 0x45, 0x3E}}, // 0
  {.rowVal = {0x00, 0x42, 0x7F, 0x40, 0x00}}, // 1
  {.rowVal = {0x42, 0x61, 0x51, 0x49, 0x46}}, // 2
  {.rowVal = {0x21, 0x41, 0x45, 0x4B, 0x31}}, // 3
  {.rowVal = {0x18, 0x14, 0x12, 0x7F, 0x10}}, // 4
  {.rowVal = {0x27, 0x45, 0x45, 0x45, 0x39}}, // 5
  {.rowVal = {0x3C, 0x4A, 0x49, 0x49, 0x30}}, // 6
  {.rowVal = {0x01, 0x71, 0x09, 0x05, 0x03}}, // 7
  {.rowVal = {0x36, 0x49, 0x49, 0x49, 0x36}}, // 8
  {.rowVal = {0x06, 0x49, 0x49, 0x29, 0x1E}}  // 9
  };


void setup()
{    
    // Indicate every port as an output and set the default latch value to HIGH
    DDRB = B11111111;
    PORTB = B010000;
    pinMode(interruptPin, INPUT);
    //Indicate to the microcontroller to switch to the function countDown when button is pressed.
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleButton, FALLING);
    Serial.begin(9600);
}


// This function sends out the appropriate LED information to the registers.
void writeLED(char columnData, char rowData)
{
    PORTB = PORTB & B101111;
    shiftOut(dataPin, clockPin, LSBFIRST, columnData);
    // byte leds2 = 1;
    shiftOut(dataPin, clockPin, LSBFIRST, rowData);
    PORTB = PORTB | B010000;
}

// This function is looping through the 5 row values and column values needed
// to display each digit on the LED array via persistence of vision.
void writeNumber(int n)
{
  for (int i = 0; i < 5; i++)
  {
    char colVal = colList[i];
    char rowVal = numbers[n].rowVal[i];
    writeLED(colVal, rowVal);
  }
}

void handleButton()
{
//  Serial.print("Received interrupt. State: ");
//  Serial.print(state);
//  Serial.print("; PinVal:");
//  Serial.println(digitalRead(interruptPin));

  // debounce
  for (int i = 0; i < 100; i++)
  {
    if (digitalRead(interruptPin) == 1)
    {
      Serial.println("Debounced!");
      return;
    }
  }
  
  if (state == 9)
  {
    state = 0;
  }
  else
  {
    //Serial.println("  -- Changed state to: ");
    state++;
    //Serial.print(state);
  }
  for (int i = 0; i < 1000; i++)
  {
    writeNumber(state);
  }
  digitalWrite(interruptPin, LOW);
}


// This function tells the microcontroller to count up from digits 0 to 9, wrapping around when finished.
void loop()
{
  writeNumber(state);
}