#include "code.h"

#include <stdio.h>
#include <stdlib.h>

#define BITS_PER_BLOCK 8

// Creates the code
Code code_init(void) {
    Code c;
    c.top = 0;

    for(uint32_t i = 0; i < MAX_CODE_SIZE; i++) {
        c.bits[i] = 0;
    }

    return c;
}

// Returns size of code
uint32_t code_size(Code *c) {
    return c->top;
}

// Returns if code is empty
bool code_empty(Code *c) {
    return (c->top == 0);
}

// Returns if code is full
bool code_full(Code *c) {
    return (c->top == ALPHABET);
}

// Sets bit at given index
bool code_set_bit(Code *c, uint32_t i) {
    if(i >= ALPHABET) {
        return false;
    }

    c->bits[i / BITS_PER_BLOCK] |= (1 << i % BITS_PER_BLOCK);
    return true;
}

// Clears bit at given index
bool code_clr_bit(Code *c, uint32_t i) {
    if(i >= ALPHABET) {
        return false;
    }

    c->bits[i / BITS_PER_BLOCK] &= ~(1 << i % BITS_PER_BLOCK);
    return true;
}

// Gets bit at given index
bool code_get_bit(Code *c, uint32_t i) {
    if(i >= ALPHABET) {
        return false;
    }

    return (c->bits[i / BITS_PER_BLOCK] >> (i % BITS_PER_BLOCK)) & 1;
}

// Pushes bit onto code
bool code_push_bit(Code *c, uint8_t bit) {
    if(code_full(c)) {
        return false;
    }

    if(bit == 1) {
        code_set_bit(c, c->top);
    } else {
        code_clr_bit(c, c->top);
    }

    c->top += 1;
    return true;
}

// Pops bit off code
bool code_pop_bit(Code *c, uint8_t *bit) {
    if(code_empty(c)) {
        return false;
    }

    c->top -= 1;
    *bit = code_get_bit(c, c->top);
    code_clr_bit(c, c->top);
    return true;
}

// Prints out code
void code_print(Code *c) {
    for(int64_t i = 0; i < ((int64_t)c->top); i++) {
        printf("%u", code_get_bit(c, i));
    }

    printf("\n");
}
