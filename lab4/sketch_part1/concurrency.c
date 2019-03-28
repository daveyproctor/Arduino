#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "concurrency.h"

/*
 * my stuff
 */
process_t *current_process = NULL;

struct process_state {
	unsigned int sp; /* stack pointer */
	struct process_state *next; /* link to next process */
};

int process_create (void (*f) (void), int n) {
	struct process_state *p_state = malloc(sizeof(struct process_state));
	p_state->sp = process_init(f, n);
	p_state->next = current_process;
	current_process = p_state;
}

void process_start (void){
	process_begin();
}
unsigned int process_select (unsigned int cursp) {
	if (cursp == 0){
		return 0;
	}
	current_process = current_process->next;
	if (!current_process){
		return 0;
	}
	return current_process->sp;
}



__attribute__((used)) unsigned char _orig_sp_hi, _orig_sp_lo;

/*
 * Disable interrupts
 * Save current stack pointer
 * Jump to .dead_proc_entry which pops stack
 */
__attribute__((used)) void process_begin ()
{
  asm volatile (
		"cli \n\t"
		"in r24,__SP_L__ \n\t"     // r24 <- __SP_L__
		"sts _orig_sp_lo, r24\n\t" // _orig_sp_lo <- r24
		"in r25,__SP_H__ \n\t"     // r25 <- __SP_H__
		"sts _orig_sp_hi, r25\n\t" // _orig_sp_hi <- r25 
		"ldi r24, 0\n\t"           // r24 <- 0
		"ldi r25, 0\n\t"					 // r25 <- 0
		"rjmp .dead_proc_entry\n\t"
		);
}

__attribute__((used)) void process_terminated ()
{
  asm volatile (
		"cli\n\t"
		// load in original stack pointer
		"lds r25, _orig_sp_hi\n\t" // r25 <- _orig_sp_hi
		"out __SP_H__, r25\n\t"
		"lds r24, _orig_sp_lo\n\t"
		"out __SP_L__, r24\n\t"
		"ldi r24, lo8(0)\n\t"
		"ldi r25, hi8(0)\n\t"
		"rjmp .dead_proc_entry"
		);
}

void process_timer_interrupt();

__attribute__((used)) void yield ()
{
  if (!current_process) return;
  asm volatile ("cli\n\t");
  asm volatile ("rjmp process_timer_interrupt\n\t");
}

__attribute__((used)) void process_timer_interrupt()
{
  asm volatile (
		"push r31\n\t"
		"push r30\n\t"
		"push r29\n\t"
		"push r28\n\t"
		"push r27\n\t"
		"push r26\n\t"
		"push r25\n\t"
		"push r24\n\t"
		"push r23\n\t"
		"push r22\n\t"
		"push r21\n\t"
		"push r20\n\t"
		"push r19\n\t"
		"push r18\n\t"
		"push r17\n\t"
		"push r16\n\t"
		"push r15\n\t"
		"push r14\n\t"
		"push r13\n\t"
		"push r12\n\t"
		"push r11\n\t"
		"push r10\n\t"
		"push r9\n\t"
		"push r8\n\t"
		"push r7\n\t"
		"push r6\n\t"
		"push r5\n\t"
		"push r4\n\t"
		"push r3\n\t"
		"push r2\n\t"
		"push r1\n\t"
		"push r0\n\t"
		"in r24, __SREG__\n\t"
		"push r24\n\t"
		"in r24, __SP_L__\n\t"
		"in r25, __SP_H__\n\t"
		// dead_proc_entry definition:
		// Better name would be next_proc_entry
		".dead_proc_entry:\n\t"
		// Figure out new process to do; next on ready queue.
		"rcall process_select\n\t"
		"eor r18,r18\n\t" // clear r18
		"or r18,r24\n\t" // store r24 into r18
		"or r18,r25\n\t" // add one's of r25 into r18
		"brne .label_resume\n\t"
		// load orig stack pointer
		"lds r25, _orig_sp_hi\n\t" // r25 <- _orig_sp_hi
		"out __SP_H__, r25\n\t"    // __SP_H__ <- r25
		"lds r24, _orig_sp_lo\n\t"
		"out __SP_L__, r24\n\t"
		// return
		"ret\n\t"
		".label_resume:\n\t"
		"out __SP_L__, r24\n\t" // __SP_L__ <- r24
		"out __SP_H__, r25\n\t"
		"pop r0\n\t"
		"out  __SREG__, r0\n\t"
		"pop r0\n\t"
		"pop r1\n\t"
		"pop r2\n\t"
		"pop r3\n\t"
		"pop r4\n\t"
		"pop r5\n\t"
		"pop r6\n\t"
		"pop r7\n\t"
		"pop r8\n\t"
		"pop r9\n\t"
		"pop r10\n\t"
		"pop r11\n\t"
		"pop r12\n\t"
		"pop r13\n\t"
		"pop r14\n\t"
		"pop r15\n\t"
		"pop r16\n\t"
		"pop r17\n\t"
		"pop r18\n\t"
		"pop r19\n\t"
		"pop r20\n\t"
		"pop r21\n\t"
		"pop r22\n\t"
		"pop r23\n\t"
		"pop r24\n\t"
		"pop r25\n\t"
		"pop r26\n\t"
		"pop r27\n\t"
		"pop r28\n\t"
		"pop r29\n\t"
		"pop r30\n\t"
		"pop r31\n\t"
		"reti\n\t");
}


/*
 * Stack: save 32 regs, +2 for entry point +2 for ret address
 */
#define EXTRA_SPACE 37
#define EXTRA_PAD 4

unsigned int process_init (void (*f) (void), int n)
{
  unsigned long stk;
  int i;
  unsigned char *stkspace;

  /* Create a new process */
  n += EXTRA_SPACE + EXTRA_PAD;
  stkspace = (unsigned char *) malloc (n);

  if (stkspace == NULL) {
    /* failed! */
    return 0;
  }

  /* Create the "standard" stack, including entry point */
  for (i=0; i < n; i++) {
      stkspace[i] = 0;
  }

  n -= EXTRA_PAD;

  stkspace[n-1] = ( (unsigned int) process_terminated ) & 0xff;
  stkspace[n-2] = ( (unsigned int) process_terminated ) >> 8;
  stkspace[n-3] = ( (unsigned int) f ) & 0xff;
  stkspace[n-4] = ( (unsigned int) f ) >> 8;

  /* SREG */
  stkspace[n-EXTRA_SPACE] = SREG;

  stk = (unsigned int)stkspace + n - EXTRA_SPACE - 1;

  return stk;
}
