#ifndef DLIST_H
#define DLIST_H

#include <stdlib.h>

/* Node in doubly linked list */
typedef struct dlist_node_t {
    struct dlist_node_t *prev;
    struct dlist_node_t *next;
    void *data;
} dlist_node_t;

/* Doubly linked list */
typedef struct dlist_t {
    dlist_node_t *head;
    dlist_node_t *tail;
    size_t count;
} dlist_t;

/**
 * Insert element into doubly linked list
 * @param data Pointer to element data
 * @param at_tail boolean, whether to insert at the tail or head
 * @return List pointer
 */
dlist_t* dlist_insert(dlist_t *dlist, void *data, int at_tail);

/**
 * Add element to tail (end) of doubly linked list
 * @param data Pointer to element data
 * @return List pointer
 */
dlist_t* dlist_add_tail(dlist_t *dlist, void *data);

/**
 * Add element to tail (end) of doubly linked list
 * @param data Pointer to element data
 * @return List pointer
 */
dlist_t* dlist_add(dlist_t *dlist, void *data);

/**
 * Removes node from doubly linked list (does not free data, only node itself)
 * @param dlist Doubly linked list
 * @param node Node to be removed
 * @return Data from removed node
 */
void* dlist_remove(dlist_t *dlist, dlist_node_t *node);

/**
 * Removes first node (head) from doubly linked list (does not free data, only node itself)
 * @param dlist Doubly linked list
 * @return Data from removed node
 */
void* dlist_remove_head(dlist_t *dlist);

/**
 * Removes last node (tail) from doubly linked list (does not free data, only node itself)
 * @param dlist Doubly linked list
 * @return Data from removed node
 */
void* dlist_remove_tail(dlist_t *dlist);

/**
 * Free doubly linked list (does not free node data)
 * @param dlist Doubly linked list
 */
void dlist_free(dlist_t *dlist);

/**
 * Free doubly linked list and node data
 * @param dlist Doubly linked list
 * @param free_node_data function used to free node data
 */
void dlist_free_all(dlist_t *dlist, void free_node_data(void* node_data));

#endif