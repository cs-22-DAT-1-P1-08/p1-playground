#include "dlist.h"
#include <stdlib.h>

dlist_t* dlist_insert(dlist_t *dlist, void *data, int at_tail) {
    dlist_node_t *node = calloc(1, sizeof(dlist_node_t));
    node->data = data;

    if (dlist->head == NULL) {
        dlist->head = node;
        dlist->tail = node;
        dlist->count = 1;

        return dlist;
    }

    if (at_tail) {
        dlist->head->prev = node;
        node->next = dlist->head;
        dlist->head = node;
    } else {
        dlist->tail->next = node;
        node->prev = dlist->tail;
        dlist->tail = node;
    }
    dlist->count++;

    return dlist;
}

void* dlist_remove(dlist_t *dlist, dlist_node_t *node) {
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL)
        node->next->prev = node->prev;
    if (dlist->head == node)
        dlist->head = node->next;
    if (dlist->tail == node) // node can be head and tail at the same time
        dlist->tail = node->next;

    void* node_data = node->data;
    free(node);

    dlist->count--;
    return node_data;
}

void* dlist_remove_head(dlist_t *dlist) {
    return dlist_remove(dlist, dlist->head);
}

void* dlist_remove_tail(dlist_t *dlist) {
    return dlist_remove(dlist, dlist->tail);
}

dlist_t* dlist_add_tail(dlist_t *dlist, void *data) {
    return dlist_insert(dlist, data, 1);
}

dlist_t* dlist_add(dlist_t *dlist, void *data) {
    return dlist_insert(dlist, data, 1);
}

dlist_t* dlist_add_head(dlist_t *dlist, void *data) {
    return dlist_insert(dlist, data, 0);
}

void dlist_free(dlist_t *dlist) {
    if (dlist == NULL)
        return;

    dlist_node_t *node = dlist->head;
    while (node != NULL) {
        dlist_node_t  *next = node->next;
        free(node);
        node = next;
    }

    free(dlist);
}