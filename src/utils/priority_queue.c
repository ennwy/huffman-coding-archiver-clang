#include "node.h"
#include "priority_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

PriorityQueue *pq_create(uint32_t cap) {
    PriorityQueue *pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    assert(pq);

    pq->size = 0;
    pq->cap = cap;

    pq->nodes = (Node**)calloc(cap, sizeof(Node*));
    assert(pq->nodes);

    return pq;
}

static inline uint32_t pq_get_parent(uint32_t index) {
    return (index - 1) / 2;
}

static inline uint32_t pq_get_left_child(uint32_t index) {
    return (index * 2) + 1;
}

static inline uint32_t pq_get_right_child(uint32_t index) {
    return (index * 2) + 2;
}

static inline void swap(PriorityQueue *q, uint32_t i, uint32_t j) {
    Node *temp = q->nodes[i];
    q->nodes[i] = q->nodes[j];
    q->nodes[j] = temp;
    return;
}


static inline bool pq_is_full(PriorityQueue *pq) {
    return pq->size == pq->cap;
}

static inline void pq_sort(PriorityQueue *q, uint32_t pos) {
    uint32_t l = pq_get_left_child(pos);
    uint32_t r = pq_get_right_child(pos);
    uint32_t smallest = pos;

    if (l < q->size && q->nodes[smallest]->frequency > q->nodes[l]->frequency) {
        smallest = l;
    }

    if (r < q->size && q->nodes[smallest]->frequency > q->nodes[r]->frequency) {
        smallest = r;
    }
 
    if (pos != smallest) {
        swap(q, smallest, pos);
        pq_sort(q, smallest);
    }

    return;
}

bool enqueue(PriorityQueue *q, Node *n) {
    if (q) {
        if (pq_is_full(q)) {
            return false;
        }

        q->size++;
        uint32_t t = q->size - 1;
        q->nodes[t] = n;

        while (t != 0 && q->nodes[pq_get_parent(t)]->frequency > q->nodes[t]->frequency) {
            swap(q, t, pq_get_parent(t));
            t = pq_get_parent(t);
        }

        return true;
    }

    return false;
}

bool dequeue(PriorityQueue *q, Node **n) {
    if (q) {
        if (q->size == 0) {
            return false;
        }

        if (q->size == 1) {
            q->size--;
            *n = q->nodes[0];
            return true;
        }
        
        *n = q->nodes[0];
        q->nodes[0] = q->nodes[q->size - 1];
        q->size--;

        pq_sort(q, 0);
        return true;
    }

    return false;
}

void pq_free(PriorityQueue **pq) {
    if (!*pq) {
        return;
    }

    for (uint32_t i = 0; i < (*pq)->size; i++) {
        free_tree(&(*pq)->nodes[i]);
    }

    free((*pq)->nodes);
    free(*pq);
    *pq = NULL;

    return;
}
