#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "../utils/node.h"
#include "../utils/code.h"

#include <stdint.h>

Node *build_tree(uint64_t hist[static MAX_CODE_NUM]);

void build_code_by_symbol_table(Node *root, Code table[MAX_CODE_NUM], CODE code, uint32_t len);

bool replace_with_codes(const char* input_file, const char* output_file, const Code table[MAX_CODE_NUM]);

bool replace_with_chars(
    const char* compressed_file,
    const char* output_file,
    Node *root,
    uint32_t data_offset,
    uint32_t original_text_size
    );

#endif