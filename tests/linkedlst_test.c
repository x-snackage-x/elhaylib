#include "../elhaylib.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

#define TEST_NAMES_LENGTH 25
#define NO_YAPPING false

typedef struct {
    char* name;
} my_test_structure;

typedef struct {
    char* name;
    char gender;
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
    if(!no_yapping)
        printf("1. Testing linked list building:\n");

    linlst_init(&my_list);
    list_node_return found_node_buffer = {0};

    int first_length = 5;
    for(int i = 0; i < first_length; ++i) {
        linlist_append_node(&my_list, NODE_INT, sizeof(i), &i);
    }

    bool test_res_first_build = true;
    for(int i = 0; i < first_length; ++i) {
        linlist_get_node(&my_list, &found_node_buffer, i);
        int data = *(int*)found_node_buffer.found_node_ptr->data;
        test_res_first_build &= data == i;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i, data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(test_res_first_build);
        printf("2. Testing linked list prepend:\n");
    }
    int my_int = -1;
    linlist_prepend_node(&my_list, NODE_INT, sizeof(my_int), &my_int);

    bool test_res_prepend = true;
    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlist_get_node(&my_list, &found_node_buffer, i);
        int data = *(int*)found_node_buffer.found_node_ptr->data;
        test_res_prepend &= data == i - 1;
        if(!no_yapping)
            printf("%d-th Element = %d\n", i, data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(test_res_prepend);
        printf("3. Testing linked list insert:\n");
    }
    float my_dec = 3.5;
    linlist_get_node(&my_list, &found_node_buffer, 4);
    linlist_insert_node(&my_list, found_node_buffer.found_node_ptr, NODE_FLOAT,
                        sizeof(my_dec), &my_dec);

    bool test_res_insert = true;
    float my_res[] = {-1.0f, 0.0f, 1.0f, 2.0f, 3.0f, 3.5f, 4.0f, 5.0f};
    float float_data = 0.0;
    int int_data = 0;
    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlist_get_node(&my_list, &found_node_buffer, i);
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
    linlist_append_node(&my_list, NODE_INT, sizeof(my_int), &my_int);

    bool test_res_append = true;
    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlist_get_node(&my_list, &found_node_buffer, i);
        switch(found_node_buffer.found_node_ptr->dtype) {
            case NODE_INT:
            default:
                int_data = *(int*)found_node_buffer.found_node_ptr->data;
                test_res_append &= (float)int_data == my_res[i];
                if(!no_yapping)
                    printf("%d-th Element = %d\n", i, int_data);

                break;
            case NODE_FLOAT:
                float_data = *(float*)found_node_buffer.found_node_ptr->data;
                test_res_append &= float_data == my_res[i];
                if(!no_yapping)
                    printf("%d-th Element = %f\n", i, float_data);
                break;
        }
    }
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(test_res_append);
    }

    test_result &= test_res_first_build && test_res_prepend &&
                   test_res_insert && test_res_append;
    return test_result;
}

bool test_delete_linked_list(bool no_yapping) {
    bool test_result = true;
    bool sub_tests[] = {true, true, true, true, true};
    int n_sub = 5;
    list_node_return found_node_buffer = {0};

    int int_data = 0;
    float float_data = 0.0;
    float my_res_1[] = {0.0f, 1.0f, 2.0f, 3.0f, 3.5f, 4.0f, 5.0f};
    float my_res_2[] = {0.0f, 1.0f, 2.0f, 3.0f, 3.5f, 4.0f};
    float my_res_3[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};

    if(!no_yapping)
        printf("\nInitial State:\n");

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlist_get_node(&my_list, &found_node_buffer, i);
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
    linlist_get_node(&my_list, &found_node_buffer, 0);
    linlist_delete_node(&my_list, found_node_buffer.found_node_ptr);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlist_get_node(&my_list, &found_node_buffer, i);
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
    linlist_get_node(&my_list, &found_node_buffer, my_list.list_len - 1);
    linlist_delete_node(&my_list, found_node_buffer.found_node_ptr);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlist_get_node(&my_list, &found_node_buffer, i);
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
    linlist_get_node(&my_list, &found_node_buffer, my_list.list_len - 2);
    linlist_delete_node(&my_list, found_node_buffer.found_node_ptr);

    for(int i = 0; i < (int)my_list.list_len; ++i) {
        linlist_get_node(&my_list, &found_node_buffer, i);
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
    linlist_delete_list(&my_list);
    linlist_get_node(&my_list, &found_node_buffer, 0);
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
    return true;
}

int main() {
    bool test_result1 = test_build_linked_list(NO_YAPPING);
    bool test_result2 = test_delete_linked_list(NO_YAPPING);
    bool test_result3 = test_circular_linked_list(NO_YAPPING);

    printf("Linked List Test Suite Summary:\n");
    printf("%*s", TEST_NAMES_LENGTH, "List Building Test: ");
    print_test_res(test_result1);
    printf("%*s", TEST_NAMES_LENGTH, "List Delete Test: ");
    print_test_res(test_result2);
    printf("%*s", TEST_NAMES_LENGTH, "Circular List Test: ");
    print_test_res(test_result3);
}