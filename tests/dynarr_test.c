#include "../elhaylib.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int anInt;
    float aFloat;
    double aDouble;
    void* aPointer;
} my_test_struct;

int main() {
    printf("1. Testing dynamic array with primitive types:\n");

    dynarr_head my_arr = {
        .elem_size = sizeof(float), .dynarr_capacity = 3, .growth_fac = 1.5f};
    dynarr_init(&my_arr);

    float* ptr_head_float = (float*)my_arr.ptr_first_elem;

    *(ptr_head_float) = 1.5437f;
    *(ptr_head_float + 1) = 25.246f;
    *(ptr_head_float + 2) = 333.33f;
    my_arr.dynarr_size = 3;

    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }

    printf("Testing expansion:\n");
    printf("Before Size: %zu\n", my_arr.dynarr_size);
    printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);

    float new_number = 4926.3f;
    dynarr_push(&my_arr, &new_number);

    printf("After Size: %zu\n", my_arr.dynarr_size);
    printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }

    new_number = 54535.f;
    dynarr_push(&my_arr, &new_number);

    printf("After Size: %zu\n", my_arr.dynarr_size);
    printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        printf("%ld-th Element = %f\n", i, *(ptr_head_float + i));
    }

    dynarr_free(&my_arr);

    printf("\n2. Testing dynamic array with a struct:\n");

    my_arr.elem_size = sizeof(my_test_struct);
    my_arr.dynarr_capacity = 2;
    my_arr.growth_fac = 2.f;
    dynarr_init(&my_arr);

    printf("Array Head Struct:\n");
    printf(" {\n");
    printf("   %zu\n", my_arr.elem_size);
    printf("   %zu\n", my_arr.dynarr_size);
    printf("   %zu\n", my_arr.dynarr_capacity);
    printf("   %f\n", my_arr.growth_fac);
    printf("   %p\n", my_arr.ptr_first_elem);
    printf(" }\n\n");

    my_test_struct* ptr_head = (my_test_struct*)my_arr.ptr_first_elem;

    my_test_struct first_struct = {1, 1.1f, 1.2, ptr_head};
    my_test_struct second_struct = {2, 2.1f, 2.2, &first_struct};
    my_test_struct third_struct = {3, 3.1f, 3.2, &second_struct};
    my_test_struct forth_struct = {4, 4.1f, 4.2, &third_struct};

    dynarr_push(&my_arr, &first_struct);
    dynarr_push(&my_arr, &second_struct);

    my_test_struct* ptr_data = 0;
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        printf("%ld-th Struct:\n", i);
        printf(" {\n");
        printf("   %d, ", ptr_data->anInt);
        printf("   %f, ", ptr_data->aFloat);
        printf("   %f\n", ptr_data->aDouble);
        printf("   %p\n", ptr_data->aPointer);
        printf(" }\n");
    }

    printf("Testing expansion:\n");
    printf("Before Size: %zu\n", my_arr.dynarr_size);
    printf("Before Cap.: %zu\n", my_arr.dynarr_capacity);
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        printf("%ld-th Struct:\n", i);
        printf(" {\n");
        printf("   %d, ", ptr_data->anInt);
        printf("   %f, ", ptr_data->aFloat);
        printf("   %f\n", ptr_data->aDouble);
        printf("   %p\n", ptr_data->aPointer);
        printf(" }\n");
    }

    dynarr_push(&my_arr, &third_struct);
    printf("After Size: %zu\n", my_arr.dynarr_size);
    printf("After Cap.: %zu\n", my_arr.dynarr_capacity);
    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        printf("%ld-th Struct:\n", i);
        printf(" {\n");
        printf("   %d, ", ptr_data->anInt);
        printf("   %f, ", ptr_data->aFloat);
        printf("   %f\n", ptr_data->aDouble);
        printf("   %p\n", ptr_data->aPointer);
        printf(" }\n");
    }

    dynarr_push(&my_arr, &forth_struct);
    printf("End Size: %zu\n", my_arr.dynarr_size);
    printf("End Cap.: %zu\n\n", my_arr.dynarr_capacity);

    for(long unsigned int i = 0; i < my_arr.dynarr_size; ++i) {
        ptr_data = (ptr_head + i);
        printf("%ld-th Struct:\n", i);
        printf(" {\n");
        printf("   %d, ", ptr_data->anInt);
        printf("   %f, ", ptr_data->aFloat);
        printf("   %f\n", ptr_data->aDouble);
        printf("   %p\n", ptr_data->aPointer);
        printf(" }\n");
    }
    dynarr_free(&my_arr);
}