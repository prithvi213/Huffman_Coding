#include "code.h"
#include "io.h"
#include "pq.h"
#include "huffman.h"
#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void) {
    /*int nbytes = 50000;
    uint8_t *buf1 = (uint8_t *)malloc(4096 * sizeof(uint8_t));
    int infd = open("../files/example.txt", O_RDONLY);
    int outfd = open("../files/out.txt", O_CREAT | O_WRONLY | O_TRUNC | S_IRUSR | S_IWUSR | S_IXUSR, 0644);
    read_bytes(infd, buf1, nbytes);
    int w = write_bytes(outfd, buf1, nbytes);
    printf("Bytes Written %d\n", w);

    close(infd);
    close(outfd);*/

    /*PriorityQueue *q = pq_create(5);
    enqueue(q, node_create('A', 700));
    enqueue(q, node_create('B', 113));
    enqueue(q, node_create('C', 111));
    enqueue(q, node_create('D', 350));
    enqueue(q, node_create('F', 182));
    pq_print(q);

    uint64_t hist[256];

    for(uint32_t i = 0; i < 256; i++) {
        hist[i] = 0;
    }
    hist['A'] = 700;
    hist['B'] = 113;
    hist['C'] = 111;
    hist['D'] = 350;
    hist['F'] = 182;

    Node *root = build_tree(hist);
    Code table[256];
    Code c = code_init();

    for(int i = 0; i < 256; i++) {
        table[i] = c;
    }
    
    build_codes(root, table);

    for(int i = 0; i < 256; i++) {
        if(code_size(&(table[i])) > 0) {
            printf("table[%c] = ", (uint8_t)i);
            code_print(&(table[i]));
        }
    }

    int outfd = open("../files/out2.txt", O_RDONLY); 
    dump_tree(outfd, root);   
    uint8_t buf[14];
    printf("%d\n", outfd);
    int64_t bytes_read = read_bytes(outfd, buf, MAX_TREE_SIZE);
    printf("%lld\n", bytes_read);
    printf("%s\n", buf);

    Node *new_root = rebuild_tree(bytes_read, buf);
    node_print(new_root->right->right->left);

    close(outfd);*/

    return 0;
}
