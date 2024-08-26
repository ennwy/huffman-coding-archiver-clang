#include "../utils/node.h"
#include "../utils/code.h"
#include "../utils/priority_queue.h"
#include "../utils/defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


Node *build_tree(uint64_t char_freqs[static MAX_CODE_NUM]) {
    printf("[ + ] Building huffman tree\n");
    Node *root = node_create('$', 0);
    Node *left = node_create('$', 0);
    Node *right = node_create('$', 0);

    PriorityQueue *pq = pq_create(MAX_CODE_NUM);

    for (uint32_t i = 0; i < MAX_CODE_NUM; i++) {
        if (char_freqs[i] > 0) {
            Node *n = node_create(i, char_freqs[i]);
            enqueue(pq, n);
        }
    }

    if (pq->size == 1) {
        dequeue(pq, &left);
        root->left = left;
    }

    while (pq->size > 1) {
        dequeue(pq, &left);
        dequeue(pq, &right);

        Node *n = create_parent(left, right);
        enqueue(pq, n);
    }

    dequeue(pq, &root);
    pq_free(&pq);

    return root;
}


void build_code_by_symbol_table(Node *root, Code table[MAX_CODE_NUM], CODE code, uint32_t len) {
    if (root == NULL) {
        return;
    }

    if (root->left == NULL && root->right == NULL) {
        table[(uint32_t)root->symbol] = create_code(root->symbol, code, len);
    }

    build_code_by_symbol_table(root->left, table, (code << 1) | 0, len + 1);
    build_code_by_symbol_table(root->right, table, (code << 1) | 1, len + 1);
}

bool replace_with_codes(const char* input_file, const char* output_file, const Code table[MAX_CODE_NUM]) {
    printf("[ + ] Writing output\n");
    FILE* input = fopen(input_file, "r");
    if (input == NULL) {
        perror("Error opening input file");
        return false;
    }

    if (fseek(input, 0, SEEK_SET) != 0) {
        perror("error seeking in file");
        fclose(input);
        return 1;
    }

    FILE* output = fopen(output_file, "ab");
    if (output == NULL) {
        perror("Error opening output file");
        fclose(input);
        return false;
    }

    uint32_t ch;
    Code c;

    uint32_t sum = 0;
    const uint8_t sum_size = 32;

    uint8_t bits_left = sum_size;

    while ((ch = fgetc(input)) != EOF) {
        if (ch < 0 || ch >= 256 || table[ch].len < 1) {
            continue;
        }

        c = table[ch];

        sum |= ((uint32_t)c.code) << (bits_left - c.len);
        bits_left -= c.len;

        while (bits_left <= (sum_size - BYTE_LEN)) {
            fputc(sum >> (sum_size - BYTE_LEN), output);

            bits_left += BYTE_LEN;
            sum <<= BYTE_LEN;
        }
    }

    while (bits_left < sum_size) {
        fputc(sum >> (sum_size - BYTE_LEN), output);

        bits_left += BYTE_LEN;
        sum <<= BYTE_LEN;
    }

    fclose(input);
    fclose(output);

    return true;
}

bool replace_with_chars(
    const char* compressed_file,
    const char* output_file,
    Node *root,
    uint32_t data_offset,
    uint32_t original_text_size
    ) {
    FILE* input = fopen(compressed_file, "rb");
    if (input == NULL) {
        perror("Error opening input file");
        return false;
    }

    if (fseek(input, data_offset, SEEK_SET) != 0) {
        perror("error seeking in file");
        fclose(input);
        return 1;
    }

    FILE* output = fopen(output_file, "w");
    if (output == NULL) {
        perror("error opening output file");
        fclose(input);
        fclose(output);
        return false;
    }

    printf("[ + ] Reading input file started\n");

    uint32_t total_bytes_read = 0;
    uint32_t byte = 0;
    Node *node = root;

    bool read_all_flag = false;

    while (!read_all_flag && (byte = fgetc(input)) != EOF) {
        if (byte < 0 || byte >= 256) {
            printf("Byte is not in bounds: continue!\n");
            return false;
        }

        for (uint32_t i = 0; i < BYTE_LEN; i++) {
            uint8_t bit = get_first_bit(byte);
            byte <<= 1;

            if (bit == 0) {
                if (node->left == NULL) {
                    perror("path is broken. left node is null");
                    fclose(input);
                    fclose(output);
                    return false;
                }
                node = node->left;

            } else if (bit == 1) {
                if (node->right == NULL) {
                    perror("path is broken. left node is null");
                    fclose(input);
                    fclose(output);
                    return false;
                }
                node = node->right;

            } else {
                perror("bit neither equals 1, nor 0. unexpected value error");
                return false;
            }

            if (node->left != NULL && node->right != NULL) {
                continue; 
            }

            fputc(node->symbol, output);
            node = root;

            if (++total_bytes_read >= original_text_size) {
                read_all_flag = true;
                break;
            }
        }
    }

    printf("[ + ] Reading input file ended\n");
    fclose(input);
    fclose(output);

    return true;
}