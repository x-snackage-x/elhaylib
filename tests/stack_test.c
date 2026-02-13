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
#define NO_YAPPING false

typedef enum { STRUCT_1 = 1000, STRUCT_2 = 1001 } my_struct_types;

typedef struct my_test_structure {
    int idx;
    char name[10];
} my_test_structure;

typedef struct my_other_test_structure {
    char gender;
    char name[10];
} my_other_test_structure;

typedef struct {
    void* ptr_to_mystruct;
    my_struct_types struct_type;
} my_stack_frame;

void print_test_res(bool result) {
    if(result) {
        printf(GRN "S" RESET);
    } else {
        printf(RED "F" RESET);
    }
    printf("\n");
}

bool test_primitive_types_stack(bool no_yapping) {
    bool test_result = true;
    bool sub_tests[] = {true, true, true};
    int n_sub = 3;

    int in_data1[] = {4, 3, 2, 1, 0};
    int in_data2[] = {40, 30, 20};

    int int_data = 0;
    int my_res_1[] = {0, 1, 2};
    int my_res_2[] = {20, 30, 40, 3, 4};

    stack_head* my_stack = stack_init(sizeof(int_data));

    // first fill
    for(int i = 0; i < 5; ++i) {
        stack_push(my_stack, &in_data1[i]);
    }

    // three pops
    if(!no_yapping)
        printf("1. Testing 3 Pops:\n");

    for(int i = 0; i < 3; ++i) {
        stack_pop(my_stack, &int_data);
        sub_tests[0] &= int_data == my_res_1[i];

        if(!no_yapping)
            printf("%d-th Popped Element = %d\n", i, int_data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[0]);
    }

    // test peak
    if(!no_yapping)
        printf("2. Testing Peek:\n");

    stack_peek(my_stack, &int_data);
    sub_tests[0] &= int_data == in_data1[1];

    if(!no_yapping)
        printf("Peeked Element, expected %d, is %d\n", in_data1[1], int_data);

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[1]);
    }

    // second fill
    for(int i = 0; i < 3; ++i) {
        stack_push(my_stack, &in_data2[i]);
    }

    // all pops
    if(!no_yapping)
        printf("3. Testing full Pops:\n");

    bool is_not_empty = stack_pop(my_stack, &int_data);
    int index = 0;
    while(is_not_empty) {
        sub_tests[2] &= int_data == my_res_2[index];

        if(!no_yapping)
            printf("%d-th Popped Element = %d\n", index++, int_data);

        is_not_empty = stack_pop(my_stack, &int_data);
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[2]);
    }

    stack_free(my_stack);

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

bool test_struct_stack(bool no_yapping) {
    bool test_result = true;
    bool sub_tests[] = {true, true};
    const int n_sub = 2;

    my_test_structure a_struct = {.idx = 1, .name = "Karl"};
    my_test_structure b_struct = {.idx = 2, .name = "Vladimir"};
    my_other_test_structure c_struct = {.gender = 'F', .name = "Jenny"};
    my_other_test_structure d_struct = {.gender = 'F', .name = "Nadezhda"};

    if(!no_yapping)
        printf("1. Testing stack with structs:\n");

    // prep stack frames
    my_stack_frame frame1 = {.ptr_to_mystruct = &a_struct,
                             .struct_type = STRUCT_1};
    my_stack_frame frame2 = {.ptr_to_mystruct = &b_struct,
                             .struct_type = STRUCT_1};
    my_stack_frame frame3 = {.ptr_to_mystruct = &c_struct,
                             .struct_type = STRUCT_2};
    my_stack_frame frame4 = {.ptr_to_mystruct = &d_struct,
                             .struct_type = STRUCT_2};
    my_stack_frame* stack_frame_expected_ptrs[4] = {&frame4, &frame2, &frame3,
                                                    &frame1};

    // prep stack
    stack_head* my_stack = stack_init(sizeof(my_stack_frame));

    // fill stack
    stack_push(my_stack, &frame1);
    stack_push(my_stack, &frame3);
    stack_push(my_stack, &frame2);
    stack_push(my_stack, &frame4);

    // pop all stack
    my_stack_frame stack_frame_ptrs[4] = {0};
    int index = 0;
    bool is_not_empty = stack_pop(my_stack, &stack_frame_ptrs[index]);
    while(is_not_empty) {
        sub_tests[0] &= stack_frame_ptrs[index].ptr_to_mystruct ==
                        stack_frame_expected_ptrs[index]->ptr_to_mystruct;

        sub_tests[0] &= stack_frame_ptrs[index].struct_type ==
                        stack_frame_expected_ptrs[index]->struct_type;
        ++index;
        is_not_empty = stack_pop(my_stack, &stack_frame_ptrs[index]);
    }

    // test stack is empty
    sub_tests[1] &= !stack_pop(my_stack, &stack_frame_ptrs[index]);

    stack_free(my_stack);

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    bool test_result1 = test_primitive_types_stack(NO_YAPPING);
    bool test_result2 = test_struct_stack(NO_YAPPING);

    printf("Stack Test Suite Summary:\n");
    printf("%*s", TEST_NAMES_LENGTH, "Primitives Stack Test: ");
    print_test_res(test_result1);
    printf("%*s", TEST_NAMES_LENGTH, "Struct Stack Test: ");
    print_test_res(test_result2);
}