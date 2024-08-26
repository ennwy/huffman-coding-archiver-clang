#include "utils/defines.h"
#include "utils/node.h"
#include "utils/code.h"
#include "utils/frequency.h"
#include "utils/code.h"
#include "huffman/huffman.h"
#include "huffman/header.h"

#include <stdio.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    const char* input_file_name = argv[1];
    const char* output_file_name = argv[2];

    uint64_t char_freqs[MAX_CODE_NUM] = { 0 };
    bool ok = get_chars_frequency(input_file_name, char_freqs);
    if (!ok) {
        return 1;
    }

    uint32_t file_size = get_file_size(input_file_name);

    Header h = create_header(file_size, char_freqs);

    write_header(h, output_file_name);

    Node *root = build_tree(char_freqs);
    if (root == NULL) {
        perror("Error building Huffman tree\n");
        return 1;
    }

    Code table[MAX_CODE_NUM] = { 0 };
    printf("[ + ] Generating Huffman codes\n");
    build_code_by_symbol_table(root, table, 0, 0);

    ok = replace_with_codes(input_file_name, output_file_name, table);
    if (!ok) {
        perror("error encodeing the file");
        return 1;
    }

    printf("Encoding completed successfully!\n");

    free_tree(&root);
    free_header(&h);

    return 0;
}
