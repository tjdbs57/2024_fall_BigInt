#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"


// bigint의 내용을 출력하는 함수
void print_bigint(bigint* x) {
    if (x == NULL) {
        printf("bigint is NULL\n");
        return;
    }

    printf("Sign: %d\n", x->sign);
    printf("Word Length: %u\n", x->wordlen);
    printf("Array: ");
    for (u32 i = 0; i < x->wordlen; i++) {
        printf("%08x ", x->a[i]);
    }
    printf("\n");
}

void test_rand_assign() {
    bigint* original_bigint = NULL;
    bigint* assigned_bigint = NULL;
    int wordlen = 3; // 원하는 word 개수
    int sign = rand() % 2; // 랜덤으로 0 또는 1 선택 (NON_NEGATIVE 또는 NEGATIVE)

    // 랜덤 bigint 생성
    bi_gen_rand(&original_bigint, sign, wordlen);

    // 할당된 bigint 출력
    printf("Original bigint:\n");
    printf("Sign: %d\n", original_bigint->sign);
    printf("Word Length: %u\n", original_bigint->wordlen);
    printf("Array: ");
    for (u32 i = 0; i < original_bigint->wordlen; i++) {
        printf("%u ", original_bigint->a[i]);
    }
    printf("\n");

    // bi_assign 함수로 값 할당
    bi_assign(&assigned_bigint, original_bigint);

    // 할당된 bigint 출력
    printf("Assigned bigint:\n");
    printf("Sign: %d\n", assigned_bigint->sign);
    printf("Word Length: %u\n", assigned_bigint->wordlen);
    printf("Array: ");
    for (u32 i = 0; i < assigned_bigint->wordlen; i++) {
        printf("%u ", assigned_bigint->a[i]);
    }
    printf("\n");

    // 메모리 해제
    bi_delete(&original_bigint);
    bi_delete(&assigned_bigint);
}


void test_set_by_string() {
    char test_string[256]; // 최대 255자 + 널 문자

    printf("Enter a number (can be in decimal, hexadecimal (0x), or binary (0b)): ");
    fgets(test_string, sizeof(test_string), stdin);
    
    // fgets로 읽은 문자열에서 개행 문자 제거
    test_string[strcspn(test_string, "\n")] = 0; 

    bigint* x;
    i32 sign;
    bi_set_by_string(&x, sign, test_string, 10); // 문자열로 변환
    printf("Assigned bigint from string:\n");
    printf("Sign: %d\n", x->sign);
    printf("Word Length: %d\n", x->wordlen);
    printf("Array: ");
    for (int i = 0; i < x->wordlen; i++) {
        printf("%08x ", x->a[i]);
    }
    printf("\n");
    bi_delete(&x); // 메모리 해제
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
    print_bigint(x);

    // 메모리 해제
    bi_delete(&x);
}