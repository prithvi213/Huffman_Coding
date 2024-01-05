#include "code.h"
#include "io.h"
#include "pq.h"
#include "huffman.h"
#include "defines.h"
#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

#define OPTIONS "hvi:o:"

static int header_size;

// Inline function used to scrape values at a line (used for first 4 lines of encoded file)
static inline unsigned long get_value_at_line(int iFile) {
    static char buf[BLOCK];
    char c;
    int index = 0, br = 0;

    while((br = read(iFile, &c, 1)) > 0) {
        if(c == '\n') {
            header_size += 1;
            buf[index] = '\0';
            break;
        }

        buf[index] = c;
        index += 1;
        header_size += 1;
    }

    unsigned long value = strtoul(buf, NULL, 10);
    memset(buf, 0, BLOCK);
    return value;
}

int main(int argc, char **argv) {
    int opt = 0, iFile = 0, oFile = 1;
    bool print_usage = false, print_stats = false;
    char *infile_name = NULL, *outfile_name = NULL;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': print_usage = true; break;
        case 'v': print_stats = true; break;
        case 'i': infile_name = optarg; break;
        case 'o': outfile_name = optarg; break;
        default: print_usage = true; break;
        }
    }

    // Prints program description
    if (print_usage) {
        printf("SYNOPSIS\n");
        printf("  A Huffman decoder.\n");
        printf("  Decompresses a file using the Huffman coding algorithm.\n\n");
        printf("USAGE\n");
        printf("  ./decode [-h] [-i infile] [-o outfile]\n\n");
        printf("OPTIONS\n");
        printf("  -h             Program usage and help.\n");
        printf("  -v             Print compression statistics.\n");
        printf("  -i infile      Input file to decompress.\n");
        printf("  -o outfile     Output of decompressed data.\n");
    }

    // Open infile if it's not NULL
    if(infile_name != NULL) {
        iFile = open(infile_name, O_RDONLY);
    }

    // Open outfile if it's not NULL
    if(outfile_name != NULL) {
        oFile = open(outfile_name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXU);
    }

    // Step 1: Verify magic number
    uint32_t magic = (uint32_t)get_value_at_line(iFile);

    if(magic != MAGIC) {
        fprintf(stderr, "Magic Number Doesn't Match\n");
        return 1;
    }

    // Step 2: Get the permissions and set it for the outfile
    uint32_t permissions = (uint32_t)get_value_at_line(iFile);
    fchmod(oFile, permissions);

    // Step 3: Get tree size and use it to read dumped tree from the infile
    // Then reconstruct tree using dumped tree data from infile
    uint16_t tree_size = (uint16_t)get_value_at_line(iFile);
    uint64_t file_size = (uint64_t)get_value_at_line(iFile);
    uint8_t tree[tree_size];
    uint8_t new_line = '\n';

    read_bytes(iFile, tree, tree_size);
    read_bytes(iFile, &(new_line), 1);

    Node *rebuilt_tree = rebuild_tree(tree_size, tree);

    // Step 4: Read infile one bit at a time and traverse down tree as each bit is read
    uint8_t bit;
    uint8_t *buf = (uint8_t *) malloc(BLOCK * sizeof(uint8_t));
    uint64_t size = 0;
    uint32_t buf_index = 0;
    Node *root = rebuilt_tree;

    while(size < file_size) {
        // Check if at a leaf node and write the symbol
        if(root->left == NULL && root->right == NULL) {
            buf[buf_index] = root->symbol;

            if(buf_index == BLOCK - 1) {
                write_bytes(oFile, buf, BLOCK);
                memset(buf, 0, BLOCK);
            }

            size += 1;
            buf_index = (buf_index + 1) % BLOCK;
            root = rebuilt_tree;
        } else {
            // Otherwise, read a bit and traverse
            read_bit(iFile, &bit);

            if(bit == 0) {
                root = root->left;
            } else {
                root = root->right;
            }
        }
    }
    
    // Write out remaining bytes in buffer
    write_bytes(oFile, buf, buf_index);
    memset(buf, 0, buf_index);

    // If print_stats option is envoked
    if(print_stats) {
        struct stat stat_ifile;
        fstat(iFile, &stat_ifile);
        printf("File Size of Encoded Input File: %llu\n", stat_ifile.st_size - header_size - tree_size - 1);
        printf("File Size of Decoded File: %llu\n", file_size);
        
        double space_saving = 100.0 * (1 - (((double)stat_ifile.st_size - header_size - tree_size - 1) / ((double)file_size)));
        space_saving = round(space_saving * 10.0) / 10.0;
        printf("Space Saved: %.1f%%\n", space_saving);
    }

    // Step 5: Close infile and outfile and free up memory
    delete_tree(&rebuilt_tree);
    free(buf);
    close(iFile);
    close(oFile);

    return 0;
}
