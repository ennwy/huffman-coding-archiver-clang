#include "defines.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

bool get_chars_frequency(const char* input_file, uint64_t char_freqs[MAX_CODE_NUM]) {
    printf("[ + ] Counting bytes frequencies\n");

    FILE* file = fopen(input_file, "r");
    if (file == NULL) {
        perror("Error opening input file, getting char frequency");
        return false;
    }

    uint32_t c;
    while ((c = fgetc(file)) != EOF) {
        if (c >= 0 && c < 256) {
            char_freqs[c]++;
        }
    }

    fclose(file);
    return true;
}