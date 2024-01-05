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

    // If print usage option is envoked
    if (print_usage) {
        printf("SYNOPSIS\n");
        printf("  A Huffman encoder.\n");
        printf("  Compresses a file using the Huffman coding algorithm.\n\n");
        printf("USAGE\n");
        printf("  ./encode [-h] [-i infile] [-o outfile]\n\n");
        printf("OPTIONS\n");
        printf("  -h              Program usage and help.\n");
        printf("  -v              Print compression statistics.\n");
        printf("  -i infile       Input file to compress.\n");
        printf("  -o outfile      Output of compressed data.\n");
    }

    // Open infile if it's not NULL
    if(infile_name != NULL) {
        iFile = open(infile_name, O_RDONLY);
    }

    // Open outfile if it's not NULL
    if(outfile_name != NULL) {
        oFile = open(outfile_name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXU);
    }

    // Step 1: Read through infile to construct histogram and copy data into string for future use
    uint64_t hist[ALPHABET] = {0};
    uint8_t *buf = (uint8_t *) malloc(BLOCK * sizeof(uint8_t));
    int bytes_read = 0;

    while((bytes_read = read_bytes(iFile, buf, BLOCK)) > 0) {
        for(int i = 0; i < bytes_read; i++) {
            hist[buf[i]] += 1;
        }

        memset(buf, 0, bytes_read);
    }

    // Step 2: Increment indexes for histogram[0] and histogram[255]
    hist[0] += 1;
    hist[255] += 1;

    // Step 3: Construct Huffman Tree
    Node *root = build_tree(hist);

    // Step 4: Create a Code Table
    Code table[ALPHABET];
    Code c = code_init();

    for(uint32_t i = 0; i < ALPHABET; i++) {
        table[i] = c;
    }

    build_codes(root, table);

    // Step 5: Construct a header and set its fields
    Header h;
    struct stat stat_ifile, stat_ofile;
    h.magic = MAGIC;
    int different_symbols = 0;
    
    if(fstat(iFile, &stat_ifile) < 0) {
        return 1;
    }

    h.permissions = (stat_ifile.st_mode & 0777);
    fchmod(oFile, h.permissions);
    
    for(uint32_t i = 0; i < ALPHABET; i++) {
        if(hist[i] > 0) {
            different_symbols += 1;
        }
    } 

    h.tree_size = (3 * different_symbols) - 1;
    h.file_size = stat_ifile.st_size;

    // Step 6: Write Header to Outfile
    snprintf((char *)buf, BLOCK, "%u\n%u\n%u\n%llu\n", h.magic, h.permissions, h.tree_size, h.file_size);
    int bytes_written = write_bytes(oFile, buf, strlen((char*)buf));
    uint64_t header_size = (uint64_t)bytes_written;
    memset(buf, 0, bytes_written);

    // Step 7: Write constructed Huffman Tree to Outfile
    dump_tree(oFile, root);
    uint8_t new_line = '\n';
    write_bytes(oFile, &(new_line), 1);

    // Step 8: Write corresponding code for each symbol to outfile and flush remaining codes
    lseek(iFile, 0, SEEK_SET);

    while((bytes_read = read_bytes(iFile, buf, BLOCK)) > 0) {        
        for(int i = 0; i < bytes_read; i++) {
            // Write the code for each character in the buffer
            write_code(oFile, &(table[buf[i]]));
        }

        memset(buf, 0, bytes_read);
    }
    
    // Write the remaining codes in the buffer which haven't been written out
    flush_codes(oFile);

    // If print_stats option is envoked
    if(print_stats) {
        fstat(oFile, &stat_ofile);
        printf("File Size of Original File: %llu\n", h.file_size);
        printf("File Size of Encoded Output: %llu\n", stat_ofile.st_size - header_size - h.tree_size - 1);
        
        double space_saving = 100.0 * (1 - (((double)stat_ofile.st_size - header_size - h.tree_size - 1) / ((double)h.file_size)));
        space_saving = round(space_saving * 10.0) / 10.0;
        printf("Space Saved: %.1f%%\n", space_saving);
    }

    // Step 9: Close infile and outfile and free up memory
    free(buf);
    delete_tree(&root);
    close(iFile);
    close(oFile);

    return 0;
}
