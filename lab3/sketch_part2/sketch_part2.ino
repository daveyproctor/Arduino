#include <avr/sleep.h>
#include <TimerOne.h>

int i = 0;
bool x = true;
void setup()
{
  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  Serial.begin(9600);
}
void loop()
{
  sleep_enable();
  sleep_mode();
  // delay(100000);
//  if (x)
//  {
//    Serial.println("HI");
//    x = false;
//  }
}

// Interrupt is called once a millisecond, 
ISR(TIMER0_COMPA_vect) 
{
  sleep_disable();
  if (i == 1000)
  {
    // x = true; 
    Serial.println("HI");
    i = 0;
  }
  i++;
}




//
//#include <avr/sleep.h>
//#define ledPin 13
// 
///* Timer2 reload value, globally available */
//volatile unsigned int tcnt2;
//volatile long n = 0L;  
// 
///* Setup phase: configure and enable timer2 overflow interrupt */
//void setup() {
//  
//  /* Configure the test pin as output */
//  pinMode(ledPin, OUTPUT); 
// 
//   /* First disable the timer overflow interrupt while we're configuring */   
//   TIMSK2 = 0x00;
//  
//   
//  /* Configure timer2 in normal mode (pure increment counting, no PWM etc.) */
//  TCCR2A = 0x00;
//   
//   
//  /* Select clock source: internal I/O clock (SCHAUEN, OB ES LANGSAMEREN CLK GIBT --> ES GIBT NUR EXTERNEN COUNTER ZUSÄTZLICH) */
//  ASSR = 0x20;  
//  
//   
//  /* Now configure the prescaler to CPU clock divided by 1024 */
//  // Prescaler auf 1024
//  TCCR2B = 0x07;  
//  
//  
//  /* Save value globally for later reload in ISR */
//  tcnt2 = 0x01; 
// 
//  /* Finally load end enable the timer */
//  TCNT2 = tcnt2;
//  
//  //Interrupt im Statusregister SREG freigeben
//  SREG = 0x80;
//
//  /* Disable Compare Match A interrupt enable (only want overflow) */
//  TIMSK2 = 0x01;
//
//  // Enable Interrupt in TIFR2-Register
//  TIFR2 = 0x01;
//
//  // activate power-save mode (auf Nomenklatur achten)
//  SMCR = 0x07;
//
//  // SLEEP instruction 
//  sleep_enable();
//  sleep_mode();
//  
//}
//
///*  void measurement () {
//    Serial.println("starte messung");
//    // during measurment IR has to be disabled
//} */
// 
//
// ISR(TIMER2_OVF_vect) {   
//    // clear sleep enable
//    sleep_disable();
//        
//       
//    if (n == 937500L){
//     // measurement();
//      for (int i = 0; i < 3000; i++) {
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);
//      digitalWrite(ledPin,HIGH);         
//       } 
//      n = 0;
//      
//    }   
//  
//  n = n + 1;
//  
// } 
//
//void loop() {
//  
//  // SLEEP instruction 
//  sleep_enable();
//  sleep_mode();
//
//  digitalWrite(ledPin,LOW); 
// 
//}
