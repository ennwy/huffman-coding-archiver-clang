#ifndef PQ_H
#define PQ_H

#include "node.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct PriorityQueue {
    uint32_t size;      
    uint32_t cap;  
    Node** nodes;
};

typedef struct PriorityQueue PriorityQueue;

PriorityQueue *pq_create(uint32_t cap);

static inline uint32_t pq_get_parent(uint32_t index);

static inline uint32_t pq_get_left_child(uint32_t index);

static inline uint32_t pq_get_right_child(uint32_t index);

static inline void swap(PriorityQueue *q, uint32_t i, uint32_t j);

static inline bool pq_is_full(PriorityQueue *pq);

static inline void pq_sort(PriorityQueue *q, uint32_t pos);

bool enqueue(PriorityQueue *q, Node *n);

bool dequeue(PriorityQueue *q, Node **n);

void pq_free(PriorityQueue **pq);

#endif