// Help for the rotary encoding comes from the following source:
// http://arduinotronics.blogspot.com/2013/09/using-rotary-encoder-with-arduino.html

// These two pins will tell us the meaning of the rotary encoder.
#define rotaryPinA 2
#define rotaryPinB 3

// The pattern of high and low when turning the encoder goes as follows:
// Clockwise (CW): A-high, B-high, A-low, B-low
// Counter-clockwise (CCW): B-high, A-high, B-low, A-low


volatile int ballSpeed = 3; // Speed of the ping-pong ball on screen
volatile unsigned int dialPos = 0; // Dial counter
unsigned int dialLastPos = 0; // Manages Change
static boolean rotating = 0; // Manages debouncing
boolean A_set = 0; // More stuff for debouncing
boolean B_set = 0;

// These are the boundaries for our speed.
const int MAXSPEED = 20;
const int MINSPEED = 0;
const int UNITSPEED = 1;

void setup() {
  // Sets the modes of our pins and pullup resistors for debouncing purposes.
  pinMode(rotaryPinA, INPUT_PULLUP);
  pinMode(rotaryPinB, INPUT_PULLUP);
  // Initialize the code to detect interrupts on both of the rotary pins.
  attachInterrupt(digitalPinToInterrupt(rotaryPinA), A_First, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryPinB), B_First, CHANGE);
  Serial.begin(9600);
}

void loop() {
  rotating = 1; // Reset the debouncer
  if (dialLastPos != dialPos)
  {
    dialLastPos = dialPos;
  }
  Serial.println(ballSpeed);
}


// Interrupt on a change in A

void A_First(){
  if (rotating) delay(1); // A bit of debouncing
  if(digitalRead(rotaryPinA) != A_set) // Did the value on the pin actually change, or was it a fluke?
  {
    A_set = !(A_set); 
    // If not a fluke, and if A is leading B (clockwise rotation),
    // iterate ballSpeed up one within its limits.
    if (A_set && !(B_set))
    {
        if(ballSpeed < MAXSPEED)
        {
          ballSpeed += UNITSPEED;
        }
    }
    rotating = 0;
  }
}

// Interrupt on a change in B
// Essentially, do the same thing as A, but iterate down instead of up,
// as the rotary would be turning in the opposite direction

void B_First(){
  if (rotating) delay(1); 
  if(digitalRead(rotaryPinB) != B_set)
  {
    B_set = !(B_set); 
    // If not a fluke, and if A is leading B (clockwise rotation),
    // iterate ballSpeed down within its limits.
    if (B_set && !(A_set))
    {
        if(ballSpeed > MINSPEED)
        {
          ballSpeed -= UNITSPEED;
        }
    }
    rotating = 0;
  }
}
