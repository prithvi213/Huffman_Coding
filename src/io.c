#include "io.h"
#include "defines.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define BITS_PER_BLOCK 8

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t buffer_write[BLOCK] = {0};
static int index_val = 0;

// Read bytes from infile into the buffer
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int byte_count = 0, current_bytes_read = 0;
    int smaller_amount = (nbytes < BLOCK) ? nbytes : BLOCK;

    while((byte_count = read(infile, buf + current_bytes_read, smaller_amount)) > 0) {
        nbytes -= byte_count;
        bytes_read += byte_count;
        current_bytes_read += byte_count;
        smaller_amount = (nbytes < BLOCK) ? nbytes : BLOCK;
    }

    return current_bytes_read;
}

// Write bytes to outfile
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int byte_count = 0, current_bytes_written = 0;
    int smaller_amount = (nbytes < BLOCK) ? nbytes : BLOCK;

    while((byte_count = write(outfile, buf + current_bytes_written, smaller_amount)) > 0 && nbytes > 0) {
        nbytes -= byte_count;
        bytes_written += byte_count;
        current_bytes_written += byte_count;
        smaller_amount = (nbytes < BLOCK) ? nbytes : BLOCK;
    }

    return current_bytes_written;
}

// Reads bits from encoded output to decode output
bool read_bit(int infile, uint8_t *bit) {
    static uint8_t buffer_read[BLOCK];
    static uint32_t index = 0;
    static int br = 0;

    if(index % (BLOCK * 8) == 0) {
        memset(buffer_read, 0, br);
        br = read_bytes(infile, buffer_read, BLOCK);
        index = 0;

        if(br <= 0) {
            return false;
        }
    }

    *bit = (buffer_read[index / BITS_PER_BLOCK] >> (index % BITS_PER_BLOCK)) & 1;
    index += 1;
    return true;
}

// Write codes in block chunks and use index tracker to determine which byte to set
void write_code(int outfile, Code *c) {
    for(uint32_t i = 0; i < code_size(c); i++) {
        if(index_val == (BLOCK * 8)) {
            write_bytes(outfile, buffer_write, BLOCK);
            memset(buffer_write, 0, BLOCK);
            index_val = 0;
        }

        if(code_get_bit(c, i)) {
            buffer_write[index_val / BITS_PER_BLOCK] |= (1 << index_val % BITS_PER_BLOCK);
        } else {
            buffer_write[index_val / BITS_PER_BLOCK] &= ~(1 << index_val % BITS_PER_BLOCK);
        }

        index_val += 1;      
    }
}

// Flush remaining codes in buffer_write
void flush_codes(int outfile) {
    if(index_val > 0) {
        uint32_t bytes = (index_val / BITS_PER_BLOCK) + (index_val % BITS_PER_BLOCK ? 1 : 0);
        int bw = write_bytes(outfile, buffer_write, bytes);
        memset(buffer_write, 0, bw);
    }
}
