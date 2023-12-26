#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "node.h"

// Creates a node
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    
    if(n != NULL) {
        n->symbol = symbol;
        n->frequency = frequency;
    }

    return n;
}

// Deletes a node
void node_delete(Node **n) {
    if(n && *n) {
        free(*n);
        *n = NULL;
    }

    return;
}

// Joins two nodes
Node *node_join(Node *left, Node *right) {
    Node *parent = node_create('$', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    return parent;
}

// Prints a node
void node_print(Node *n) {
    printf("(%c|%llu)", n->symbol, n->frequency);
}
