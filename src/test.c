#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"



void test_rand_assign() {
    bigint* original_bigint = NULL;
    bigint* assigned_bigint = NULL;
    int wordlen = 3; // 원하는 word 개수
    int sign = rand() % 2; // 랜덤으로 0 또는 1 선택 (NON_NEGATIVE 또는 NEGATIVE)

    // 랜덤 bigint 생성
    bi_gen_rand(&original_bigint, sign, wordlen);

    // 할당된 bigint 출력
    bi_show_hex(original_bigint);
    printf("\n");

    // bi_assign 함수로 값 할당
    bi_assign(&assigned_bigint, original_bigint);

    // 할당된 bigint 출력
    bi_show_hex(assigned_bigint);
    printf("\n");

    // 메모리 해제
    bi_delete(&original_bigint);
    bi_delete(&assigned_bigint);
}



void test_set_by_array() 
{
    bigint* x = NULL;
    word arr[] = {0x12345678, 0xaabbccdd}; // 예제 배열
    int sign = NON_NEGATIVE; // 부호 설정
    int wordlen = sizeof(arr) / sizeof(arr[0]); // 배열 길이 계산

    // bi_set_by_array를 호출하여 bigint 설정
    bi_set_by_array(&x, sign, arr, wordlen);

    // 결과 출력
    printf("Assigned bigint from array:\n");
    bi_show_hex(x);

    // 메모리 해제
    bi_delete(&x);
}

void test_bigint_functions() {
    bigint* num1 = NULL;
    bigint* num2 = NULL;

    // bi_set_by_array 테스트
    word array[] = {0x12345678, 0x9ABCDEF0}; // 예시 값
    int sign1 = NON_NEGATIVE;
    bi_set_by_array(&num1, sign1, array, 2);
    printf("Testing bi_set_by_array:\n");
    bi_show_hex(num1);
    printf("\n");

    // // bi_set_by_string 테스트
    // char* str = "123456789ABCDEF0";
    // int sign2 = NEGATIVE;
    // bi_set_by_string(&num2, sign2, str, 16); // 16진수로 문자열 설정
    // printf("Testing bi_set_by_string:\n");
    // bi_show_hex(num2);
    // printf("\n");

    // 메모리 해제
    bi_delete(&num1);
    bi_delete(&num2);
}

