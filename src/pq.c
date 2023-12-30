#include "pq.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Priority queue struct
struct PriorityQueue { 
    uint32_t capacity;
    uint32_t top;
    Node **items;
    uint32_t head;
    uint32_t tail;
};

// Creates priority queue
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));

    if(q) {
        q->capacity = capacity;
        q->top = 0;
        q->items = (Node **) calloc(q->capacity, sizeof(Node *));
        q->head = q->tail = 0;
    }

    return q;
}

// Delete priority queue
void pq_delete(PriorityQueue **q) {
    if(q && *q && (*q)->items) {
        free((*q)->items);
        free(*q);
        *q = NULL;
    }

    return;
}

// Checks if prority queue is empty
bool pq_empty(PriorityQueue *q) {
    return (q->top == 0);
}

// Checks if priority queue is full
bool pq_full(PriorityQueue *q) {
    return (q->top == q->capacity);
}

// Returns size of priority queue
uint32_t pq_size(PriorityQueue *q) {
    return q->top;
}

// Enqueues an element into priority queue
bool enqueue(PriorityQueue *q, Node *n) {
    if(pq_full(q)) {
        return false;
    }

    int64_t max_index = ((int64_t)q->tail) - 1;

    while(max_index >= 0 && q->items[max_index]->frequency > n->frequency) {
        q->items[(uint32_t)(max_index + 1)] = q->items[(uint32_t)(max_index)];
        max_index -= 1;
    }

    q->items[(uint32_t)(max_index + 1)] = n;
    q->tail += 1;
    q->top += 1;
    return true;
}

// Dequeues element from priority queue
bool dequeue(PriorityQueue *q, Node **n) {
    if(pq_empty(q)) {
        return false;
    }

    q->top -= 1;
    *n = q->items[q->head];
    q->head += 1;
    return true;
}

// Prints out whole priority queue
void pq_print(PriorityQueue *q) {
    for(int64_t i = q->head; i < ((int64_t)q->tail) - 1; i++) {
        node_print(q->items[(uint32_t)i]);
        printf(", ");
    }

    if(q->top > 0) {
        node_print(q->items[q->tail - 1]);
    }

    printf("\n");
}
