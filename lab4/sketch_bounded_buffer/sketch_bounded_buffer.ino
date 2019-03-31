#include "concurrency.h"

#define MSGSIZE 10
#define BUFFERSIZE 2

lock_t l;

typedef struct item item_t;
struct item {
	unsigned int val;
	struct item *next; /* link to next item */
};

typedef struct buffer buffer_t;
struct buffer {
    int len;
    struct item *head;
    struct item *tail;
};

void queueInit(buffer_t *Q);
void enqueue(buffer_t *Q, item_t *p);
item_t *dequeue(buffer_t *Q);

buffer_t *b = NULL;

item_t items[MSGSIZE];

void writer (void)
{
    unsigned int vals[MSGSIZE];
    for (int i = 0; i < MSGSIZE; i++){
        vals[i] = 2 * i;
    }
    for (int i = 0; i < MSGSIZE; i++){
        unsigned int nextval = vals[i];
        item_t *nextItem = &items[i]; // malloc(sizeof(item_t));
        if (nextItem == NULL){
            // failing here with malloc, dunno what to say.
            digitalWrite(WHITE, HIGH);
        }
        nextItem->val = nextval;
        lock_acquire(&l);
        while (b->len == BUFFERSIZE){
            // digitalWrite(WHITE, HIGH);
            lock_release(&l);
            lock_acquire(&l);
        }
        enqueue(b, nextItem);
        lock_release(&l);
    }
    digitalWrite(BLUE, HIGH);
    return;
}

unsigned int readVals[MSGSIZE];
void reader (void)
{
    for (int i = 0; i < MSGSIZE; i++){
        lock_acquire(&l);
        while (b->len == 0){
            digitalWrite(WHITE, HIGH); // keeps turning on
            digitalWrite(WHITE, LOW);
            lock_release(&l);
            lock_acquire(&l);
        }
        item_t *nextItem = dequeue(b);
        unsigned int nextval = nextItem->val;
        lock_release(&l);
        readVals[i] = nextval;
    }
    if (b->len != 0){
        // digitalWrite(WHITE, HIGH);
    }
    for (int i = 0; i < MSGSIZE; i++){
        if (readVals[i] != 2 * i){
            digitalWrite(YELLOW, HIGH);
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
    b = malloc(sizeof(buffer_t));
    if (b == NULL){
        digitalWrite(WHITE, HIGH);
    }
    queueInit(b);
    if (process_create (writer, 64) < 0) {
      // digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    if (process_create (reader, 64) < 0) {
      digitalWrite(WHITE, 1);
      Serial.println("FAIL");
      return;
    }
    process_start();
    return;
    // for (int i = 0; i < MSGSIZE; i++){
    //     Serial.println(readVals[i]);
    //     delay(100);
    // }
}

void loop()
{
    // digitalWrite(YELLOW, HIGH);
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
        Q->tail = p;
        Q->len++;
    }
    p->next = NULL;
    return;
}

item_t *dequeue(buffer_t *Q){
    if (Q->len == 0) {
        digitalWrite(WHITE, HIGH);
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
