#include <math.h>
#include <string.h>
#include <arduinoFFT.h>
//#include <string>

// Labelling pins

#define MIC_PIN A0
#define MIC_TRIGGER 10
#define SPEAKER_PIN 11
#define A_PIN 6
#define A_SHARP_PIN 5
#define F_PIN 8
#define C_PIN 7
#define RED_PIN 4
#define GREEN_PIN 3
#define BLUE_PIN 2

// Define how lenient this tuner will be

#define CENTS_RANGE 3

// Setting up Fast Fourier Transform Parameters

#define SAMPLES 128 // Must be a power of 2
#define SAMPLING_FREQ 3400 // Must be less than 1000 due to ADC limitations

unsigned int sampling_period_us;
unsigned long microseconds;

double waveReal[SAMPLES];
double waveImag[SAMPLES];
double peak;
double peak_last = 0;
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
    to be within 1 cent of the center pitch.
    
    We also define what ranges of pitch to detect as "within a family"
    as being 50 cents away from the center pitch.
*/

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

double twelveToneScale[12];
String twelveToneNames[12] = {"A ", "A# ", "B ", "C ", "C# ", "D ", "D# ", "E ", "F ", "F# ", "G ", "G# "};
/*
twelveToneNames[0] = String("A ");
twelveToneNames[1] = String("A# ");
twelveToneNames[2] = String("B ");
twelveToneNames[3] = String("C ");
twelveToneNames[4] = String("C# "); 
twelveToneNames[5] = String("D ");
twelveToneNames[6] = String("D# ");
twelveToneNames[7] = String("E ");
twelveToneNames[8] = String("F ");
twelveToneNames[9] = String("F# ");
twelveToneNames[10] = String("G ");
twelveToneNames[11] = String("G# ");
/*
typedef struct pitchFamily pitchFamilyT;
struct pitchFamily
{
    int pitchTag;
    double centerPitch;
    //string serialLabel; // What to print into the Serial Monitor.
};
*/

double topPitch(double, int);
double goodEnoughTop(double, int);
double bottomPitch(double, int);
double goodEnoughBottom(double,int);
double centerPitch(double, int);

/* 
 * Orchestrally speaking,
 * tuning notes are typically A, A#, F, or C, 
 * so we probably only need buttons to play those four. 
 */

void play_a()
{
  a_trig = 1;
  //Serial.println("Interrupt A");
  delay(200);
}

void play_asharp()
{
  asharp_trig = 1;
  delay(200);
}

void play_c()
{
  c_trig = 1;
  delay(200);
}

void play_f()
{
  f_trig = 1;
  delay(200);
}

double centerPitch(double note, int octave)
{
  return note*pow(2,(float)octave);
}

double topPitch(double note, int octave){
  return note*pow(2,((float)1/24) + (float)octave);
}

double goodEnoughTop(double note, int octave){
  return note*pow(2,((float)CENTS_RANGE/1200) + (float)octave);
}

double bottomPitch(double note, int octave){
  return note*pow(2,(((float)-1/24) + (float)octave));
}

double goodEnoughBottom(double note, int octave){
  return note*pow(2,(((float)-CENTS_RANGE/1200) + (float)octave));
}

void setup()
{
for(int i=0;i<12;i++)
{
    twelveToneScale[i] = 440*pow(2,((float)i/12));
}

  Serial.begin(9600);

  // Define inputs...
  pinMode(MIC_PIN, INPUT);
  pinMode(MIC_TRIGGER, INPUT);
  pinMode(A_PIN, INPUT);
  pinMode(A_SHARP_PIN, INPUT_PULLUP);
  pinMode(C_PIN, INPUT_PULLUP);
  pinMode(F_PIN, INPUT_PULLUP);
  // ...and outputs.
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  //Interrupt setup.
  attachInterrupt(digitalPinToInterrupt(A_SHARP_PIN),play_asharp,FALLING);
  attachInterrupt(digitalPinToInterrupt(A_PIN),play_a,FALLING);
  attachInterrupt(digitalPinToInterrupt(F_PIN),play_f,FALLING);
  attachInterrupt(digitalPinToInterrupt(C_PIN),play_c,FALLING);

  //FFT sampling period setup.
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQ));
}

void loop() {

/*
  if(digitalRead(A_PIN) == LOW)
  {
    Serial.println("Intterupt A");
  }
*/

  // Reference Pitches

  // Play A
  
  while((digitalRead(A_PIN) == LOW) && (asharp_trig = 1)){
  //Serial.println("LOW");
  //digitalWrite(12, HIGH);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
    tone(SPEAKER_PIN, twelveToneScale[0]); //
  } 
  a_trig = 0;

  //Play A#
  while((digitalRead(A_SHARP_PIN) == LOW) && (asharp_trig = 1)){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
    tone(SPEAKER_PIN, twelveToneScale[1]);
  }
  asharp_trig = 0; 

  //Play C
  while((digitalRead(C_PIN) == LOW) && (c_trig = 1)){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
    tone(SPEAKER_PIN, twelveToneScale[3]);
  } 
  c_trig = 0;

  //Play F
  while((digitalRead(C_PIN) == LOW) && (f_trig = 1)){
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  tone(SPEAKER_PIN, twelveToneScale[8]);
  } 
  f_trig = 0;
    
  
  //Reading in a wave to tune.
  
  while((digitalRead(MIC_TRIGGER) == 1) && (f_trig == 0) && (c_trig == 0) && (asharp_trig == 0) && (a_trig) == 0)
  {
    tone(SPEAKER_PIN, 698.46);
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
      peak = peak - peak*29/440; // Correction factor
      //peak = 400;
      /*
      Serial.println(peak);
      Serial.println(topPitch(peak, 0));
      Serial.println(bottomPitch(peak, 0));
      */
      
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

                if(peak != peak_last)
                {
                Serial.print(twelveToneNames[j]);
                //Serial.println(1200*log(peak/centerPitch(twelveToneScale[j], octave))/log(2));
                Serial.println(peak);
                }
                
              }
              
          }
      }
      
      //Serial.println(peak);
      //delay(20);

  peak_last = peak;
  }
  
  // Write the LED pins to low once we're done playing a note.


  //Serial.print(twelveToneNames[j]);
  

  //delay(500);

}
