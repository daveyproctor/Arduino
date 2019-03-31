#include "concurrency.h"

#define MSGSIZE 10

lock_t l;

typedef struct item item_t;
struct item {
	unsigned int val;
	struct item *next; /* link to next item */
};

typedef struct buffer buffer_t;
struct buffer {
    unsigned int len;
    struct item *head;
    struct item *tail;
};

void queueInit(buffer_t *Q);
void enqueue(buffer_t *Q, item_t *p);
item_t *dequeue(buffer_t *Q);

buffer_t *b = NULL;

void writer (void)
{
    unsigned int vals[MSGSIZE];
    for (int i = 0; i < MSGSIZE; i++){
        vals[i] = 2 * i;
    }
    for (int i = 0; i < MSGSIZE; i++){
        unsigned int nextval = vals[i];
        lock_acquire(&l);
        while (b->len > 2){
            lock_release(&l);
            lock_acquire(&l);
        }
        lock_release(&l);
    }
    digitalWrite(BLUE, HIGH);
    return;
}

void reader (void)
{
    unsigned int vals[MSGSIZE];
    for (int i = 0; i < MSGSIZE; i++){
        lock_acquire(&l);
        while (b->len == 0){
            lock_release(&l);
            lock_acquire(&l);
        }
        unsigned int nextval = dequeue(b);
        lock_release(&l);
        vals[i] = nextval;
    }
    for (int i = 0; i < MSGSIZE; i++){
        if (vals[i] != 2 * i){
            digitalWrite(WHITE, HIGH);
        }
    }
    digitalWrite(GREEN, HIGH);
    return;
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Setup");

    pinMode(BLUE, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(WHITE, OUTPUT);

    digitalWrite(WHITE, HIGH);
    delay(500);
    digitalWrite(WHITE, LOW);
    delay(500);
    if (queueTest(100) == 1){
        Serial.println("Queue test passed");
    }
    delay(1000);

    lock_init(&l);
    queueInit(b);
    if (process_create (writer, 64) < 0) {
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    if (process_create (reader, 64) < 0) {
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    process_start();
}

void loop()
{
}

void queueInit(buffer_t *Q){
    Q->len = 0;
    Q->head = NULL;
    Q->tail = NULL;
}

void enqueue(buffer_t *Q, item_t *p){
    if (Q->len == 0) {
        Q->head = p;
        Q->tail = p;
        Q->len = 1;
    } else {
        Q->tail->next = p;
        /*
         * Debug log: need to update tail, lest maxlen=2
         */
        Q->tail = p;
        Q->len++;
    }
    p->next = NULL;
    return;
}

item_t *dequeue(buffer_t *Q){
    if (Q->len == 0) {
        return NULL;
    }
    item_t *popped = Q->head;
    if (Q->len == 1) {
        Q->head = NULL;
        Q->tail = NULL;
    } else {
        Q->head = popped->next;
    }
    Q->len--;
    popped->next = NULL;
    return popped;
}
