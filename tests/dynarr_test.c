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
#define NO_YAPPING true

typedef struct {
    int anInt;
    float aFloat;
    double aDouble;
    void* aPointer;
} my_test_struct;

void print_test_res(bool result) {
    if(result) {
        printf(GRN "S" RESET);
    } else {
        printf(RED "F" RESET);
    }
    printf("\n");
}

bool test_primitive_types(bool no_yapping) {
    bool test_result = true;
    int n_sub = 7;
    bool sub_tests[] = {true, true, true, true, true, true, true};

    if(!no_yapping)
        printf("1. Testing dynamic array with scaler types:\n");

    dynarr_head my_arr = {
        .elem_size = sizeof(float), .dynarr_capacity = 3, .growth_fac = 1.5f};
    dynarr_init(&my_arr);

    float* ptr_head_float = (float*)my_arr.ptr_first_elem;

    float my_floats[] = {1.5437f, 25.246f, 333.35f, 4926.3f, 54535.f};

    *(ptr_head_float) = my_floats[0];
    *(ptr_head_float + 1) = my_floats[1];
    *(ptr_head_float + 2) = my_floats[2];
    my_arr.dynarr_size = 3;

    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        sub_tests[0] &= (my_floats[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[0]);
        printf("Testing expansion:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }

    ptr_head_float = (float*)dynarr_append(&my_arr, &my_floats[3]);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        test_result &= (my_floats[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }

    ptr_head_float = (float*)dynarr_append(&my_arr, &my_floats[4]);
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[1]);
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        sub_tests[2] &= (my_floats[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[2]);
        printf("Testing insert:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }

    float my_floats_insert[] = {1.5437f, 25.246f, 268.879f,
                                333.35f, 4926.3f, 54535.f};

    ptr_head_float = (float*)dynarr_insert(&my_arr, &my_floats_insert[2], 2);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        sub_tests[3] &= (my_floats_insert[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[3]);
        printf("Testing remove:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    dynarr_remove(&my_arr, 2);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        sub_tests[4] &= (my_floats[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[4]);
        printf("Testing n-remove:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    float my_floats_shrunk[] = {1.5437f, 25.246f, 54535.f};
    dynarr_remove_n(&my_arr, 2, 2);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        sub_tests[5] &= (my_floats_shrunk[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[5]);
        printf("Testing End element remove:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    dynarr_remove(&my_arr, 2);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        sub_tests[6] &= (my_floats_shrunk[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[6]);
    }

    dynarr_free(&my_arr);

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

bool test_struct(bool no_yapping) {
    bool test_result = true;
    int n_sub = 6;
    bool sub_tests[] = {true, true, true, true, true, true};

    if(!no_yapping)
        printf("\n2. Testing dynamic array with a struct:\n");

    dynarr_head my_arr = {0};
    my_arr.elem_size = sizeof(my_test_struct);
    my_arr.dynarr_capacity = 2;
    my_arr.growth_fac = 2.f;
    dynarr_init(&my_arr);

    if(!no_yapping) {
        printf("Array Head Struct:\n");
        printf(" {\n");
        printf("   %zu\n", my_arr.elem_size);
        printf("   %zu\n", my_arr.dynarr_size);
        printf("   %zu\n", my_arr.dynarr_capacity);
        printf("   %f\n", my_arr.growth_fac);
        printf("   %p\n", my_arr.ptr_first_elem);
        printf(" }\n\n");
    }

    my_test_struct* ptr_head = (my_test_struct*)my_arr.ptr_first_elem;

    my_test_struct my_structs[] = {{1, 1.1f, 1.2, ptr_head},
                                   {2, 2.1f, 2.2, 0},
                                   {3, 3.1f, 3.2, 0},
                                   {4, 4.1f, 4.2, 0}};

    my_structs[1].aPointer = &my_structs[0];
    my_structs[2].aPointer = &my_structs[1];
    my_structs[3].aPointer = &my_structs[2];

    my_test_struct my_structs_shrunk[] = {{1, 1.1f, 1.2, ptr_head},
                                          {4, 4.1f, 4.2, 0}};
    my_structs_shrunk[1].aPointer = &my_structs[2];

    ptr_head = (my_test_struct*)dynarr_append(&my_arr, &my_structs[0]);
    ptr_head = (my_test_struct*)dynarr_append(&my_arr, &my_structs[1]);

    my_test_struct* ptr_data = 0;
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        sub_tests[0] &= (my_structs[i].anInt == ptr_data->anInt);
        sub_tests[0] &= (my_structs[i].aFloat == ptr_data->aFloat);
        sub_tests[0] &= (my_structs[i].aDouble == ptr_data->aDouble);
        sub_tests[0] &= (my_structs[i].aPointer == ptr_data->aPointer);

        if(!no_yapping) {
            printf("%ld-th Struct:\n", i);
            printf(" {\n");
            printf("   %d, ", ptr_data->anInt + 1);
            printf("   %f, ", ptr_data->aFloat);
            printf("   %f\n", ptr_data->aDouble);
            printf("   %p\n", ptr_data->aPointer);
            printf(" }\n");
        }
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[0]);
        printf("Testing expansion:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        sub_tests[1] &= (my_structs[i].anInt == ptr_data->anInt);
        sub_tests[1] &= (my_structs[i].aFloat == ptr_data->aFloat);
        sub_tests[1] &= (my_structs[i].aDouble == ptr_data->aDouble);
        sub_tests[1] &= (my_structs[i].aPointer == ptr_data->aPointer);

        if(!no_yapping) {
            printf("%ld-th Struct:\n", i);
            printf(" {\n");
            printf("   %d, ", ptr_data->anInt);
            printf("   %f, ", ptr_data->aFloat);
            printf("   %f\n", ptr_data->aDouble);
            printf("   %p\n", ptr_data->aPointer);
            printf(" }\n");
        }
    }

    ptr_head = (my_test_struct*)dynarr_append(&my_arr, &my_structs[2]);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        sub_tests[1] &= (my_structs[i].anInt == ptr_data->anInt);
        sub_tests[1] &= (my_structs[i].aFloat == ptr_data->aFloat);
        sub_tests[1] &= (my_structs[i].aDouble == ptr_data->aDouble);
        sub_tests[1] &= (my_structs[i].aPointer == ptr_data->aPointer);

        if(!no_yapping) {
            printf("%ld-th Struct:\n", i);
            printf(" {\n");
            printf("   %d, ", ptr_data->anInt);
            printf("   %f, ", ptr_data->aFloat);
            printf("   %f\n", ptr_data->aDouble);
            printf("   %p\n", ptr_data->aPointer);
            printf(" }\n");
        }
    }

    ptr_head = (my_test_struct*)dynarr_append(&my_arr, &my_structs[3]);
    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[1]);
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        sub_tests[2] &= (my_structs[i].anInt == ptr_data->anInt);
        sub_tests[2] &= (my_structs[i].aFloat == ptr_data->aFloat);
        sub_tests[2] &= (my_structs[i].aDouble == ptr_data->aDouble);
        sub_tests[2] &= (my_structs[i].aPointer == ptr_data->aPointer);

        if(!no_yapping) {
            printf("%ld-th Struct:\n", i);
            printf(" {\n");
            printf("   %d, ", ptr_data->anInt);
            printf("   %f, ", ptr_data->aFloat);
            printf("   %f\n", ptr_data->aDouble);
            printf("   %p\n", ptr_data->aPointer);
            printf(" }\n");
        }
    }

    if(!no_yapping) {
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[2]);
        printf("Testing insert:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }

    my_test_struct my_structs_insert[] = {{2, 2.6f, 2.6, &my_structs[3]}};

    ptr_head =
        (my_test_struct*)dynarr_insert(&my_arr, &my_structs_insert[0], 2);
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        if(i < 2) {
            sub_tests[3] &= (my_structs[i].anInt == ptr_data->anInt);
            sub_tests[3] &= (my_structs[i].aFloat == ptr_data->aFloat);
            sub_tests[3] &= (my_structs[i].aDouble == ptr_data->aDouble);
            sub_tests[3] &= (my_structs[i].aPointer == ptr_data->aPointer);

        } else if(i == 2) {
            sub_tests[3] &= (my_structs_insert[0].anInt == ptr_data->anInt);
            sub_tests[3] &= (my_structs_insert[0].aFloat == ptr_data->aFloat);
            sub_tests[3] &= (my_structs_insert[0].aDouble == ptr_data->aDouble);
            sub_tests[3] &=
                (my_structs_insert[0].aPointer == ptr_data->aPointer);
        } else {
            sub_tests[3] &= (my_structs[i - 1].anInt == ptr_data->anInt);
            sub_tests[3] &= (my_structs[i - 1].aFloat == ptr_data->aFloat);
            sub_tests[3] &= (my_structs[i - 1].aDouble == ptr_data->aDouble);
            sub_tests[3] &= (my_structs[i - 1].aPointer == ptr_data->aPointer);
        }

        if(!no_yapping) {
            printf("%ld-th Struct:\n", i);
            printf(" {\n");
            printf("   %d, ", ptr_data->anInt);
            printf("   %f, ", ptr_data->aFloat);
            printf("   %f\n", ptr_data->aDouble);
            printf("   %p\n", ptr_data->aPointer);
            printf(" }\n");
        }
    }

    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[3]);
        printf("Testing remove:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }

    dynarr_remove(&my_arr, 2);
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        sub_tests[4] &= (my_structs[i].anInt == ptr_data->anInt);
        sub_tests[4] &= (my_structs[i].aFloat == ptr_data->aFloat);
        sub_tests[4] &= (my_structs[i].aDouble == ptr_data->aDouble);
        sub_tests[4] &= (my_structs[i].aPointer == ptr_data->aPointer);

        if(!no_yapping) {
            printf("%ld-th Struct:\n", i);
            printf(" {\n");
            printf("   %d, ", ptr_data->anInt);
            printf("   %f, ", ptr_data->aFloat);
            printf("   %f\n", ptr_data->aDouble);
            printf("   %p\n", ptr_data->aPointer);
            printf(" }\n");
        }
    }

    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[4]);
        printf("Testing n-remove:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }

    dynarr_remove_n(&my_arr, 1, 2);
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        sub_tests[5] &= (my_structs_shrunk[i].anInt == ptr_data->anInt);
        sub_tests[5] &= (my_structs_shrunk[i].aFloat == ptr_data->aFloat);
        sub_tests[5] &= (my_structs_shrunk[i].aDouble == ptr_data->aDouble);
        sub_tests[5] &= (my_structs_shrunk[i].aPointer == ptr_data->aPointer);

        if(!no_yapping) {
            printf("%ld-th Struct:\n", i);
            printf(" {\n");
            printf("   %d, ", ptr_data->anInt);
            printf("   %f, ", ptr_data->aFloat);
            printf("   %f\n", ptr_data->aDouble);
            printf("   %p\n", ptr_data->aPointer);
            printf(" }\n");
        }
    }

    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
        printf("%*s", TEST_NAMES_LENGTH, "Result: ");
        print_test_res(sub_tests[5]);
    }

    dynarr_free(&my_arr);

    for(int i = 0; i < n_sub; ++i) {
        test_result &= sub_tests[i];
    }
    return test_result;
}

int main() {
    bool scalers_test = test_primitive_types(NO_YAPPING);
    bool struct_test = test_struct(NO_YAPPING);

    printf("Dynamic Array Test Suite Summary:\n");
    printf("%*s", TEST_NAMES_LENGTH, "Primitives Array Test: ");
    print_test_res(scalers_test);
    printf("%*s", TEST_NAMES_LENGTH, "Struct Array Test: ");
    print_test_res(struct_test);
}