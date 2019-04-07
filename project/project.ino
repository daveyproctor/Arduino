#include <math.h>
#include <string>

#define A_PIN 13
#define A_SHARP_PIN 12
#define F_PIN 8
#define C_PIN 7
/*
    A tone is considered within a note family but out of tune
    if the note is with 50 cents sharp or flat of the desires pitch.
    The difference n in cents from note a to note b is defined as follows:
    n = 1200*log2(b/a)

    No tuning is going to be perfect,
    so we define an acceptable range of in-tune frequencies
    to be within 1 cent of the center pitch.
    
    We also define what ranges of pitch to detect as "within a family"
    as being 50 cents away from the center pitch,
    so that users will have a way to 
*/

struct pitchFamily
{
    int pitchTag;
    short centerPitch;
    short topPitch = centerPitch*(2^(1/24));
    short bottomPitch = centerPitch*(2^(-1/24));
    short goodEnoughTop = centerPitch*(2^(1/1200));
    short goodEnoughBottom = centerPitch*(2^(-1/1200));
    string serialLabel; // What to print into the Serial Monitor.
};

/*
    Our tuning frequencies are defined according to the table at this URL:
    https://pages.mtu.edu/~suits/notefreqs.html
    We define the base A note as 440Hz
*/

pitchFamily A;
  A.pitchTag = 1;
  A.centerPitch = 440;
  A.serialLabel = "A ";
pitchFamily A_SHARP; 
  A_SHARP.pitchTag = 2;
  A_SHARP.centerPitch = 466.16;
  A_SHARP.serialLabel = "A# ";
pitchFamily B; 
  B.pitchTag = 3;
  B.centerPitch = 493.88;
  B.serialLabel = "B ";
pitchFamily C;
  C.pitchTag = 4;
  C.centerPitch = 523.25;
  C.serialLabel = "C ";
pitchFamily C_SHARP;
  C_SHARP.pitchTag = 5;
  C_SHARP.centerPitch = 554.37;
  C_SHARP.serialLabel = "C# ";
pitchFamily D;
  D.pitchTag = 6;
  D.centerPitch = 587.33;
  D.serialLabel = "D ";
pitchFamily D_SHARP;
  D_SHARP.pitchTag = 7;
  D_SHARP.centerPitch = 622.25;
  D_SHARP.serialLabel = "D# ";
pitchFamily E;
  E.pitchTag = 8;
  E.centerPitch = 659.25;
  E.serialLabel = "E "
pitchFamily F;
  F.pitchTag = 9;
  F.centerPitch = 698.46;
  F.serialLabel = "F ";
pitchFamily F_SHARP;
  F_SHARP.pitchTag = 10;
  F_SHARP.centerPitch = 739.99;
  F_SHARP.serialLabel = "F# ";
pitchFamily G; 
  G.pitchTag = 11;
  G.centerPitch = 783.99;
  G.serialLabel = "G ";
pitchFamily G_SHARP;
  G_SHARP.pitchTag = 12;
  G_SHARP.centerPitch = 830.61;
  G_SHARP.serialLabel = "G# "


/* 
 * Orchestrally speaking,
 * tuning notes are typically A, A#, F, or C, 
 * so we probably only need buttons to play those four. 
 */

void play_a()
{
  tone(13, 440); 
}

void setup()
{
    attachInterupt(digitalPinToInterrupt(A_PIN),play_a,LOW);
  
}

void loop() {
  
  
}

            
//enum short TopTones{A_TOP = A*2^(1/24), A_SHARP_TOP}
