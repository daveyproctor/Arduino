int dataPin2 = 10;
int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
byte leds = 0;
int currentLED = 0;

void setup()
{
    pinMode(dataPin2, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    leds = 0;
    Serial.begin(9600);
}

void loop()
{
    if (leds == 7)
    {
        leds = 0;
    }
    else
    {
        leds++;
    }
    Serial.println(leds);
    
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, leds);
    byte leds2 = 1;
    shiftOut(dataPin2, clockPin, LSBFIRST, leds2);
    digitalWrite(latchPin, HIGH);

    delay(1000);
}
