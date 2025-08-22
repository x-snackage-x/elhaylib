#include "elhaylib.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DYNAMIC ARRAY
void dynarr_init(dynarr_head* const ptr_head) {
    assert(ptr_head->elem_size > 0 && "Element size must be greater zero.");
    if(ptr_head->dynarr_capacity <= 0) {
        ptr_head->dynarr_capacity = 10;
    }

    if(ptr_head->growth_fac <= 1.0f) {
        ptr_head->growth_fac = 2.0f;
    }

    ptr_head->dynarr_size = 0;
    ptr_head->ptr_first_elem =
        calloc(ptr_head->dynarr_capacity, ptr_head->elem_size);
}

void dynarr_push(dynarr_head* const ptr_head, const void* element) {
    if(ptr_head->dynarr_size == ptr_head->dynarr_capacity) {
        dynarr_expand(ptr_head);
    }

    memcpy(ptr_head->ptr_first_elem +
               (ptr_head->dynarr_size * ptr_head->elem_size),
           element, ptr_head->elem_size);
    ++ptr_head->dynarr_size;
}

void dynarr_free(dynarr_head* const ptr_head) {
    free(ptr_head->ptr_first_elem);
    ptr_head->dynarr_size = 0;
    ptr_head->ptr_first_elem = NULL;
}

// internals
void dynarr_expand(dynarr_head* const ptr_head) {
    ptr_head->dynarr_capacity =
        (size_t)(ptr_head->dynarr_capacity * ptr_head->growth_fac);
    void* new_ptr = realloc(ptr_head->ptr_first_elem,
                            ptr_head->dynarr_capacity * ptr_head->elem_size);
    if(!new_ptr) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
    ptr_head->ptr_first_elem = new_ptr;
}

// LINKED LIST
void linlst_init(linked_list_head* const ptr_head) {
    assert(ptr_head->list_type > 0 && "List-type must be defined");
    ptr_head->list_len = 0;
    ptr_head->ptr_first_node = NULL;
    ptr_head->ptr_last_node = NULL;
}

void linlist_append_node(linked_list_head* const ptr_head,
                         size_t data_size,
                         const void const* data) {
    list_node* new_node_ptr = calloc(1, sizeof(list_node));
    new_node_ptr->data = calloc(1, data_size);
    memcpy(new_node_ptr->data, data, data_size);

    ptr_head->ptr_last_node = new_node_ptr;
    if(ptr_head->list_len == 0) {
        ptr_head->list_len++;
        ptr_head->ptr_first_node = new_node_ptr;

        switch(ptr_head->list_type) {
            case CIRCULAR:
                new_node_ptr->next_node = new_node_ptr;
                new_node_ptr->previous_node = new_node_ptr;
                break;
            case OPEN:
                break;
        }
        return;
    }

    list_node* old_last_node_ptr = ptr_head->ptr_last_node;
    old_last_node_ptr->next_node = new_node_ptr;
    new_node_ptr->previous_node = old_last_node_ptr;

    switch(ptr_head->list_type) {
        case CIRCULAR:
            new_node_ptr->next_node = ptr_head->ptr_first_node;
            ptr_head->ptr_last_node = new_node_ptr;
            break;
        case OPEN:
            break;
    }

    ptr_head->list_len++;
}