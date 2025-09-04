#include "../elhaylib.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

#define TEST_NAMES_LENGTH 25
#define NO_YAPPING true

typedef enum { STRUCT_1 = 1000, STRUCT_2 = 1001 } my_struct_types;

typedef struct my_test_structure {
    int idx;
    char name[10];
} my_test_structure;

typedef struct my_other_test_structure {
    char gender;
    char name[10];
} my_other_test_structure;

linked_list_head my_list = {.list_type = OPEN};

void print_test_res(bool result) {
    if(result) {
        printf(GRN "S" RESET);
    } else {
        printf(RED "F" RESET);
    }
    printf("\n");
}

bool test_build_linked_list(bool no_yapping) {
    bool test_result = true;
    int n_sub = 4;
    bool sub_tests[] = {true, true, true, true};
    if(!no_yapping)
        printf("1. Testing linked list building:\n");

    linlst_init(&my_list);
    list_node_return found_node_buffer = {0};

    int first_length = 5;
    for(int i = 0; i < first_length; ++i) {
        linlst_append_node(&my_list, NODE_INT, sizeof(i), &i);
    }

    for(int i = 0; i < first_length; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        int data = *(int*)found_node_buffer.found_node_ptr->data;
        sub_tests[0] &= data == i;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i, data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[0]);
        printf("2. Testing linked list prepend:\n");
    }
    int my_int = -1;
    linlst_prepend_node(&my_list, NODE_INT, sizeof(my_int), &my_int);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        int data = *(int*)found_node_buffer.found_node_ptr->data;
        sub_tests[1] &= data == i - 1;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i, data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[1]);
        printf("3. Testing linked list insert:\n");
    }
    float my_dec = 3.5;
    linlst_get_node(&my_list, &found_node_buffer, 4);
    linlst_insert_node(&my_list, found_node_buffer.found_node_ptr, NODE_FLOAT,
                       sizeof(my_dec), &my_dec);

    bool test_res_insert = true;
    float my_res[] = {-1.0f, 0.0f, 1.0f, 2.0f, 3.0f, 3.5f, 4.0f, 5.0f};
    float float_data = 0.0;
    int int_data = 0;
    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        switch(found_node_buffer.found_node_ptr->dtype) {
            case NODE_INT:
            default:
                int_data = *(int*)found_node_buffer.found_node_ptr->data;
                test_res_insert &= (float)int_data == my_res[i];
                if(!no_yapping)
                    printf("%d-th Element = %d\n", i, int_data);

                break;
            case NODE_FLOAT:
                float_data = *(float*)found_node_buffer.found_node_ptr->data;
                test_res_insert &= float_data == my_res[i];
                if(!no_yapping)
                    printf("%d-th Element = %f\n", i, float_data);
                break;
        }
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(test_res_insert);
        printf("4. Testing linked list append:\n");
    }
    my_int = 5;
    linlst_append_node(&my_list, NODE_INT, sizeof(my_int), &my_int);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        switch(found_node_buffer.found_node_ptr->dtype) {
            case NODE_INT:
            default:
                int_data = *(int*)found_node_buffer.found_node_ptr->data;
                sub_tests[3] &= (float)int_data == my_res[i];
                if(!no_yapping)
                    printf("%d-th Element = %d\n", i, int_data);

                break;
            case NODE_FLOAT:
                float_data = *(float*)found_node_buffer.found_node_ptr->data;
                sub_tests[3] &= float_data == my_res[i];
                if(!no_yapping)
                    printf("%d-th Element = %f\n", i, float_data);
                break;
        }
    }
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[3]);
    }

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

bool test_delete_linked_list(bool no_yapping) {
    bool test_result = true;
    int n_sub = 5;
    bool sub_tests[] = {true, true, true, true, true};
    list_node_return found_node_buffer = {0};

    int int_data = 0;
    float float_data = 0.0;
    float my_res_1[] = {0.0f, 1.0f, 2.0f, 3.0f, 3.5f, 4.0f, 5.0f};
    float my_res_2[] = {0.0f, 1.0f, 2.0f, 3.0f, 3.5f, 4.0f};
    float my_res_3[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};

    if(!no_yapping)
        printf("\nInitial State:\n");

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        switch(found_node_buffer.found_node_ptr->dtype) {
            case NODE_INT:
            default:
                int_data = *(int*)found_node_buffer.found_node_ptr->data;
                if(!no_yapping)
                    printf("%d-th Element = %d\n", i, int_data);

                break;
            case NODE_FLOAT:
                float_data = *(float*)found_node_buffer.found_node_ptr->data;
                if(!no_yapping)
                    printf("%d-th Element = %f\n", i, float_data);
                break;
        }
    }

    if(!no_yapping)
        printf("1. Testing Delete first Node:\n");
    linlst_get_node(&my_list, &found_node_buffer, 0);
    linlst_delete_node(&my_list, found_node_buffer.found_node_ptr);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        switch(found_node_buffer.found_node_ptr->dtype) {
            case NODE_INT:
            default:
                int_data = *(int*)found_node_buffer.found_node_ptr->data;
                sub_tests[0] &= (float)int_data == my_res_1[i];
                if(!no_yapping)
                    printf("%d-th Element = %d\n", i, int_data);
                break;
            case NODE_FLOAT:
                float_data = *(float*)found_node_buffer.found_node_ptr->data;
                sub_tests[0] &= float_data == my_res_1[i];
                if(!no_yapping)
                    printf("%d-th Element = %f\n", i, float_data);
                break;
        }
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[0]);
        printf("2. Testing Delete last Node:\n");
    }
    linlst_get_node(&my_list, &found_node_buffer,
                    (uint8_t)my_list.list_len - 1);
    linlst_delete_node(&my_list, found_node_buffer.found_node_ptr);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        switch(found_node_buffer.found_node_ptr->dtype) {
            case NODE_INT:
            default:
                int_data = *(int*)found_node_buffer.found_node_ptr->data;
                sub_tests[1] &= (float)int_data == my_res_2[i];
                if(!no_yapping)
                    printf("%d-th Element = %d\n", i, int_data);

                break;
            case NODE_FLOAT:
                float_data = *(float*)found_node_buffer.found_node_ptr->data;
                sub_tests[1] &= float_data == my_res_2[i];
                if(!no_yapping)
                    printf("%d-th Element = %f\n", i, float_data);
                break;
        }
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[1]);
        printf("3. Testing Delete middle Node:\n");
    }
    linlst_get_node(&my_list, &found_node_buffer,
                    (uint8_t)my_list.list_len - 2);
    linlst_delete_node(&my_list, found_node_buffer.found_node_ptr);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        switch(found_node_buffer.found_node_ptr->dtype) {
            case NODE_INT:
            default:
                int_data = *(int*)found_node_buffer.found_node_ptr->data;
                sub_tests[2] &= (float)int_data == my_res_3[i];
                if(!no_yapping)
                    printf("%d-th Element = %d\n", i, int_data);

                break;
            case NODE_FLOAT:
                float_data = *(float*)found_node_buffer.found_node_ptr->data;
                sub_tests[2] &= float_data == my_res_3[i];
                if(!no_yapping)
                    printf("%d-th Element = %f\n", i, float_data);
                break;
        }
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[2]);
        printf("4. Testing Delete full list:\n");
    }
    linlst_delete_list(&my_list);
    linlst_get_node(&my_list, &found_node_buffer, 0);
    sub_tests[3] &= !found_node_buffer.node_found;
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[3]);
    }

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

bool test_circular_linked_list(bool no_yapping) {
    bool test_result = true;
    bool sub_tests[] = {true, true, true, true, true};
    int n_sub = 5;
    list_node_return found_node_buffer = {0};

    int int_data = 0;
    int my_res_1[] = {0, 1, 2, 3, 4};
    int my_res_2[] = {0, 1, 2, 3, 3, 4};
    int my_res_3[] = {-1, 0, 1, 2, 3, 4};
    int my_res_4[] = {0, 1, 2, 3, 4, 5};

    if(!no_yapping)
        printf("1. Testing circular linked list building:\n");

    my_list.list_type = CIRCULAR;
    linlst_init(&my_list);

    int first_length = 5;
    for(int i = 0; i < first_length; ++i) {
        linlst_append_node(&my_list, NODE_INT, sizeof(i), &i);
    }

    for(int i = 0; i < first_length; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        int_data = *(int*)found_node_buffer.found_node_ptr->data;
        sub_tests[0] &= int_data == my_res_1[i];
        if(!no_yapping)
            printf("%d-th Element = %d\n", i, int_data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[0]);
        printf("2. Testing circular linked list insert:\n");
    }
    int my_int = 3;
    linlst_get_node(&my_list, &found_node_buffer, 3);
    linlst_insert_node(&my_list, found_node_buffer.found_node_ptr, NODE_INT,
                       sizeof(my_int), &my_int);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        int_data = *(int*)found_node_buffer.found_node_ptr->data;
        sub_tests[1] &= int_data == my_res_2[i];
        if(!no_yapping)
            printf("%d-th Element = %d\n", i, int_data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[1]);
        printf("3a. Testing circular linked list traversal:\n");
    }
    int curr_list_len = (int)my_list.list_len;
    list_node* curr_list_node = my_list.ptr_first_node;
    for(int i = 0; i < curr_list_len * 2; ++i) {
        int_data = *(int*)curr_list_node->data;
        sub_tests[2] &= int_data == my_res_2[i % curr_list_len];
        curr_list_node = curr_list_node->next_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i % curr_list_len, int_data);
    }
    if(!no_yapping)
        printf("3b. Testing circular linked list reversed traversal:\n");
    curr_list_node = my_list.ptr_last_node;
    for(int i = curr_list_len * 2 - 1; i >= 0; --i) {
        int_data = *(int*)curr_list_node->data;
        sub_tests[2] &= int_data == my_res_2[i % curr_list_len];
        curr_list_node = curr_list_node->previous_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i % curr_list_len, int_data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[2]);
        printf("4. Testing circular linked list delete:\n");
    }
    linlst_get_node(&my_list, &found_node_buffer,
                    (uint8_t)my_list.list_len - 2);
    linlst_delete_node(&my_list, found_node_buffer.found_node_ptr);
    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)i);
        int_data = *(int*)found_node_buffer.found_node_ptr->data;
        sub_tests[3] &= int_data == my_res_1[i];
        if(!no_yapping)
            printf("%d-th Element = %d\n", i, int_data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[3]);
        printf("5. Testing circular linked list edges:\n");
    }

    if(!no_yapping) {
        printf("Prepend Case:\n");
        printf("Forward traversal:\n");
    }
    my_int = -1;
    linlst_prepend_node(&my_list, NODE_INT, sizeof(my_int), &my_int);
    curr_list_len = (int)my_list.list_len;
    curr_list_node = my_list.ptr_first_node;
    for(int i = 0; i < curr_list_len + 1; ++i) {
        int_data = *(int*)curr_list_node->data;
        sub_tests[4] &= int_data == my_res_3[i % curr_list_len];
        curr_list_node = curr_list_node->next_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i % curr_list_len, int_data);
    }
    if(!no_yapping)
        printf("Reversed traversal:\n");
    curr_list_node = my_list.ptr_last_node;
    for(int i = curr_list_len - 1; i >= -1; --i) {
        int_data = *(int*)curr_list_node->data;
        int idx = (i != -1) ? i : curr_list_len - 1;
        sub_tests[4] &= int_data == my_res_3[idx % curr_list_len];
        curr_list_node = curr_list_node->previous_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", idx % curr_list_len, int_data);
    }

    if(!no_yapping) {
        printf("Prepend Case - Delete:\n");
        printf("Forward traversal:\n");
    }
    linlst_delete_node(&my_list, my_list.ptr_first_node);
    curr_list_len = (int)my_list.list_len;
    curr_list_node = my_list.ptr_first_node;
    for(int i = 0; i < curr_list_len + 1; ++i) {
        int_data = *(int*)curr_list_node->data;
        sub_tests[4] &= int_data == my_res_1[i % curr_list_len];
        curr_list_node = curr_list_node->next_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i % curr_list_len, int_data);
    }
    if(!no_yapping)
        printf("Reversed traversal:\n");
    curr_list_node = my_list.ptr_last_node;
    for(int i = curr_list_len - 1; i >= -1; --i) {
        int_data = *(int*)curr_list_node->data;
        int idx = (i != -1) ? i : curr_list_len - 1;
        sub_tests[4] &= int_data == my_res_1[idx % curr_list_len];
        curr_list_node = curr_list_node->previous_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", idx % curr_list_len, int_data);
    }

    if(!no_yapping) {
        printf("Append Case:\n");
        printf("Forward traversal:\n");
    }
    my_int = 5;
    linlst_append_node(&my_list, NODE_INT, sizeof(my_int), &my_int);
    curr_list_len = (int)my_list.list_len;
    curr_list_node = my_list.ptr_first_node;
    for(int i = 0; i < curr_list_len + 1; ++i) {
        int_data = *(int*)curr_list_node->data;
        sub_tests[4] &= int_data == my_res_4[i % curr_list_len];
        curr_list_node = curr_list_node->next_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i % curr_list_len, int_data);
    }
    if(!no_yapping)
        printf("Reversed traversal:\n");
    curr_list_node = my_list.ptr_last_node;
    for(int i = curr_list_len - 1; i >= -1; --i) {
        int_data = *(int*)curr_list_node->data;
        int idx = (i != -1) ? i : curr_list_len - 1;
        sub_tests[4] &= int_data == my_res_4[idx % curr_list_len];
        curr_list_node = curr_list_node->previous_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", idx % curr_list_len, int_data);
    }

    if(!no_yapping) {
        printf("Append Case - Delete:\n");
        printf("Forward traversal:\n");
    }
    linlst_delete_node(&my_list, my_list.ptr_last_node);
    curr_list_len = (int)my_list.list_len;
    curr_list_node = my_list.ptr_first_node;
    for(int i = 0; i < curr_list_len + 1; ++i) {
        int_data = *(int*)curr_list_node->data;
        sub_tests[4] &= int_data == my_res_4[i % curr_list_len];
        curr_list_node = curr_list_node->next_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i % curr_list_len, int_data);
    }
    if(!no_yapping)
        printf("Reversed traversal:\n");
    curr_list_node = my_list.ptr_last_node;
    for(int i = curr_list_len - 1; i >= -1; --i) {
        int_data = *(int*)curr_list_node->data;
        int idx = (i != -1) ? i : curr_list_len - 1;
        sub_tests[4] &= int_data == my_res_4[idx % curr_list_len];
        curr_list_node = curr_list_node->previous_node;
        if(!no_yapping)
            printf("%d-th Element = %d\n", idx % curr_list_len, int_data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[4]);
    }

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

bool test_struct_linked_list(bool no_yapping) {
    bool test_result = true;
    bool sub_tests[] = {true, true, true, true, true};
    const int n_sub = 5;

    my_test_structure a_struct = {.idx = 1, .name = "Karl"};
    my_test_structure b_struct = {.idx = 2, .name = "Vladimir"};
    my_other_test_structure c_struct = {.gender = 'F', .name = "Jenny"};
    my_other_test_structure d_struct = {.gender = 'F', .name = "Nadjia"};

    my_test_structure* array_ptr_structs_a[] = {&a_struct, &b_struct};
    my_other_test_structure* array_ptr_structs_b[] = {&c_struct, &d_struct};
    const int n_structs = 4;

    if(!no_yapping)
        printf("1. Testing structs linked list building:\n");

    my_list.list_type = OPEN;
    linlst_init(&my_list);
    list_node_return found_node_buffer = {0};
    int count_struct_a = 0;
    int count_struct_b = 0;
    for(int i = 0; i < n_structs; ++i) {
        if(i % 2 == 0) {
            linlst_append_node(&my_list, (node_type)STRUCT_1,
                               sizeof(my_test_structure),
                               array_ptr_structs_a[count_struct_a++]);
        } else {
            linlst_append_node(&my_list, (node_type)STRUCT_2,
                               sizeof(my_test_structure),
                               array_ptr_structs_b[count_struct_b++]);
        }
    }

    count_struct_a = 0;
    count_struct_b = 0;
    my_test_structure buffer_a = {0};
    my_other_test_structure buffer_b = {0};
    int j = 0;
    linlst_get_node(&my_list, &found_node_buffer, (uint8_t)j);
    while(found_node_buffer.node_found) {
        if(!no_yapping)
            printf("┌────────────────────────────┐\n");

        list_node* found_node_ptr = found_node_buffer.found_node_ptr;
        my_struct_types found_struct_type =
            (my_struct_types)found_node_ptr->dtype;
        switch(found_struct_type) {
            case STRUCT_1:
            default:
                buffer_a = *(my_test_structure*)found_node_ptr->data;
                sub_tests[0] &=
                    buffer_a.idx == array_ptr_structs_a[count_struct_a]->idx;

                sub_tests[0] &=
                    0 == strcmp(buffer_a.name,
                                array_ptr_structs_a[count_struct_a]->name);
                ++count_struct_a;
                if(!no_yapping)
                    printf("│ Index: %d  - Name: %-8s │\n", buffer_a.idx,
                           buffer_a.name);
                break;

            case STRUCT_2:
                buffer_b = *(my_other_test_structure*)found_node_ptr->data;
                sub_tests[0] &= buffer_b.gender ==
                                array_ptr_structs_b[count_struct_b]->gender;

                sub_tests[0] &=
                    0 == strcmp(buffer_b.name,
                                array_ptr_structs_b[count_struct_b]->name);
                ++count_struct_b;
                if(!no_yapping)
                    printf("│ Gender: %c - Name: %-8s │\n", buffer_b.gender,
                           buffer_b.name);
                break;
        }

        if(!no_yapping)
            printf("└────────────────────────────┘\n");

        ++j;
        linlst_get_node(&my_list, &found_node_buffer, (uint8_t)j);
        if(!no_yapping && found_node_buffer.node_found) {
            printf("              ↑  │\n");
            printf("              │  ↓\n");
        }
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[0]);
    }

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    bool test_result1 = test_build_linked_list(NO_YAPPING);
    bool test_result2 = test_delete_linked_list(NO_YAPPING);
    bool test_result3 = test_circular_linked_list(NO_YAPPING);
    bool test_result4 = test_struct_linked_list(NO_YAPPING);

    printf("Linked List Test Suite Summary:\n");
    printf("%*s", TEST_NAMES_LENGTH, "List Building Test: ");
    print_test_res(test_result1);
    printf("%*s", TEST_NAMES_LENGTH, "List Delete Test: ");
    print_test_res(test_result2);
    printf("%*s", TEST_NAMES_LENGTH, "Circular List Test: ");
    print_test_res(test_result3);
    printf("%*s", TEST_NAMES_LENGTH, "Struct List Test: ");
    print_test_res(test_result4);
}