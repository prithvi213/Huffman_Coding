#pragma once

#include "node.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct Stack Stack;

struct Stack { 
    uint32_t top;
    uint32_t capacity;
    Node **items; 
};

Stack *stack_create(uint32_t capacity);

void stack_delete(Stack **s);

bool stack_empty(Stack *s);

bool stack_full(Stack *s);

uint32_t stack_size(Stack *s);

bool stack_push(Stack *s, Node *n);

bool stack_pop(Stack *s, Node **n);

void stack_print(Stack *s);
