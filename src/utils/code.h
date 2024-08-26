#ifndef CODE_H
#define CODE_H

#include "defines.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>

#define CODE uint16_t
#define CODE_SIZE 16

typedef struct Code {
    char letter;
    CODE code;
    uint32_t len;  
} Code;

Code create_code(char letter, CODE code, uint32_t len);

uint8_t get_first_bits(Code c, uint8_t bits_n);

uint8_t get_last_bits(Code c, uint8_t bits_n);

uint8_t get_first_bit(CODE byte);

void printb(uint64_t c, uint8_t len);

#endif