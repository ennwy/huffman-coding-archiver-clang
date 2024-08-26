#include "defines.h"
#include "code.h"
#include "priority_queue.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

Code create_code(char letter, CODE code, uint32_t len) {
    Code c;

    c.letter = letter;
    c.code = code;
    c.len = len;

    return c;
}

uint8_t get_first_bits(Code c, uint8_t bits_n) {
    return c.code >> (c.len - bits_n);
}

uint8_t get_last_bits(Code c, uint8_t bits_n) {
    CODE n = c.code << (CODE_SIZE - bits_n);
    return n >> (CODE_SIZE - bits_n);
}

uint8_t get_first_bit(CODE byte) {
    return (byte & 0b10000000) >> (BYTE_LEN - 1);
}

void printb(uint64_t c, uint8_t len) {
    for (int i = len - 1; i >= 0; i--) {
        printf("%lu", (c >> i) & 1);
    }
}

