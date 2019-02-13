// int dataPin2 = 10;
// Set the pin for the latch, the clock pin for the storage registers in the 74HC595s
int latchPin = 12;
// Set the pin for the clock for the shift registers in the 74HC595s
int clockPin = 11;
// Set this as the serial data going into the two registers, which goes into pin 14 of 74HC595 1
int dataPin = 13;
// Set up one data value for the rows and another for the columns.
byte rowData = 0x00;
byte columnData = 0x00;

// byte data = 0;
// int currentLED = 0;

void setup()
{
    // pinMode(dataPin2, OUTPUT);
    
    // Label each of the 3 pins as an output
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    
    // Initialize one data value for the rows and another for the columns.
    rowData = 0x00;
    columnData = 0x00;
    
    
    // byte leds = 0;
    // Start the serial
    Serial.begin(9600);
}

void loop()
{
    /* 
       I think the way that this function should work is that one register
       should hold the data for the 7 rows of the LED board -- which are active high
       -- and the second should hold the data for the 5 columns of the LED board --
       which are active low. On every latch cycle, the second shift register will turn on one row
       with a data of value 11111110, 11111101, 11111011, 11110111, or 11101111, and the first register
       will hold the data for which lights in the active row need to be turned on, byte values which are indicated
       by the hexadecimal numbers in the font list included with our lab.
       
       We'd nest each number in a for loop, which would give the Arduino the necessary time
       to properly display a for loop. Something like the below loop, but a bit more refined.
     */
    
    // This is a rough idea of how I think we could get the Arduino to display a 0.
    // Of course, it needs refining, as I don't think the data changing would occur in the right place.
    int i;
    for(i = 0, i <= 200, i++)
    {
        columnData = 0xFE; // First Column
        rowData = 0x3E;
        columnData = 0xFD; // Second Column
        rowData = 0x51;
        columnData = 0xFB; // Third Column
        rowData = 0x49;
        columnData = 0xF7; // Fourth Column
        rowData = 0x45;
        columnData = 0xEF; // Fifth Column
        rowData = 0x3E;
    } 
    
        
    
    
    /*
    if (leds == 7)
    {
        leds = 0;
    }
    else
    {
        leds++;
    }
    Serial.println(leds);
    */
    
    // latchPin is held at 0 for as long as we want to transmit information
    
    /* 
       Working with the two registers in cascade, we send the data
       intended for the second register first, and then the stuff
       for the first register second. That way, when the data intended for the first
       register gets sent, the stuff intended for the second one will get shifted to its
       proper register.
    */
    
    
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, leds);
    // byte leds2 = 1;
    shiftOut(dataPin2, clockPin, LSBFIRST, leds2);
    digitalWrite(latchPin, HIGH);

    delay(1000);
}
