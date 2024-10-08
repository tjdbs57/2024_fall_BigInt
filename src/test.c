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
    number->sign = NEGATIVE; // Set to non_negative
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
    number3->sign = NEGATIVE; // Set to non-negative
    printf("Original sign: %d\n", get_sign_bit(number3)); // Should print 0
    flip_sign_bit(number3);
    printf("Flipped sign: %d\n", get_sign_bit(number3)); // Should print 1
    flip_sign_bit(number3);
    printf("Flipped sign again: %d\n", get_sign_bit(number3)); // Should print 0
}

void test_get_word_length(){

    bigint* num1 = NULL;
    bi_new(&num1, 5); // 단어 길이를 5로 새로 생성

    printf("Word Length: %d\n", get_word_length(num1)); // 예상 출력: Word Length: 5

    bi_delete(&num1); // 메모리 해제
}

void test_get_bit_length() {
    // Create test cases
    bigint* num1 = NULL;
    bi_new(&num1, 1); // Allocate space for 1 word
    if (!num1) {
        fprintf(stderr, "Failed to allocate bigint for num1\n");
        return;
    }
    num1->a[0] = 0; // Representing zero
    num1->sign = NON_NEGATIVE;

    bigint* num2 = NULL;
    bi_new(&num2, 1);
    if (!num2) {
        fprintf(stderr, "Failed to allocate bigint for num2\n");
        return;
    }
    num2->a[0] = 1; // Representing 1
    num2->sign = NON_NEGATIVE;

    bigint* num3 = NULL;
    bi_new(&num3, 1);
    if (!num3) {
        fprintf(stderr, "Failed to allocate bigint for num3\n");
        return;
    }
    num3->a[0] = 0xFFFFFFFF; // Maximum 32-bit unsigned value
    num3->sign = NON_NEGATIVE;

    bigint* num4 = NULL;
    bi_new(&num4, 1);
    if (!num4) {
        fprintf(stderr, "Failed to allocate bigint for num4\n");
        return;
    }
    num4->a[0] = 1; // Representing 1
    num4->sign = NEGATIVE;

    bigint* num5 = NULL;
    bi_new(&num5, 2);
    if (!num5) {
        fprintf(stderr, "Failed to allocate bigint for num5\n");
        return;
    }
    num5->a[0] = 0xFFFFFFFE; // 4294967294
    num5->a[1] = 0xFFFFFFFD; // 4294967293
    num5->sign = NEGATIVE;

    printf("Test 1 - Bit Length of ");
    bi_show_hex(num1); // Print the hexadecimal representation
    printf("%d \n", get_bit_length(num1));

    printf("Test 2 - Bit Length of ");
    bi_show_hex(num2); // Print the hexadecimal representation
    printf("%d \n", get_bit_length(num2));

    printf("Test 3 - Bit Length of ");
    bi_show_hex(num3); // Print the hexadecimal representation
    printf("%d \n", get_bit_length(num3));

    printf("Test 4 - Bit Length of ");
    bi_show_hex(num4); // Print the hexadecimal representation
    printf("%d \n", get_bit_length(num4));

    printf("Test 5 - Bit Length of ");
    bi_show_hex(num5); // Print the hexadecimal representation
    printf("%d \n", get_bit_length(num5));
   
    // Clean up
    bi_delete(&num1);
    bi_delete(&num2);
    bi_delete(&num3);
    bi_delete(&num4);
    bi_delete(&num5);
}


void test_left_shift() {
    bigint* num;
    bi_new(&num, 2); // bigint 생성

    num->a[0] = 0x00000000;  // 예시 값
    num->a[1] = 0x00000001;  // 예시 값

    printf("Before left shift:\n");
    bi_show_hex(num);

    // 왼쪽으로 5비트 시프트
    left_shift(num, 5);

    printf("After left shift by 5 bits:\n");
    bi_show_hex(num);

    // 메모리 해제
    bi_delete(&num);
}

void test_right_shift() {
    bigint* num;
    bi_new(&num, 2); // bigint 생성

    num->a[0] = 0xFFFFFFFF;  // 예시 값
    num->a[1] = 0x00000001;  // 예시 값

    printf("Before right shift:\n");
    bi_show_hex(num);

    // 오른쪽으로 5비트 시프트
    right_shift(num, 5);

    printf("After right shift by 5 bits:\n");
    bi_show_hex(num);

    // 메모리 해제
    bi_delete(&num);
}

void test_reduction() {
    bigint *num, *result;
    bi_new(&num, 3); // Create a bigint with 3 words

    // Test case 1: Modulus with r = 32 (exactly one word)
    num->a[0] = 0x000000FF; 
    num->a[1] = 0x00000001; 
    num->a[2] = 0x00000002;
    
    bi_new(&result, 0); // Create an empty result bigint
    reduction(num, 32, result); // Perform modulus with r = 32
    
    printf("Test case 1: Modulus by 32 bits:\n");
    bi_show_hex(result); // Expected: 0x000000FF
    bi_delete(&result);

    // Test case 2: Modulus with r = 32 (maximum single word)
    num->a[0] = 0xFFFFFFFF; 
    bi_new(&result, 0);
    reduction(num, 32, result); // Perform modulus with r = 32
    
    printf("Test case 2: Modulus by 32 bits (max value):\n");
    bi_show_hex(result); // Expected: 0xFFFFFFFF
    bi_delete(&result);

    // Test case 3: Modulus with r = 32 (zero in higher words)
    num->a[0] = 0x00000001; 
    num->a[1] = 0x00000000; 
    num->a[2] = 0x00000000;
    bi_new(&result, 0);
    reduction(num, 32, result); // Perform modulus with r = 32
    
    printf("Test case 3: Modulus by 32 bits (higher words zero):\n");
    bi_show_hex(result); // Expected: 0x00000001
    bi_delete(&result);

    // Test case 4: Modulus with r = 32 (higher word non-zero)
    num->a[0] = 0x00000000; 
    num->a[1] = 0x00000002; 
    num->a[2] = 0x00000000;
    bi_new(&result, 0);
    reduction(num, 32, result); // Perform modulus with r = 32
    
    printf("Test case 4: Modulus by 32 bits (higher word non-zero):\n");
    bi_show_hex(result); // Expected: 0x00000000
    bi_delete(&result);

    // Test case 5: Modulus with r = 32 (all words are the same)
    num->a[0] = 0x00000005; 
    num->a[1] = 0x00000005; 
    num->a[2] = 0x00000005;
    bi_new(&result, 0);
    reduction(num, 32, result); // Perform modulus with r = 32
    
    printf("Test case 5: Modulus by 32 bits (all words same):\n");
    bi_show_hex(result); // Expected: 0x00000005
    bi_delete(&result);

    // Test case 6: Modulus with r = 32 (overflow scenario)
    num->a[0] = 0x00000000; 
    num->a[1] = 0xFFFFFFFF; 
    num->a[2] = 0xFFFFFFFF;
    bi_new(&result, 0);
    reduction(num, 32, result); // Perform modulus with r = 32
    
    printf("Test case 6: Modulus by 32 bits (overflow scenario):\n");
    bi_show_hex(result); // Expected: 0xFFFFFFFF
    bi_delete(&result);

    // Test case 7: Modulus with r = 32 (max integer representation)
    num->a[0] = 0x7FFFFFFF; 
    num->a[1] = 0x00000000; 
    num->a[2] = 0x00000000;
    bi_new(&result, 0);
    reduction(num, 32, result); // Perform modulus with r = 32
    
    printf("Test case 7: Modulus by 32 bits (max int):\n");
    bi_show_hex(result); // Expected: 0x7FFFFFFF
    bi_delete(&result);

    // Clean up
    bi_delete(&num);
}

