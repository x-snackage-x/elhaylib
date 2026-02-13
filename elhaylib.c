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

    if(!ptr_head->ptr_first_elem) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
}

char* dynarr_append(dynarr_head* const ptr_head, const void* element) {
    if(ptr_head->dynarr_size == ptr_head->dynarr_capacity) {
        dynarr_expand(ptr_head);
    }

    void* dest = ptr_head->ptr_first_elem +
                 (ptr_head->dynarr_size * ptr_head->elem_size);
    memcpy(dest, element, ptr_head->elem_size);

    ++ptr_head->dynarr_size;

    return ptr_head->ptr_first_elem;
}

void dynarr_free(dynarr_head* const ptr_head) {
    free(ptr_head->ptr_first_elem);
    ptr_head->dynarr_capacity = 0;
    ptr_head->dynarr_size = 0;
    ptr_head->ptr_first_elem = NULL;
}

char* dynarr_insert(dynarr_head* const ptr_head,
                    void const* element,
                    size_t insert_index) {
    assert(insert_index <= ptr_head->dynarr_size &&
           "Inserts must be within current Dyn-Array bounds.");

    if(ptr_head->dynarr_size == ptr_head->dynarr_capacity) {
        dynarr_expand(ptr_head);
    }

    void* insert_point =
        ptr_head->ptr_first_elem + insert_index * ptr_head->elem_size;
    void* shift_point =
        ptr_head->ptr_first_elem + (insert_index + 1) * ptr_head->elem_size;
    size_t size_move =
        (ptr_head->dynarr_size - insert_index) * ptr_head->elem_size;
    memmove(shift_point, insert_point, size_move);

    memcpy(insert_point, element, ptr_head->elem_size);

    ++ptr_head->dynarr_size;

    return ptr_head->ptr_first_elem;
}

void dynarr_remove(dynarr_head* const ptr_head, size_t index) {
    dynarr_remove_n(ptr_head, index, 1);
}

void dynarr_remove_n(dynarr_head* const ptr_head,
                     size_t index,
                     size_t n_elements) {
    assert(index <= ptr_head->dynarr_size &&
           "Index must be within current Dyn-Array bounds.");
    assert(
        index + n_elements <= ptr_head->dynarr_size &&
        "All elements to be removed must be within current Dyn-Array bounds.");

    void* start_point = ptr_head->ptr_first_elem + index * ptr_head->elem_size;
    void* end_point =
        ptr_head->ptr_first_elem + (index + n_elements) * ptr_head->elem_size;
    size_t size_move =
        (ptr_head->dynarr_size - index - n_elements) * ptr_head->elem_size;

    memmove(start_point, end_point, size_move);

    ptr_head->dynarr_size -= n_elements;
}

// internals
void dynarr_expand(dynarr_head* const ptr_head) {
    size_t new_capacity =
        (size_t)(ptr_head->dynarr_capacity * ptr_head->growth_fac);
    void* new_ptr =
        realloc(ptr_head->ptr_first_elem, new_capacity * ptr_head->elem_size);

    if(!new_ptr) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
    ptr_head->ptr_first_elem = new_ptr;
    ptr_head->dynarr_capacity = new_capacity;
}

// LINKED LIST
void linlst_init(linked_list_head* const ptr_head) {
    ptr_head->list_len = 0;

    char zero = 0;
    list_node* sentinel_node_ptr =
        linlst_prepare_node(NODE_CHAR, sizeof(char), &zero);
    sentinel_node_ptr->previous_node = sentinel_node_ptr;
    sentinel_node_ptr->next_node = sentinel_node_ptr;

    ptr_head->ptr_first_node = sentinel_node_ptr;
    ptr_head->ptr_sentinel_node = sentinel_node_ptr;
}

void linlst_append_node(linked_list_head* const ptr_head,
                        node_type dtype,
                        size_t data_size,
                        void const* data) {
    list_node* new_node_ptr = linlst_prepare_node(dtype, data_size, data);
    list_node* ptr_sentinel = ptr_head->ptr_sentinel_node;
    list_node* old_last_node = ptr_sentinel->previous_node;

    if(ptr_head->ptr_first_node == ptr_sentinel) {
        ptr_head->ptr_first_node = new_node_ptr;
    }

    new_node_ptr->next_node = ptr_sentinel;
    new_node_ptr->previous_node = old_last_node;
    ptr_sentinel->previous_node = new_node_ptr;
    old_last_node->next_node = new_node_ptr;

    ptr_head->list_len++;
}

void linlst_prepend_node(linked_list_head* const ptr_head,
                         node_type dtype,
                         size_t data_size,
                         void const* data) {
    list_node* new_node_ptr = linlst_prepare_node(dtype, data_size, data);
    list_node* ptr_sentinel = ptr_head->ptr_sentinel_node;

    new_node_ptr->next_node = ptr_head->ptr_first_node;
    new_node_ptr->previous_node = ptr_sentinel;

    ptr_head->ptr_first_node->previous_node = new_node_ptr;
    ptr_sentinel->next_node = new_node_ptr;
    ptr_head->ptr_first_node = new_node_ptr;

    ptr_head->list_len++;
}

void linlst_insert_node(linked_list_head* const ptr_head,
                        list_node* const pre_node,
                        node_type dtype,
                        size_t data_size,
                        void const* data) {
    list_node* new_node_ptr = linlst_prepare_node(dtype, data_size, data);

    new_node_ptr->previous_node = pre_node;
    new_node_ptr->next_node = pre_node->next_node;
    pre_node->next_node = new_node_ptr;
    new_node_ptr->next_node->previous_node = new_node_ptr;

    ptr_head->list_len++;
}

list_node* linlst_next_circular(linked_list_head* const ptr_head,
                                list_node* cur_node) {
    cur_node = cur_node->next_node;
    return (cur_node == ptr_head->ptr_sentinel_node) ? cur_node->next_node
                                                     : cur_node;
}

list_node* linlst_prev_circular(linked_list_head* const ptr_head,
                                list_node* cur_node) {
    cur_node = cur_node->previous_node;
    return (cur_node == ptr_head->ptr_sentinel_node) ? cur_node->previous_node
                                                     : cur_node;
}

bool linlst_index_insert_node(linked_list_head* const ptr_head,
                              size_t insert_index,
                              node_type dtype,
                              size_t data_size,
                              void const* data) {
    if(insert_index > ptr_head->list_len) {
        return false;
    }

    linlst_index_insert_clamped(ptr_head, insert_index, dtype, data_size, data);
    return true;
}

void linlst_index_insert_clamped(linked_list_head* const ptr_head,
                                 size_t insert_index,
                                 node_type dtype,
                                 size_t data_size,
                                 void const* data) {
    if(insert_index == 0) {
        linlst_prepend_node(ptr_head, dtype, data_size, data);
        return;
    } else if(insert_index >= ptr_head->list_len) {
        linlst_append_node(ptr_head, dtype, data_size, data);
        return;
    }

    list_node_return found_node_buffer = {0};
    linlst_get_node(ptr_head, &found_node_buffer, insert_index);
    linlst_insert_node(ptr_head,
                       found_node_buffer.found_node_ptr->previous_node, dtype,
                       data_size, data);
}

void linlst_get_node(linked_list_head* const ptr_head,
                     list_node_return* found_node_struct,
                     uint64_t index) {
    if(ptr_head->list_len == 0 || ptr_head->list_len - 1 < index) {
        found_node_struct->found_node_ptr = NULL;
        found_node_struct->node_found = false;
        return;
    }

    found_node_struct->node_found = true;
    if(index == 0 && ptr_head->list_len != 0) {
        found_node_struct->found_node_ptr = ptr_head->ptr_first_node;
        return;
    }
    if(index == ptr_head->list_len - 1) {
        found_node_struct->found_node_ptr =
            ptr_head->ptr_sentinel_node->previous_node;
        return;
    }

    found_node_struct->found_node_ptr = ptr_head->ptr_first_node;
    for(uint64_t i = 1; i <= index; ++i) {
        found_node_struct->found_node_ptr =
            (found_node_struct->found_node_ptr)->next_node;
    }
}

void linlst_delete_node(linked_list_head* const ptr_head,
                        list_node* const node) {
    if(node == ptr_head->ptr_sentinel_node) {
        return;
    }

    if(node == ptr_head->ptr_first_node) {
        ptr_head->ptr_first_node = node->next_node;
    }

    list_node* pre_node = node->previous_node;
    list_node* post_node = node->next_node;

    pre_node->next_node = post_node;
    post_node->previous_node = pre_node;

    free(node);
    if(--ptr_head->list_len == 0) {
        ptr_head->ptr_first_node = ptr_head->ptr_sentinel_node;
    }
}

void linlst_index_delete_node(linked_list_head* const ptr_head,
                              size_t delete_index) {
    list_node_return found_node_buffer = {0};
    linlst_get_node(ptr_head, &found_node_buffer, delete_index);
    if(found_node_buffer.node_found) {
        linlst_delete_node(ptr_head, found_node_buffer.found_node_ptr);
    }
}

void linlst_delete_list(linked_list_head* const ptr_head) {
    list_node* cur_node = ptr_head->ptr_first_node;
    while(cur_node != ptr_head->ptr_sentinel_node) {
        list_node* node_next = cur_node->next_node;
        free(cur_node);
        cur_node = node_next;
    }

    list_node* sentinel_node_ptr = ptr_head->ptr_sentinel_node;
    sentinel_node_ptr->previous_node = sentinel_node_ptr;
    sentinel_node_ptr->next_node = sentinel_node_ptr;
    ptr_head->ptr_first_node = sentinel_node_ptr;
    ptr_head->list_len = 0;
}

// internals
list_node* linlst_prepare_node(node_type dtype,
                               size_t data_size,
                               void const* data) {
    list_node* new_node_ptr = calloc(1, sizeof(list_node) + data_size);
    if(!new_node_ptr) {
        perror("calloc failed");
        exit(EXIT_FAILURE);
    }

    new_node_ptr->data_size = data_size;
    new_node_ptr->dtype = dtype;
    memcpy(new_node_ptr->data, data, data_size);

    return new_node_ptr;
}

// STACK
stack_head* stack_init() {
    stack_head* stack_head = calloc(1, sizeof(stack_head));
    if(!stack_head) {
        perror("calloc failed");
        exit(EXIT_FAILURE);
    }

    linlst_init(stack_head->impl_list);

    return stack_head;
}

bool stack_pop(stack_head* stack_head, void* data) {
    if(stack_head->impl_list->ptr_first_node ==
       stack_head->impl_list->ptr_sentinel_node) {
        return false;
    }

    list_node* ptr_top =
        stack_head->impl_list->ptr_sentinel_node->previous_node;
    size_t data_size = ptr_top->data_size;
    memcpy(data, ptr_top->data, data_size);
    linlst_delete_node(stack_head->impl_list, ptr_top);

    return true;
}

bool stack_peek(stack_head* stack_head, void* data) {
    if(stack_head->impl_list->ptr_first_node ==
       stack_head->impl_list->ptr_sentinel_node) {
        return false;
    }

    list_node* ptr_top =
        stack_head->impl_list->ptr_sentinel_node->previous_node;
    size_t data_size = ptr_top->data_size;
    memcpy(data, ptr_top->data, data_size);

    return true;
}

void stack_push(stack_head* stack_head,
                node_type dtype,
                size_t data_size,
                void const* data) {
    linlst_append_node(stack_head->impl_list, dtype, data_size, data);
}

void stack_free(stack_head* stack_head) {
    linlst_delete_list(stack_head->impl_list);
    free(stack_head);
}

// TREE
void tree_init(tree_head* const ptr_head) {
    ptr_head->tree_size = 0;
    ptr_head->in_order_partition = 0.5f;
    ptr_head->tree_root = NULL;
}
void tree_define_inorder_partition(tree_head* const ptr_head,
                                   float inorder_partition) {
    ptr_head->in_order_partition = inorder_partition;
}

void tree_node_root(tree_head* const ptr_head,
                    node_type dtype,
                    size_t data_size,
                    void const* data) {
    tree_node* new_node_ptr = tree_prepare_node(dtype, data_size, data);
    ptr_head->tree_root = new_node_ptr;
    ptr_head->tree_size++;
}

void tree_node_add(tree_head* const ptr_head,
                   tree_node* const ptr_parent,
                   node_type dtype,
                   size_t data_size,
                   void const* data) {
    tree_node* new_node_ptr = tree_prepare_node(dtype, data_size, data);
    new_node_ptr->parent = ptr_parent;

    linked_list_head* siblings_list = ptr_parent->children;
    linlst_append_node(siblings_list, NODE_PTR, sizeof(tree_node*),
                       new_node_ptr);
    ptr_head->tree_size++;
}

void tree_node_add_at_index(tree_head* const ptr_head,
                            tree_node* const ptr_parent,
                            node_type dtype,
                            size_t graft_index,
                            size_t data_size,
                            void const* data) {
    tree_node* new_node_ptr = tree_prepare_node(dtype, data_size, data);
    new_node_ptr->parent = ptr_parent;

    linked_list_head* siblings_list = ptr_parent->children;
    linlst_index_insert_node(siblings_list, graft_index, NODE_PTR,
                             sizeof(tree_node*), new_node_ptr);
    ptr_head->tree_size++;
}
// internals
tree_node* tree_prepare_node(node_type dtype,
                             size_t data_size,
                             void const* data) {
    tree_node* new_node_ptr = calloc(1, sizeof(list_node) + data_size);

    new_node_ptr->data_size = data_size;
    new_node_ptr->dtype = dtype;
    memcpy(new_node_ptr->data, data, data_size);

    linked_list_head* children_linked_list = malloc(sizeof(linked_list_head));
    linlst_init(children_linked_list);
    new_node_ptr->children = children_linked_list;

    return new_node_ptr;
}
