#include "huffman.h"
#include "pq.h"
#include "stack.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Extern variable for build_codes
Code c;

Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *q = pq_create(ALPHABET);
    Node *root;

    for(uint32_t i = 0; i < ALPHABET; i++) {
        if(hist[i] > 0) {
            enqueue(q, node_create((uint8_t)i, hist[i]));
        }
    }

    while(pq_size(q) > 1) {
        Node *left, *right, *parent;
        dequeue(q, &left);
        dequeue(q, &right);
        parent = node_join(left, right);
        enqueue(q, parent);
    }

    dequeue(q, &root);
    pq_delete(&q);
    return root;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    uint8_t bit;

    if(root != NULL) {
        if(root->left == NULL && root->right == NULL) {
            table[root->symbol] = c;
        } else {
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, &bit);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &bit);
        }
    }
}

void dump_tree(int outfile, Node *root) {
    if(root != NULL) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        uint8_t node_type;

        if(root->left == NULL && root->right == NULL) {
            node_type = 'L';
            write_bytes(outfile, &node_type, 1);
            write_bytes(outfile, &(root->symbol), 1);
        } else {
            node_type = 'I';
            write_bytes(outfile, &node_type, 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(nbytes);
    Node *root;

    for(uint16_t i = 0; i < nbytes; i++) {
        if(tree[i] == 'L') {
            stack_push(s, node_create(tree[i + 1], 0));
            //printf("Character is: %c\n", tree[i + 1]);
            i += 1;
            
        } else if(tree[i] == 'I') {
            Node *left, *right;
            stack_pop(s, &right);
            stack_pop(s, &left);
            stack_push(s, node_join(left, right));
        } else {
            continue;
        }
    }

    stack_pop(s, &root);
    stack_delete(&s);
    return root;
}

void delete_tree(Node **root) {
    if(root && *root) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        free(*root);
        *root = NULL;
    }

    return;
}
