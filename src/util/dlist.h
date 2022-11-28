#ifndef DLIST_H
#define DLIST_H

#include <stdlib.h>

typedef struct dlist_node_t {
    struct dlist_node_t *prev;
    struct dlist_node_t *next;
    void *data;
} dlist_node_t;

typedef struct dlist_t {
    dlist_node_t *head;
    dlist_node_t *tail;
    size_t count;
} dlist_t;

dlist_t *dlist_new();

dlist_t* dlist_insert(dlist_t *dlist, void *data, int at_tail);
dlist_t* dlist_add_tail(dlist_t *dlist, void *data);
dlist_t* dlist_add(dlist_t *dlist, void *data);

void* dlist_remove(dlist_t *dlist, dlist_node_t *node);
void* dlist_remove_head(dlist_t *dlist);
void* dlist_remove_tail(dlist_t *dlist);

void dlist_free(dlist_t *dlist);

#endif