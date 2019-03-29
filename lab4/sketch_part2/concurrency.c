#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "concurrency.h"

/*
 * processes
 */

enum State{RUNNING, READY, WAITING, DEAD};

struct process_state {
	unsigned int sp; /* stack pointer */
	struct process_state *next; /* link to next process */
	int state;
};

// RUNNING process
process_t *current_process = NULL;

/*
 * Process queue for READY and WAITING
 */
typedef struct process_queue process_queue_t;
struct process_queue {
    unsigned int len;
    struct process_state *head;
    struct process_state *tail;
};

process_queue_t *readyQueue = NULL;

void queueInit(process_queue_t *Q){
    Q->len = 0;
    Q->head = NULL;
    Q->tail = NULL;
}

void enqueue(process_queue_t *Q, process_t *p){
    if (Q->len == 0) {
        Q->head = p;
        Q->tail = p;
        Q->len = 1;
    } else {
        Q->tail->next = p;
        Q->len++;
    }
}

struct process_t *dequeue(process_queue_t *Q){
    if (Q->len == 0) {
        return NULL;
    } 
    process_t *popped = Q->head;
    if (Q->len == 1) {
        Q->head = NULL;
        Q->tail = NULL;
    } else {
        Q->head = popped->next;
    }
    Q->len--;
    return popped;
}

int process_create (void (*f) (void), int n) {
	asm volatile ("cli\n\t");
    if (readyQueue == NULL){
        readyQueue = malloc(sizeof(process_queue_t));
        queueInit(readyQueue);
    }
	struct process_state *p_state = malloc(sizeof(process_t));
	if (!p_state){
		return -1;
	}
	p_state->sp = process_init(f, n);
	p_state->state = READY;
    enqueue(readyQueue, p_state);
	// p_state->next = current_process;
	// current_process = p_state;
	asm volatile ("sei\n\t");
}

void process_start (void){
    current_process = dequeue(readyQueue);
    if (current_process == NULL){
        return;
    }
    current_process->state = RUNNING;
	process_begin();
}
struct process_state *readyQueueEnd(void){
    if (!current_process){
		return 0;
	}
    struct process_state *end;
    for (end = current_process; end->next != NULL; end = end->next);
    return end;
}
/*
 * Additionally need to re-queue current process; not if process_begin though.
 */
unsigned int process_select (unsigned int cursp) {
	if (cursp == 0){
        // This is the stack pointer of setup.
        // We either just began or a process has terminated.
	}
  if (!current_process){
		return 0;
	}

	return current_process->sp;
}

// these represent the main "setup"'s stack pointer. After process_begin happens,
// these values become the stack pointer whenever a process terminates
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
  asm volatile ("cli\n\t");
  // Remove current_process from queue
  struct process_state *tmp = current_process;
  free(current_process->sp);
  if (current_process->next == NULL){
      current_process = NULL;
  } else {
    current_process = current_process->next;
  }
  free(tmp);
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
  if (!current_process) {
      return;
  }
  asm volatile ("cli\n\t");

	// Move current process to end, round robin style
	// Should work for just one process, too.
	if (current_process->state == READY){
		struct process_state *end = readyQueueEnd();
		end->next = current_process;
		end = current_process;
		current_process = current_process->next;
		end->next = NULL;
	} else {
		// Assume on a waiting queue
		current_process = current_process->next;
	}
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
		// This is how process_select(cursp) gets its argument
		"in r24, __SP_L__\n\t"
		"in r25, __SP_H__\n\t"
		// dead_proc_entry definition:
		// Better name would be try_next_proc
		".dead_proc_entry:\n\t"
		// Figure out new process to do; next on ready queue.
		"rcall process_select\n\t"
		"eor r18,r18\n\t" // clear r18
		"or r18,r24\n\t"  // r18 <- r24                  
		"or r18,r25\n\t"  // add one's of r25 into r18
		// ^ we just tested if r24 = r25 =(?) 0; this is the case where we have no next process
		"brne .label_resume\n\t"
		// Assuming no next process, load orig stack pointer
		// No need to pop stack?
		"lds r25, _orig_sp_hi\n\t" // r25 <- _orig_sp_hi
		"out __SP_H__, r25\n\t"    // __SP_H__ <- r25
		"lds r24, _orig_sp_lo\n\t"
		"out __SP_L__, r24\n\t"
		// return -- interrupts still disabled?
        // I think this move is to keep our sanity in this assignment  - don't keep calling yield if you just told me there were no processes to be selected; come back into main's loop() and go from there.
		"ret\n\t"
		".label_resume:\n\t" // Should be called "next_process_resume"
		// Crux of the context switch
        // Takes return value of process_select
		"out __SP_L__, r24\n\t" // __SP_L__ <- r24
		"out __SP_H__, r25\n\t"
		// Symmetric to proc_init and process_timer_interrupt beginning.
        //
        /* Suppose this process has previously run; had been interrupted; is now selected via process_select: */
        //    REFERENCE            DATA                        SIZE
        // ------------------------------------------------------------
        // ||                   -> 0                   ||  --| {EXTRA_PAD}
        // ||                   -> 0                   || 
        // ||                   -> process_terminated  ||  --|  
        // ||                   -> f                   ||    | {n0 + EXTRA_SPACE}
        // ||                   -> ?                   ||    | 
        // ||                   -> (...)               ||    | 
        // ||                   -> ?                   ||    | 
        // ||                   -> PC_WHEN_INTERRUPTED ||    | 
        // ||                   -> {r31}               ||    |
        // ||                   -> (...)               ||    |
        // ||                   -> {r0}                ||    |
        // ||                   -> SREG                ||    |
        // || __SP_H__^__SP_L__ -> 0                   ||    | 
        // ||                   -> (...)               ||    | 
        // ||                   -> 0                   ||  --| 
        
		"pop r0\n\t" // r0 <- next_proc SREG
		"out  __SREG__, r0\n\t" // SREG <- r0
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
		"reti\n\t"); // in next_proc
}


/*
 * Stack: save 32 regs, +2 for entry point +2 for ret address
 */
#define EXTRA_SPACE 37
#define EXTRA_PAD 4

unsigned int process_init (void (*f) (void), int n) // n0
{
  unsigned long stk;
  int i;
  unsigned char *stkspace;

  /* Create a new process */
  n += EXTRA_SPACE + EXTRA_PAD; // n1
  stkspace = (unsigned char *) malloc (n);

  if (stkspace == NULL) {
    /* failed! */
    return 0;
  }

  /* Create the "standard" stack, including entry point */
  //    REFERENCE        DATA                        SIZE
  // ------------------------------------------------------------
  // || stkspace + n1 -> 0                  ||  --| {EXTRA_PAD}
  // ||               -> 0                  || 
  // || stkspace + n2 -> process_terminated ||  --|  
  // ||               -> f                  ||    | {EXTRA_SPACE}
  // ||               -> {r31=0}            ||    |
  // ||               -> (...)              ||    |
  // ||               -> {r0=0}             ||    |
  // ||               -> SREG               ||  --|
  // || stk           -> 0                  || 
  // ||               -> (...)              ||  --| {n0}
  // || stkspace      -> 0                  || 
  
  for (i=0; i < n; i++) {
      stkspace[i] = 0;
  }

  n -= EXTRA_PAD; // n2

  stkspace[n-1] = ( (unsigned int) process_terminated ) & 0xff;
  stkspace[n-2] = ( (unsigned int) process_terminated ) >> 8;
  stkspace[n-3] = ( (unsigned int) f ) & 0xff;
  stkspace[n-4] = ( (unsigned int) f ) >> 8;

  /* SREG */
  stkspace[n-EXTRA_SPACE] = SREG;

  stk = (unsigned int)stkspace + n - EXTRA_SPACE - 1;

  return stk;
}

// Locks

/*
// Initialize a new waiting queue
void lock_init (lock_t *l){
	asm volatile ("cli\n\t");
	l->holder = NULL;
	l->headWaitingQueue = NULL;
	l->tailWaitingQueue = NULL;
	asm volatile ("sei\n\t");
}

// Add you to a waiting queue
void lock_acquire (lock_t *l){
	asm volatile ("cli\n\t");
    if (l->holder){
        // Join the end of the ready queue
				if (!l->headWaitingQueue){
					l->headWaitingQueue = current_process;
				}
				else {
					l->tailWaitingQueue->next = current_process;
				}
				l->tailWaitingQueue = current_process;
				current_process->state = WAITING;
        yield();
    }
    l->holder = current_process;
		current_process->state = READY;
	asm volatile ("sei\n\t");
}

// Pop the next person off the waiting queue if anyone there.
void lock_release (lock_t *l){
	asm volatile ("cli\n\t");

	if (!l->headWaitingQueue){
		l->holder = NULL;
		return;
	}
	struct process_state nextHolder = l->headWaitingQueue;
	l->headWaitingQueue = nextHolder->next;
	l->holder = nextHolder;
	nextHolder->state = READY;
	asm volatile ("sei\n\t");
}
*/





















