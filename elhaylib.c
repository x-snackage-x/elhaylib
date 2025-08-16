#include "elhaylib.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DYNAMIC ARRAY
void dynarr_init(dynarr_head* ptr_head) {
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

void dynarr_push(dynarr_head* ptr_head, const void* element) {
    if(ptr_head->dynarr_size == ptr_head->dynarr_capacity) {
        dynarr_expand(ptr_head);
    }

    memcpy(ptr_head->ptr_first_elem +
               (ptr_head->dynarr_size * ptr_head->elem_size),
           element, ptr_head->elem_size);
    ++ptr_head->dynarr_size;
}

void dynarr_free(dynarr_head* ptr_head) {
    free(ptr_head->ptr_first_elem);
    ptr_head->dynarr_size = 0;
    ptr_head->ptr_first_elem = NULL;
}

// internals
void dynarr_expand(dynarr_head* ptr_head) {
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
