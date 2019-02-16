#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int testasm(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_assembly

   .data
   ; any data values/static storage can go here

   .text

   ; Your assembly language program goes here
   ;
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;

start_of_assembly:
   ; Input a is %1 and r26
   mov r26,%1 
   ; Input b is %2 and r27
   mov r27,%2 

   ; Stack description:
   ; r26 and r27 saved on stack "caller-preserved" just in case (not actually used)
   ; r14 stores the value of sumval
   ; nothing else on the stack.

   push r26
   push r27
   ; Call sum on the inputs
   call sumval
   pop r27
   pop r26
   movw r14,r24
   push r14
   ; take absolute value of difference on same inputs
   call diffval
   ; add the results of sumval and diffval together
   pop r14
   adc r24,r14
   ;mov r25,r1
   ;rol r25
   jmp results
      
sumval:
    ; Copy "a" to a new register 
    mov r24,r26 
    ; Clear r15, just in case
    mov r15,r1 
    ; Add "a" and "b"
    adc r24,r27
    ; put the carry bit in r25
    mov r25,r1
    rol r25 
    ; Return to the main function
    ret 

diffval: 
    ; See whether a or b is greater
   cp r27,r26
   ; If b is smaller, jump ahead
   ; Below is the subtraction protocol for if a is the smaller value
   ; Move b to r24, and then subtract a from it.
   brlt smallb
   mov r24,r27
   sub r24,r26
   jmp return1

   ; This is the same protocol as above, but for if b is the smaller value
   ; Move a to r24, and then subtract b from it.
   smallb: 
   mov r24,r26
   sub r24,r27
   ; Exit the subfunction
   return1: ret

;Display results
results: mov %0,r24
   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}
