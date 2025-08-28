#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum { OK } ERROR_CODES;

// DYNAMIC ARRAY
typedef struct {
    size_t elem_size;
    size_t dynarr_size;
    size_t dynarr_capacity;
    float growth_fac;
    char* ptr_first_elem;
} dynarr_head;

void dynarr_init(dynarr_head* const ptr_head);
void dynarr_push(dynarr_head* const ptr_head, void const* element);
void dynarr_free(dynarr_head* const ptr_head);
// internals
void dynarr_expand(dynarr_head* const ptr_head);

// LINKED LIST
typedef enum { OPEN = 1, CIRCULAR } list_type;
typedef struct list_node list_node;
typedef struct {
    list_type list_type;
    size_t list_len;
    list_node* ptr_first_node;
    list_node* ptr_last_node;
} linked_list_head;

struct list_node {
    char* data;
    list_node* next_node;
    list_node* previous_node;
};

typedef struct {
    bool node_found;
    list_node* found_node_ptr;
} list_node_return;

void linlst_init(linked_list_head* const ptr_head);
void linlist_append_node(linked_list_head* const ptr_head,
                         size_t data_size,
                         void const* data);
void linlist_prepend_node(linked_list_head* const ptr_head,
                          size_t data_size,
                          void const* data);
void linlist_insert_node(linked_list_head* const ptr_head,
                         list_node* const pre_node,
                         size_t data_size,
                         void const* data);
void linlist_get_node(linked_list_head* const ptr_head,
                      list_node_return* found_node_struct,
                      uint8_t index);
void linlist_delete_node(linked_list_head* const ptr_head,
                         list_node* const node);
void linlist_delete_list(linked_list_head* const ptr_head);
bool linlist_is_empty(linked_list_head* const ptr_head);
