#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

// bi_show_hex 테스트 함수 구현
void test_bi_show_hex() {

   // 1. 양수 big integer 테스트
    printf("Test 1: Positive bigint\n");
    bigint* x1 = NULL;
    bi_new(&x1, 3);
    x1->a[0] = 0x12345678;
    x1->a[1] = 0x9ABCDEF0;
    x1->a[2] = 0xFEDCBA98;
    bi_show_hex(x1);  // 예상 출력: 0xFEDCBA989ABCDEF012345678
    bi_delete(&x1);

    // 2. 음수 big integer 테스트
    printf("Test 2: Negative bigint\n");
    bigint* x2 = NULL;
    bi_new(&x2, 1);
    x2->sign = 1;  // 음수
    x2->a[0] = 0xFFFFFFFF;
    bi_show_hex(x2);  // 예상 출력: -ffffffff
    bi_delete(&x2); 

    // 3. 0 big integer 테스트
    printf("Test 3: Zero bigint\n");
    bigint* x3 = NULL;
    bi_new(&x3, 1);
    x3->a[0] = 0x00000000;
    bi_show_hex(x3);  // 예상 출력: 00000000
    bi_delete(&x3);

    // 4. NULL 포인터에 대한 테스트
    printf("Test 4: NULL bigint\n");
    bigint* x4 = NULL;
    bi_show_hex(x4); // 예상 출력: Invalid bigint

    // 5. 음수 big integer 테스트 (여러 word)
    printf("Test 5: Negative bigint with multiple words\n");
    bigint* x5 = NULL;
    bi_new(&x5, 2);
    x5->sign = -1;  // 음수
    x5->a[0] = 0x012345678;
    x5->a[1] = 0x9abcdef;
    bi_show_hex(x5);  // 예상 출력: -9abcdef012345678
    bi_delete(&x5);

    // 6. 2진수 입력 테스트
    printf("Test 6: Binary input\n");
    bigint* x6 = NULL;
    bi_new(&x6, 2);
    x6->a[0] = 0b110111; // 55 in decimal
    x6->sign = 0;  // 양수
    bi_show_hex(x6);  // 예상 출력: 0x0000000000000037
    bi_delete(&x6);

    // 7. 10진수 입력 테스트
    printf("Test 7: Decimal input\n");
    bigint* x7 = NULL;
    bi_new(&x7, 2);
    x7->a[0] = 12345; // 12345 in decimal
    x7->sign = 0;  // 양수
    bi_show_hex(x7);  // 예상 출력: 0x0000000000003039
    bi_delete(&x7);
}

void test_get_jth_bit(){

    bigint* num =NULL;
    bi_new(&num, 2); // Assume we create a bigint with 2 words
    num->a[0] = 0xFFFFFFFF; // Set some values
    num->a[1] = 0x00000000;

    // Get the 0th bit
    int bit_value = get_jth_bit(num, 31); // Returns 1
    printf("Bit 0: %d\n", bit_value);

    // Clean up
    bi_delete(&num);

}

void test_get_sign_bit(){
    bigint* number;
    bi_new(&number, 1);
    number->sign = NON_NEGATIVE; // Set to non_negative
    number->a[0] = 0x012345678;

    if (get_sign_bit(number) == NON_NEGATIVE) {
        printf("The number is non-negative.(0)\n");
    } else {
        printf("The number is negative.(1)\n");
    }

}

void test_flip_sign_bit(){

    printf("test case 1 : \n");
    bigint* number1 = NULL;
    bi_new(&number1, 1); // Assuming this creates a bigint
    number1->sign = NON_NEGATIVE; // Set to non-negative
    printf("Original sign: %d\n", get_sign_bit(number1)); // Should print 0
    flip_sign_bit(number1);
    printf("Flipped sign: %d\n", get_sign_bit(number1)); // Should print 1
    flip_sign_bit(number1);
    printf("Flipped sign again: %d\n", get_sign_bit(number1)); // Should print 0
    
    printf("\ntest case 2 : \n");
    bigint* number2 = NULL;
    bi_new(&number2, 1); // Assuming this creates a bigint
    number2->sign = 0; // Set to non-negative
    printf("Original sign: %d\n", get_sign_bit(number2)); // Should print 0
    flip_sign_bit(number2);
    printf("Flipped sign: %d\n", get_sign_bit(number2)); // Should print 1
    flip_sign_bit(number2);
    printf("Flipped sign again: %d\n", get_sign_bit(number2)); // Should print 0

    printf("\ntest case 3 : \n");
    bigint* number3 = NULL;
    bi_new(&number3, 1); // Assuming this creates a bigint
    number3->sign = 1; // Set to non-negative
    printf("Original sign: %d\n", get_sign_bit(number3)); // Should print 0
    flip_sign_bit(number3);
    printf("Flipped sign: %d\n", get_sign_bit(number3)); // Should print 1
    flip_sign_bit(number3);
    printf("Flipped sign again: %d\n", get_sign_bit(number3)); // Should print 0
}