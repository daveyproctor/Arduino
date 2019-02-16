#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int testasm(unsigned char a) {
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

   ; Calling convention: r26 is input. r24 is output.
   ; a is %1 and r26
   mov r26,%1 
   ; r27 is 1, for comparison reasons.
   mov r27,r1
   ; increment r27 to make it equal to 1
   inc r27
   ; call fib for the first time
   call fib
   ; jump to returning the results when it's all over
   jmp results

    ; Fibonacci sub-funciton
    ; Implement the base cases first
fib: cp r26,r1
    ;
    ; Stack description: 
    ; We store on the stack r26, the value of n-1
    ; and r24, the n-1th Fibonacci number.
    ;
    breq zerocase
    cp r26,r27
    breq onecase
    ; These are the recursive cases: n-1 and n-2.
    ; First ncrement "n" down one, and push it to the stack
    sub r26,r27
    push r26
    ; Call fib recursively to get the n-1th term
    call fib
    ; Pop the value of n-1 out of the stack
    pop r26

    ; Increment n-1 down again to mark Fibonacci number-2
    sub r26,r27
    ; Pushing r24 to the stack saves the value of the n-1th Fibonacci number
    push r24
    ; Calling fib here gives us the n-2th term.
    call fib
    ; Popping the value from the stack to get the value of the n-1th Fibonacci number back for adding purposes
    pop r23
    ; Add n-1th and n-2th terms together to get the nth Fibonacci term
    add r24,r23
    ;Exit the recursive function
    ret

; If n=0, return 0
zerocase: mov r24,r1
ret

; If n=1, return 1
onecase: mov r24,r27
ret

;Display results
results: movw %0,r24
   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;

)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}
