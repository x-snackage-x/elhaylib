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

int main() {
    bool test_result1 = true;

    printf("Linked List Test Suite Summary:\n");
    printf("%*s", TEST_NAMES_LENGTH, "Some Test: ");
    if(test_result1) {
        printf(GRN "S" RESET);
    } else {
        printf(RED "F" RESET);
    }
    printf("\n");
}