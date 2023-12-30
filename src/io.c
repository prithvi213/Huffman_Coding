#include "io.h"
#include "defines.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
//static uint8_t buffer_write[BLOCK];

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int byte_count, smaller_byte_amount;
    smaller_byte_amount = (nbytes < BLOCK) ? nbytes : BLOCK;

    while((byte_count = read(infile, buf + bytes_read, smaller_byte_amount)) > 0 && nbytes > 0) {
        nbytes -= byte_count;
        bytes_read += byte_count;
        smaller_byte_amount = (nbytes < BLOCK) ? nbytes : BLOCK;
    }

    buf[bytes_read] = '\0';
    return bytes_read;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int byte_count, smaller_byte_amount, smaller_size;
    smaller_size = ((int)(strlen((char *)buf)) < nbytes) ? (int)(strlen((char *)buf)) : nbytes;
    smaller_byte_amount = (smaller_size < BLOCK) ? smaller_size : BLOCK;

    while((byte_count = write(outfile, buf + bytes_written, smaller_byte_amount)) > 0 && nbytes > 0) {
        smaller_size -= byte_count;
        bytes_written += byte_count;
        smaller_byte_amount = (smaller_size < BLOCK) ? smaller_size : BLOCK;
    }

    return bytes_written;
}

bool read_bit(int infile, uint8_t *bit) {
    static uint8_t buffer_read[BLOCK];
    static uint32_t index = 0;

    if(index == 0) {
        int bytes_read = read(infile, buffer_read, BLOCK);

        if(bytes_read == 0) {
            return false;
        }
    }

    *bit = (buffer_read[index / 8] >> (index % 8)) & 1;
    index = (index + 1 % BLOCK);
    return true;
}

//void write_code(int outfile, Code *c) {}

//void flush_codes(int outfile) {}
