#include "concurrency.h"
// int 

void p1 (void)
{
  /* process 1 here */
  while(1){
    Serial.println("hi from p1");
    serialEventRun();
  }
  return;
}

void p2 (void)
{
/* process 2 here */
  while(1){
    Serial.println("hi from p2");
    serialEventRun();
  }
 return;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("hi");
  if (process_create (p1, 64) < 0) {
    return;
  }
  if (process_create (p2, 64) < 0) {
    return;
  }
}

void loop()
{
  process_start();
  Serial.println("oops");
  /* if you get here, then all processes are either finished or
     there is deadlock */
  while (1) ;
}



// void setup(){
//   Serial.begin(9600);
// }

// void loop(){
//   Serial.println("hi");
// }
