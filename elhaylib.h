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
void dynarr_push(dynarr_head* const ptr_head, const void const* element);
void dynarr_free(dynarr_head* const ptr_head);
// internals
void dynarr_expand(dynarr_head* const ptr_head);

// LINKED LIST
typedef enum { OPEN = 1, CIRCULAR } list_type;

typedef struct {
    list_type list_type;
    size_t list_len;
    char* ptr_first_node;
    char* ptr_last_node;
} linked_list_head;

typedef struct {
    char* data;
    struct list_node* next_node;
    struct list_node* previous_node;
} list_node;

void linlst_init(linked_list_head* const ptr_head);
void linlist_append_node(linked_list_head* const ptr_head,
                         size_t data_size,
                         const void const* data);
void linlist_insert_node(linked_list_head* const ptr_head,
                         list_node* const pre_node,
                         size_t data_size,
                         const void const* data);
void linlist_delete_node(linked_list_head* const ptr_head,
                         list_node* const node);
void linlist_delete_list(linked_list_head* const ptr_head);
bool linlist_get_node(linked_list_head* const ptr_head,
                      list_node* found_node,
                      int index);
bool linlist_is_empty(linked_list_head* const ptr_head);
