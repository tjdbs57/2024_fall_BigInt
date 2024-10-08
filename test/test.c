#include "test.h"
#include "utils.h"

//test functions
void test_rand_assign() {
    bigint* original_bigint = NULL;
    bigint* assigned_bigint = NULL;
    int wordlen = 3; // Desired word count
    int sign = rand() % 2; // Randomly select 0 or 1 (NON_NEGATIVE or NEGATIVE)

    // Generate random bigint
    bi_gen_rand(&original_bigint, sign, wordlen);

    // Print assigned bigint
    bi_show_hex(original_bigint);
    printf("\n");

    // Assign value using bi_assign function
    bi_assign(&assigned_bigint, original_bigint);

    // Print assigned bigint
    bi_show_hex(assigned_bigint);
    printf("\n");

    // Free memory
    bi_delete(&original_bigint);
    bi_delete(&assigned_bigint);
}

void test_set_by_array() {
    bigint* x = NULL;
    word arr[] = {0x12345678, 0xaabbccdd}; // Example array
    int sign = NON_NEGATIVE; // Set sign
    int wordlen = sizeof(arr) / sizeof(arr[0]); // Calculate array length

    // Set bigint using bi_set_by_array
    bi_set_by_array(&x, sign, arr, wordlen);

    // Print result
    printf("Assigned bigint from array:\n");
    bi_show_hex(x);

    // Free memory
    bi_delete(&x);
}

void test_bigint_functions() {
    bigint* num1 = NULL;
    bigint* num2 = NULL;

    // Test bi_set_by_array
    word array[] = {0x12345678, 0x9ABCDEF0}; // Sample values
    int sign1 = NON_NEGATIVE;
    bi_set_by_array(&num1, sign1, array, 2);
    printf("Testing bi_set_by_array:\n");
    bi_show_hex(num1);
    printf("\n");

    // // Test bi_set_by_string
    // char* str = "123456789ABCDEF0";
    // int sign2 = NEGATIVE;
    // bi_set_by_string(&num2, sign2, str, 16); // Set string as hexadecimal
    // printf("Testing bi_set_by_string:\n");
    // bi_show_hex(num2);
    // printf("\n");

    // Free memory
    bi_delete(&num1);
    bi_delete(&num2);
}

void test_bi_new()
{
    bigint* num = NULL;
    int wordlen = 10;  // 매우 큰 값으로 메모리 할당 실패 유도

    // bi_new 호출 (메모리 할당 실패 유도)
    bi_new(&num, wordlen);  // 여기서 메모리 할당 실패 시 오류 메시지가 출력되어야 함

    printf("Test failed, memory allocation did not fail as expected.\n");

    // 할당된 메모리 해제
    bi_delete(&num);
}

void test_bi_string()
{
    bigint* original_bigint = NULL;
    int sign = rand() % 2; // Randomly select 0 or 1 (NON_NEGATIVE or NEGATIVE)
    
    // 테스트할 문자열 예시 (16진수 문자열)
    char* test_str = "123456789abcde"; // 예시 문자열
    int base = 16; // 16진수로 설정

    // bi_set_by_string 호출
    if (bi_set_by_string(&original_bigint, sign, test_str, base) != 0) {
        printf("Failed to set bigint from string.\n");
        return;
    }

    bi_show_hex(original_bigint);
    bi_delete(&original_bigint);
}
