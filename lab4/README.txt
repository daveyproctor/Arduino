--------------------------------------------------------
Davey Proctor and Michael McNamara
README for Lab 4
--------------------------------------------------------

For queueing processes, we have a "*process_t" struct which keeps track of the current process
and a flexible "process_queue_t" struct which keeps track of the processes waiting for mutual exclusion.
Two "process_queue_t" structures we have are a readyQueue and a waitingQueue for the two states of funcitons above.
We also have a "testQueue", but this is just a sanity check.
We have implemented several functions that operate on the process_t and process_queue_t structs:

    - queueInit: initializes the queue in memory with length zero.
    - enqueue: adds a process to the queue and extends its length by 1
    - dequeue: pops off the process at the top of the queue back into the system and decreases queue length by 1.
    - queueTest: sanity check to ensure our queueing structure works.

We use these funcitons for both our concurrency and lock implementations.

Our processes can have four possible states in our queues:
    - RUNNING: The process is online.
    - WAITING: The process has to wait for mutual exclusion before it can proceed.
    - READY: The process either still has some work to do before queueing for mutual exclusion or doesn't need it at all.
    - DEAD: The process is finished running.

Our process_state structure that we use to keep track of our context-switching processes has the following components:
    - sp_bot: Bottom of that process's stack, for keeping track of malloc'd memory.
    - sp: The stack pointer, which needs to be saved when a process is put on hold.
    - process_state *next: Another process to which this one is linked.
    - state: One of the four queueing states above.

The functions we've implemented for concurrency are as follows:

    - process_create (atomic): blocks out the memory needed for a process and puts it on the readyQueue.
            + if the process is the first one created, we create the readyQueue at this point as well.
            + blocking out the process calls process_init, but blocking out the queue involves direct mallocing.
    - process_start: initializes the processes to start running concurrently.
            + relies mostly on process_begin.
    - process_select: swaps out the currently running process for the one on top of the waiting queue.
            + the process that had been currently running gets put on the readyQueue.
            + if it calls a dead process, that process is destroyed.
            + works even if you only have one process running.

For locks, we create another struct, lock_state lock_t.
This structure keeps track of the process currently holding the lock
and the processes on a waitingQueue ready to take the lock from the current process.

The functions on this structure are as follows:
    - lockInit: initializes the waitingQueue and holder structures for the lock.
    - lock_acquire (atomic): 
        + if the lock currently belongs to nobody, than the current process requesting the lock acquires it.
        + if the lock is currently owned by a process, then the process requesting the lock is put on a waitingQueue and yields.
        + This yielding is to ensure mutual exclusion.
    - lock_release (atomic): passes the lock on to the next function in the waitingQueue requesting mutual exclusion,
        + if no function is on the waitingQueue, the lock belongs to nobody.


We often used LEDs for debugging our concurrency and lock issues, with a WHITE LED being our primary error indicator.
Most of the times in our code where we write this WHITE LED to HIGH, 
an error has occurred where the program has reached a state we didn't want it to.
We also often test our queue structure in our setup before preceding with the bulk of our function.

--------------------------------------------------------

We have a couple examples for each part of the lab.
-- Part 1:        
        + Context-switching LEDs (sketch_locks_LEDs)
        + Terminating and self-chaining processes (sketch_chain_processes)

-- Part 2:
        + Two bouncing balls sharing SPI resources (sketch_bouncing)
        + Bounded buffer problem (sketch_bounded_buffer)

Included in our .zip file are demonstrations of each of these with narration that describes their purposes.
