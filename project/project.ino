#include <math.h>
#include <string.h>
#include <arduinoFFT.h>
//#include <string>

// Labelling pins

#define MIC_PIN A0
#define MIC_TRIGGER 10
#define SPEAKER_PIN 11
#define A_PIN 5
#define A_SHARP_PIN 6
#define F_PIN 8
#define C_PIN 7
#define RED_PIN 4
#define GREEN_PIN 3
#define BLUE_PIN 2

// Define how lenient this tuner will be

#define CENTS_RANGE 4

// Setting up Fast Fourier Transform Parameters

#define SAMPLES 128 // Must be a power of 2
#define SAMPLING_FREQ 4000 // Must be less than 10000 due to ADC limitations

unsigned int sampling_period_us;
unsigned long microseconds;

double waveReal[SAMPLES];
double waveImag[SAMPLES];
float peak;
float peak_last = 0;
int octave;

boolean a_trig = 0;
boolean asharp_trig = 0;
boolean c_trig = 0;
boolean f_trig = 0;

arduinoFFT FFT = arduinoFFT();

/*
    A tone is considered within a note family but out of tune
    if the note is with 50 cents sharp or flat of the desires pitch.
    The difference n in cents from note a to note b is defined as follows:
    n = 1200*log2(b/a). Each tone is 100 cents above the next, i.e.
    one note has a frequency of 2^(1/12) the note below it.
        
    No tuning is going to be perfect,
    so we define an acceptable range of in-tune frequencies
    to be within 4 cents of the center pitch.
    
    We also define what ranges of pitch to detect as "within a family"
    as being 50 cents away from the center pitch.
*/

/*
  Initialize arrays for the in-tone frequencies
  and the names associated with those notes.
*/

double twelveToneScale[12];
String twelveToneNames[12] = {"A ", "A# ", "B ", "C ", "C# ", "D ", "D# ", "E ", "F ", "F# ", "G ", "G# "};

/*
  These functions give us the pitch ranges
  necessary for classificaiton and checking intonation.
*/

float topPitch(float, int);
float goodEnoughTop(float, int);
float bottomPitch(float, int);
float goodEnoughBottom(float,int);
float centerPitch(float, int);

float centerPitch(float note, int octave)
{
  return note*pow(2,(float)octave);
}

float topPitch(float note, int octave){
  return note*pow(2,((float)1/24) + (float)octave);
}

float goodEnoughTop(float note, int octave){
  return note*pow(2,((float)CENTS_RANGE/1200) + (float)octave);
}

float bottomPitch(float note, int octave){
  return note*pow(2,(((float)-1/24) + (float)octave));
}

float goodEnoughBottom(float note, int octave){
  return note*pow(2,(((float)-CENTS_RANGE/1200) + (float)octave));
}

void setup()
{
  // Define the center pitches.

  /*
  Our base tuning frequencies are defined according to the table at this URL:
  https://pages.mtu.edu/~suits/notefreqs.html
  
  Pitch definitions in terms of Hertz: A to G#
  More precisely, each pitch is a factor of 2^(1/12)
  above the lower pitch.
  A: 440   A#: 466.16   B: 493.88  C: 523.25
  C#: 554.37  D: 587.33  D#: 622.25  E: 659.25
  F: 698.46  F#: 739.99  G: 783.99  G#: 830.61
  */
  for(int i=0;i<12;i++)
  {
      twelveToneScale[i] = 440*pow(2,((float)i/12));
  }
  

  Serial.begin(9600);

  // Define inputs...
  pinMode(MIC_PIN, INPUT);
  pinMode(MIC_TRIGGER, INPUT);
  pinMode(A_PIN, INPUT_PULLUP);
  pinMode(A_SHARP_PIN, INPUT_PULLUP);
  pinMode(C_PIN, INPUT_PULLUP);
  pinMode(F_PIN, INPUT_PULLUP);
  // ...and outputs.
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  

  //FFT sampling period setup.
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQ));
}

void loop() {
  // Toggling Reference Pitches.
  // Done using a polling method of gauging inputs.

  // Play A
  
  while((digitalRead(A_PIN) == LOW)){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
    tone(SPEAKER_PIN, twelveToneScale[0]); //
  } 
  if(digitalRead(A_PIN == HIGH)){
  noTone(SPEAKER_PIN);
  }

  //Play A#
  while(digitalRead(A_SHARP_PIN) == LOW){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
    tone(SPEAKER_PIN, twelveToneScale[1]);
  }
  if(digitalRead(A_SHARP_PIN == HIGH)){
  noTone(SPEAKER_PIN);
  } 

  //Play C
  while(digitalRead(C_PIN) == LOW){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
    tone(SPEAKER_PIN, twelveToneScale[3]);
  } 
  if(digitalRead(A_SHARP_PIN == HIGH)){
  noTone(SPEAKER_PIN);
  }

  //Play F
  while(digitalRead(F_PIN) == LOW){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  tone(SPEAKER_PIN, twelveToneScale[8]);
  } 
  if(digitalRead(A_SHARP_PIN == HIGH)){
  noTone(SPEAKER_PIN);
  }
    
  
  //Reading in a wave to tune.
  /*
    A guide for our FFT implementation can be found here: 
    https://www.norwegiancreations.com/2017/08/what-is-fft-and-how-can-you-implement-it-on-an-arduino/
  */
  
  while((digitalRead(MIC_TRIGGER) == 1) && (digitalRead(A_PIN) == HIGH) && (digitalRead(A_SHARP_PIN) == HIGH) && (digitalRead(C_PIN) == HIGH) && (digitalRead(F_PIN) == HIGH)) // && (f_trig == 0) && (c_trig == 0) && (asharp_trig == 0) && (a_trig == 0))
  { 
      for(int i=0; i<SAMPLES; i++)
      {
          microseconds = micros();    //Overflows after around 70 minutes!

          waveReal[i] = analogRead(MIC_PIN);
          waveImag[i] = 0;

          while(micros() < (microseconds + sampling_period_us)){
          }
      }
    
  
      // Get the most prominent frequency: that's what allows us to tune.
      FFT.Compute(waveReal, waveImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(waveReal, waveImag, SAMPLES);
      peak = FFT.MajorPeak(waveReal, SAMPLES, SAMPLING_FREQ);
      // The limited memory of the Arduino means there will be some error in the FFT result.
      // Corrections, determined through trial and error, need to be made.
      peak = peak*pow(2,((float)1/24)) - pow(peak,((float)161/100))*((double)1/880);

      
      // Compare to see where the tone is relative to our core frequencies.
      
      for(octave = -3; octave <= 1; octave++)
      {      
          for(int j=0;j<12;j++)
          {
              
              if((peak <= topPitch(twelveToneScale[j], octave)) && (peak >= bottomPitch(twelveToneScale[j], octave)))
              {

              //Serial.println(goodEnoughBottom(twelveToneScale[j], octave));
              //Serial.println(goodEnoughTop(twelveToneScale[j], octave));
              
                // In tune
                
                if((peak >= goodEnoughBottom(twelveToneScale[j], octave)) && (peak <= goodEnoughTop(twelveToneScale[j], octave))) 
                {
                  Serial.println("In tune");
                  digitalWrite(RED_PIN, LOW);
                  digitalWrite(GREEN_PIN, HIGH);
                  digitalWrite(BLUE_PIN, LOW);
                }
                
                // Too low    
                else if(peak <= goodEnoughBottom(twelveToneScale[j], octave)) 
                {
                  digitalWrite(RED_PIN, HIGH);
                  digitalWrite(GREEN_PIN, LOW);
                  digitalWrite(BLUE_PIN, LOW);
                }

                // Too high
                else
                {
                  digitalWrite(RED_PIN, LOW);
                  digitalWrite(GREEN_PIN, LOW);
                  digitalWrite(BLUE_PIN, HIGH);
                }

                if(peak != peak_last) // If the dominant frequency changes...
                {
                Serial.print(twelveToneNames[j]);
                // This prints cents from center
                Serial.println(1200*log(peak/centerPitch(twelveToneScale[j], octave))/log(2)); 
                // This prints pitch frequency
                //Serial.println(peak); 
                }
                
              }
              
          }
      }
      
  peak_last = peak;
  //delay(200);
  }
}
