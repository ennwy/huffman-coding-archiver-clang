#include "node.h"

#include <stdio.h>
#include <stdlib.h>

Node* node_create(char symbol, uint32_t frequency) {
    Node* node = (Node*)malloc(sizeof(Node));

    node->symbol = symbol;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;

    return node;
}

Node* create_parent(Node *left, Node *right) {
    Node *parent = node_create('$', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;

    return parent;
}

void free_node(Node **node) {
    if (!*node) {
        return;
    }

    free(*node);
    *node = NULL;
}

void free_tree(Node **root) {
    if (!*root) {
        return;
    }

    if ((*root)->left) {
        free_tree(&((*root)->left));
    }

    if ((*root)->right) {
        free_tree(&((*root)->right));
    }

    free_node(root);
}