int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
byte leds = 0;
int currentLED = 0;

void setup()
{
    Serial.begin(9600);
    Serial.println("HI");
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    leds = 0;
}

void loop()
{
    
    // leds = 0;

    if (leds == 7)
    {
        leds = 0;
    }
    else
    {
        leds++;
    }

    // bitSet(leds, currentLED);

    Serial.println(leds);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, leds);
    digitalWrite(latchPin, HIGH);

    delay(250);
}
