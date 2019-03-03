Lab 3 by Davey Proctor and Michael McNamara
----------------------------------------------------------------------------------------
Relevant Datasheets, Documentation, and Other Information:

- AVR Datasheet (for Chapter 21 on ADC implementation and registers): 
       https://www.sparkfun.com/datasheets/Components/SMD/ATMega328.pdf
- Additional Information about how analogRead() and its associated registers work: 
       https://garretlab.web.fc2.com/en/arduino/inside/arduino/wiring_analog.c/analogRead.html
- Photoresistor: 
       http://csl.yale.edu/~rajit/classes/eeng348/files/photocells.pdf
- RGB LED: 
       http://csl.yale.edu/~rajit/classes/eeng348/files/RGB_LED.pdf
- Ultrasonic Distance Detector: 
       http://csl.yale.edu/~rajit/classes/eeng348/files/HCSR04.pdf
- Rotary Encoder: 
       http://csl.yale.edu/~rajit/classes/eeng348/files/TW-700198.pdf
- Adafruit SSD1306 and GFX libraries utilized for Part 3: 
       https://cdn-learn.adafruit.com/downloads/pdf/adafruit-gfx-graphics-library.pdf
- Help with implementing the Rotary Encoder in Arduino:
       
-----------------------------------------------------------------------------------------
analogRead(A0) Implementation:
  Chapter 21 of the AVR and the garretlab webpage above detail the guts of the analogRead() function: 
  the AVR's ADC and the registers which control it and which it controls.
  This ADC converts voltages to 10-bit values, and as such needs two registers to store the value: ADCH and ADCL.
  Analog input is selected via a MUX register, ADMUX, and it even has its own status register, ADCSRA.
  
  The ADMUX Register has 4 select bits, but only 3 of them matter for the ADR's 3-bit MUX, and values 6 & 7 are disabled.
  ADMUX Also controls the ADC's reference voltage with its top 2, which by default is 5 volts and is selected by the code 01.
  The presentation of the data is also controlled by the ADMUX with the third highest bit, but the Arduino sets this to 0 automatically.
  
  To enable the conversion to happen, we need to set the ADSC bit, the 2nd highest bit in the ADCSRA register.
  It will clear itself when the conversion is done. There are other important bits in this register, 
  but they are either handled for us or have no use in the Arduino software. 
  
  With all this in mind, an implementation of analogRead(A0) -- reading in from the
  0th analog port -- for our purposes in Part 1 could look like this:
  
  // Set ADMUX to select the 0th port and have the default reference voltage
  ADMUX = ((01 << 6) | (0x00));
  // Set ADSC
  sbi(ADCSRA, 6);
  // Wait until the conversion is done
  while ((ADCSRA) & (1 << 6));
  // Return the value as a combination of the ADCL and ADCH.
  // ADCL needs to be read first, otherwise it will get overwritten.
  byte valLower = ADCL;
  byte valHigher = ADCH;
  // Combine ADCL and ADCH into an int.
  int val = (valHigher << 8) | valLower;
  
