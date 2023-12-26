#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

// Creates the stack
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));

    if (s != NULL) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) calloc(s->capacity, sizeof(Node *));
    }

    return s;
}

// Deletes the stack
void stack_delete(Stack **s) {
    if(s && *s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }

    return;
}

// Checks if stack is empty
bool stack_empty(Stack *s) {
    return (s->top == 0);
}

// Checks if stack is full
bool stack_full(Stack *s) {
    return (s->top == s->capacity);
}

// Returns stack size
uint32_t stack_size(Stack *s) {
    return s->top;
}

// Pushes element onto stack
bool stack_push(Stack *s, Node *n) {
    if(stack_full(s)) {
        return false;
    }

    s->items[s->top] = n;
    s->top += 1;
    return true;
}

// Pops element off stack
bool stack_pop(Stack *s, Node **n) {
    if(stack_empty(s)) {
        return false;
    }

    s->top -= 1;
    *n = s->items[s->top];
    return true;
}

// Prints out stack contents
void stack_print(Stack *s) {
    for(uint32_t i = 0; i < s->top - 1; i++) {
        node_print(s->items[i]);
        printf(", ");
    }

    node_print(s->items[s->top - 1]);
    printf("\n");
}
