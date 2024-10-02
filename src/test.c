#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

// Function to create a bigint for testing
bigint* create_bigint(u8 sign, u32 wordlen, word* values) {
    bigint* x = (bigint*)malloc(sizeof(bigint));
    if (x == NULL) return NULL; // memory allocation failed

    x->sign = sign;
    x->wordlen = wordlen;
    x->a = (word*)malloc(wordlen * sizeof(word));
    if (x->a == NULL) {
        free(x);
        return NULL; // memory allocation failed
    }

    for (u32 i = 0; i < wordlen; i++) {
        x->a[i] = values[i];
    }

    return x;
}


void free_bigint(bigint* x) {
    if (x) {
        free(x->a);
        free(x);
    }
}


void test_bi_show_hex(){
     
    // Test case 1: Positive bigint
    word values1[] = {0x00000001, 0x00000002, 0x00000003}; // 0x030201
    bigint* b1 = create_bigint(0, 3, values1);
    printf("Test Case 1: Positive bigint: ");
    bi_show_hex(b1);
    free_bigint(b1);

    // Test case 2: Negative bigint
    word values2[] = {0x00000001, 0x00000002, 0x00000003}; // -0x030201
    bigint* b2 = create_bigint(1, 3, values2);
    printf("Test Case 2: Negative bigint: ");
    bi_show_hex(b2);
    free_bigint(b2);

    // Test case 3: Zero bigint
    word values3[] = {0x00000000}; // 0x0
    bigint* b3 = create_bigint(0, 1, values3);
    printf("Test Case 3: Zero bigint: ");
    bi_show_hex(b3);
    free_bigint(b3);

    // Test case 4: Invalid bigint (NULL pointer)
    printf("Test Case 4: Invalid bigint (NULL pointer): ");
    bi_show_hex(NULL);

    // Test case 5: Invalid bigint (empty)
    bigint empty = {0, 0, NULL};
    printf("Test Case 5: Invalid bigint (empty): ");
    bi_show_hex(&empty);

    return 0;
}