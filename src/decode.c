#include "utils/node.h"
#include "huffman/header.h"
#include "huffman/huffman.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    bool ok = false;

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    Header h;
    uint32_t offset = 0;

    ok = read_header(input_file, &h, &offset);
    if (!ok) {
        fprintf(stderr, "error reading header\n");
        return 1;
    }

    uint64_t char_freqs[MAX_CODE_NUM] = { 0 };
    ok = get_all_char_freqs(&h, char_freqs);
    if (!ok) {
        perror("unexpected error");
        return 1;
    }

    Node *root = build_tree(char_freqs);
    if (root == NULL) {
        perror("error building huffman tree");
        return 1;
    }

    replace_with_chars(input_file, output_file, root, offset, h.input_file_size);

    printf("Decoding completed successfully!\n");

    free_tree(&root);
    free_header(&h);

    return 0;
}
