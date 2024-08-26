#ifndef HEADER_H
#define HEADER_H

#include "../utils/defines.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Frequency Frequency;

struct Frequency {
    uint8_t byte;
    uint32_t frequency;
};

typedef struct Header Header;

struct Header {
    uint32_t input_file_size;
    uint8_t frequencies_num;
    Frequency *table;
};


Header create_header(uint32_t input_file_size, uint64_t char_freqs[MAX_CODE_NUM]);

void print_header(Header *h);

uint32_t get_file_size(const char *filename);

Frequency* filter_non_zero(uint64_t chars_freqs[MAX_CODE_NUM], uint8_t *size);

bool get_all_char_freqs(Header *h, uint64_t char_freqs[MAX_CODE_NUM]);

void write_header(Header h, const char *output_file);

bool read_header(const char *compressed_file, Header *h, uint32_t *offset);

void free_header(Header *h);

#endif