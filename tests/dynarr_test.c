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

bool test_primitive_types(bool no_yapping) {
    bool test_result = true;
    if(!no_yapping)
        printf("1. Testing dynamic array with primitive types:\n");

    dynarr_head my_arr = {
        .elem_size = sizeof(float), .dynarr_capacity = 3, .growth_fac = 1.5f};
    dynarr_init(&my_arr);

    float* ptr_head_float = (float*)my_arr.ptr_first_elem;

    float my_floats[] = {1.5437f, 25.246f, 333.33f, 4926.3f, 54535.f};

    *(ptr_head_float) = my_floats[0];
    *(ptr_head_float + 1) = my_floats[1];
    *(ptr_head_float + 2) = my_floats[2];
    my_arr.dynarr_size = 3;

    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        test_result &= (my_floats[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i) + 1);
    }

    if(!no_yapping) {
        printf("Testing expansion:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }

    my_floats[3] = 4926.3f;
    dynarr_push(&my_arr, &my_floats[3]);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        test_result &= (my_floats[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }

    my_floats[4] = 54535.f;
    dynarr_push(&my_arr, &my_floats[4]);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        test_result &= (my_floats[i] == *(ptr_head_float + i));
        if(!no_yapping)
            printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }

    dynarr_free(&my_arr);

    return test_result;
}

bool test_struct(bool no_yapping) {
    bool test_result = true;
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

    dynarr_push(&my_arr, &my_structs[0]);
    dynarr_push(&my_arr, &my_structs[1]);

    my_test_struct* ptr_data = 0;
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        test_result &= (my_structs[i].anInt == ptr_data->anInt);
        test_result &= (my_structs[i].aFloat == ptr_data->aFloat);
        test_result &= (my_structs[i].aDouble == ptr_data->aDouble);
        test_result &= (my_structs[i].aPointer == ptr_data->aPointer);

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
        printf("Testing expansion:\n");
        printf("Before Size: %zu\n", my_arr.dynarr_size);
        printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        test_result &= (my_structs[i].anInt == ptr_data->anInt);
        test_result &= (my_structs[i].aFloat == ptr_data->aFloat);
        test_result &= (my_structs[i].aDouble == ptr_data->aDouble);
        test_result &= (my_structs[i].aPointer == ptr_data->aPointer);

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

    dynarr_push(&my_arr, &my_structs[2]);
    if(!no_yapping) {
        printf("After Size: %zu\n", my_arr.dynarr_size);
        printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        test_result &= (my_structs[i].anInt == ptr_data->anInt);
        test_result &= (my_structs[i].aFloat == ptr_data->aFloat);
        test_result &= (my_structs[i].aDouble == ptr_data->aDouble);
        test_result &= (my_structs[i].aPointer == ptr_data->aPointer);

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

    dynarr_push(&my_arr, &my_structs[3]);
    if(!no_yapping) {
        printf("End Size: %zu\n", my_arr.dynarr_size);
        printf("End Cap.: %zu\n\n", my_arr.dynarr_capacity);
    }
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        test_result &= (my_structs[i].anInt == ptr_data->anInt);
        test_result &= (my_structs[i].aFloat == ptr_data->aFloat);
        test_result &= (my_structs[i].aDouble == ptr_data->aDouble);
        test_result &= (my_structs[i].aPointer == ptr_data->aPointer);

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
    dynarr_free(&my_arr);

    return test_result;
}

int main() {
    bool primitives_test = test_primitive_types(NO_YAPPING);
    bool struct_test = test_struct(NO_YAPPING);

    printf("Dynamic Array Test Suite Summary:\n");
    printf("%*s", TEST_NAMES_LENGTH, "Primitives Array Test: ");
    if(primitives_test) {
        printf(GRN "S" RESET);
    } else {
        printf(RED "F" RESET);
    }
    printf("\n");
    printf("%*s", TEST_NAMES_LENGTH, "Struct Array Test: ");
    if(struct_test) {
        printf(GRN "S" RESET);
    } else {
        printf(RED "F" RESET);
    }
    printf("\n");
}