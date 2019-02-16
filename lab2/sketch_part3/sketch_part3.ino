// Initiate the pins for data transfer.
int clockPin = 11;
int latchPin = 12;
int dataPin = 13;
// We also need a pin with which to track interrupts. It'll be active LOW.
int interruptPin = 3;
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
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    //Also indicate the interrupt pin as an input.
    pinMode(interruptPin, INPUT_PULLUP);
    //Indicate to the microcontroller to switch to the function countDown when button is pressed and released.
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleButton, RISING);
    Serial.begin(9600);
}


// This function sends out the appropriate LED information to the registers.
void writeLED(char columnData, char rowData)
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, columnData);
    // byte leds2 = 1;
    shiftOut(dataPin, clockPin, LSBFIRST, rowData);
    digitalWrite(latchPin, HIGH);
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
  // Switch to the next number when the button is pressed.
  
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

  //Change occurs on the rising edge, so debouncing occurs here.
  
  for (int i = 0; i < 1500; i++)
  {
    writeNumber(state);
  }
  
  delay(3000);
  
}


// This function tells the microcontroller to display the current digit.
void loop()
{
  writeNumber(state);
}
