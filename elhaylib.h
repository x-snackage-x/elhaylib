#include <inttypes.h>
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

void dynarr_init(dynarr_head* ptr_head);
void dynarr_push(dynarr_head* ptr_head, const void* element);
void dynarr_free(dynarr_head* ptr_head);
// internals
void dynarr_expand(dynarr_head* ptr_head);

// LINKED LIST
typedef enum {
    SINGLE_OPEN,
    SINGLE_CIRCULAR,
    DOUBLE_OPEN,
    DOUBLE_CIRCULAR,
} list_type;
typedef struct {
    list_type list_type;
    size_t elem_size;
    size_t list_len;
    char* ptr_first_elem;
} linked_list_head;

typedef struct {
    char* data;
    struct list_node* next_node;
    struct list_node* previous_node;
} list_node;