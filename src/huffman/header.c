#include "header.h"
#include "../utils/defines.h"
#include "../utils/code.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

Header create_header(uint32_t input_file_size, uint64_t char_freqs[MAX_CODE_NUM]) {
    Header h;

    h.input_file_size = input_file_size;
    h.frequencies_num = 0;
    h.table = filter_non_zero(char_freqs, &h.frequencies_num);

    return h;
}

void print_header(Header *h) {
    if (!h) {
        return;
    }

    printf("[ + ] Input file size: %u\n", h->input_file_size);
    printf("[ + ] Number different letters met in the text: %u\n", h->frequencies_num);
    printf("[ + ] Frequency table:\n");
    for (uint8_t i = 0; i < h->frequencies_num; i++) {
        printf("ASCII: %u ", h->table[i].byte);
        printb(h->table[i].byte, BYTE_LEN);
        printf(" Frequency: %u\n", h->table[i].frequency);
    }

    printf("\n");
}

uint32_t get_file_size(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Error seeking file");
        fclose(file);
        return 0;
    }

    uint32_t size = ftell(file);
    if (size == -1) {
        perror("Error getting file size");
        fclose(file);
        return 0;
    }

    fclose(file);

    return size;
};

Frequency* filter_non_zero(uint64_t chars_freqs[MAX_CODE_NUM], uint8_t *size) {
    int count = 0;
    for (int i = 0; i < MAX_CODE_NUM; i++) {
        if (chars_freqs[i] != 0) {
            count++;
        }
    }
    
    Frequency *frequencies = (Frequency*) calloc(count, sizeof(Frequency));
    if (frequencies == NULL) {
        *size = 0;
        return NULL;    }
    
    uint32_t index = 0;
    for (int i = 0; i < MAX_CODE_NUM; i++) {
        if (chars_freqs[i] > 0) {
            frequencies[index].byte = (uint8_t)i;
            frequencies[index].frequency = (uint32_t)chars_freqs[i];
            index++;
        }
    }

    *size = count;

    return frequencies;
}

bool get_all_char_freqs(Header *h, uint64_t char_freqs[MAX_CODE_NUM]) {
    if (!h) {
        return false;
    }

    if (!char_freqs) {
        return false;
    }

    for (uint32_t i = 0; i < h->frequencies_num; i++) {
        uint8_t byte = h->table[i].byte;
        char_freqs[byte] = h->table[i].frequency;
    }

    return true;
}

void write_header(Header h, const char *output_file){
    printf("[ + ] Writing header\n");
    FILE *f = fopen(output_file, "wb");
    if (f == NULL) {
        return;
    }
    
    fwrite(&h.input_file_size, sizeof(uint32_t), 1, f);
    fwrite(&h.frequencies_num, sizeof(uint8_t), 1, f);
    fwrite(h.table, sizeof(h.table) * h.frequencies_num, 1, f);

    fclose(f);
}

bool read_header(const char *compressed_file, Header *h, uint32_t *offset) {
    printf("[ + ] Reading Header\n");
    if (!h) {
        perror("Header is not initialized");
    }

    FILE *file = fopen(compressed_file, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return false;
    }

    *offset += sizeof(uint32_t);
    if (fread(&h->input_file_size, sizeof(uint32_t), 1, file) != 1) {
        perror("Error reading input file size");
        fclose(file);
        return false;
    }

    *offset += sizeof(uint8_t);
    if (fread(&h->frequencies_num, sizeof(uint8_t), 1, file) != 1) {
        perror("Error reading number of frequencies");
        fclose(file);
        return 1;
    }


    h->table = (Frequency *)malloc(h->frequencies_num * sizeof(Frequency));
    if (h->table == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return false;
    }

    *offset += h->frequencies_num * sizeof(Frequency);
    if (fread(h->table, sizeof(Frequency), h->frequencies_num, file) != h->frequencies_num) {
        perror("Error reading frequency table");
        fclose(file);
        free(h->table);
        return false;
    }

    fclose(file);

    return true;
}

void free_header(Header *h) {
    if (!h) {
        return;
    }

    free(h->table);
}
