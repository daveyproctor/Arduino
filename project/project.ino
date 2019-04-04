#include <math.h>

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
    short centerPitch;
    short topPitch = centerPitch*(2^(1/24));
    short bottomPitch = centerPitch*(2^(-1/24));
    short goodEnoughTop = centerPitch*(2^(1/1200));
    short goodEnoughBottom = centerPitch*(2^(-1/1200));
};

/*
    Our tuning frequencies are defined according to the table at this URL:
    https://pages.mtu.edu/~suits/notefreqs.html
    We define the base A note as 440Hz
*/

pitchFamily A;  
  A.centerPitch = 440;
pitchFamily A_SHARP; 
  A_SHARP.centerPitch = 466.16;
pitchFamily B; 
  B.centerPitch = 493.88;
pitchFamily C; 
  C.centerPitch = 523.25;
pitchFamily C_SHARP; 
  C_SHARP.centerPitch = 554.37;
pitchFamily D; 
  D.centerPitch = 587.33;
pitchFamily D_SHARP; 
  D_SHARP.centerPitch = 622.25;
pitchFamily E; 
  E.centerPitch = 659.25;
pitchFamily F; 
  F.centerPitch = 698.46;
pitchFamily F_SHARP; 
  F_SHARP.centerPitch = 739.99;
pitchFamily G; 
  G.centerPitch = 783.99;
pitchFamily G_SHARP; 
  G_SHARP.centerPitch = 830.61;


/* 
 * Orchestrally speaking,
 * tuning notes are typically A, A#, F, or C, 
 * so we probably only need buttons to play those four. 
 */

void play_a()
{
  
}

void setup()
{
    attachInterupt(digitalPinToInterrupt(A_PIN),play_a,LOW);
  
}

void loop() {
  
  
}

            
//enum short TopTones{A_TOP = A*2^(1/24), A_SHARP_TOP}
