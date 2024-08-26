#ifndef NODE_H
#define NODE_H

#include <inttypes.h>
#include <stdint.h>

extern uint32_t symbols;

typedef struct Node Node;

struct Node {
    Node *left;
    Node *right;
    uint8_t symbol;
    uint64_t frequency;
};

Node* node_create(char symbol, uint32_t frequency);

Node* create_parent(Node *left, Node *right);

void free_node(Node **node);

void free_tree(Node **root);

#endif