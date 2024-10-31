#include "test.h"
#include "utils.h"

/*
void test_sub_borrow() {
    word A, B, borrow_in;
    word borrow_out, result;

    // Test case 1: A >= B
    A = 10; 
    B = 5; 
    borrow_in = 0; // No previous borrow
    sub_borrow(A, B, borrow_in, &borrow_out, &result);
    printf("Test Case 1: A = %u, B = %u, borrow_in = %u => result = %u, borrow_out = %u\n", 
           A, B, borrow_in, result, borrow_out); 
    // Expected: result = 5, borrow_out = 0

    // Test case 2: A < B
    A = 5; 
    B = 10; 
    borrow_in = 0;
    sub_borrow(A, B, borrow_in, &borrow_out, &result);
    printf("Test Case 2: A = %u, B = %u, borrow_in = %u => result = %u, borrow_out = %u\n", 
           A, B, borrow_in, result, borrow_out);
    // Expected: result = 4294967291 (assuming 32-bit unsigned wrap around), borrow_out = 1

    // Test case 3: A = B and borrow_in = 0
    A = 10; 
    B = 10; 
    borrow_in = 0;
    sub_borrow(A, B, borrow_in, &borrow_out, &result);
    printf("Test Case 3: A = %u, B = %u, borrow_in = %u => result = %u, borrow_out = %u\n", 
           A, B, borrow_in, result, borrow_out);
    // Expected: result = 0, borrow_out = 0

    // Test case 4: A = B and borrow_in = 1
    A = 10; 
    B = 10; 
    borrow_in = 1;
    sub_borrow(A, B, borrow_in, &borrow_out, &result);
    printf("Test Case 4: A = %u, B = %u, borrow_in = %u => result = %u, borrow_out = %u\n", 
           A, B, borrow_in, result, borrow_out);
    // Expected: result = 4294967295 (assuming 32-bit unsigned wrap around), borrow_out = 1

    // Test case 5: A = 0 and B = 0
    A = 0; 
    B = 0; 
    borrow_in = 0;
    sub_borrow(A, B, borrow_in, &borrow_out, &result);
    printf("Test Case 5: A = %u, B = %u, borrow_in = %u => result = %u, borrow_out = %u\n", 
           A, B, borrow_in, result, borrow_out);
    // Expected: result = 0, borrow_out = 0

    // Test case 6: A = 0 and B = 1
    A = 0; 
    B = 1; 
    borrow_in = 0;
    sub_borrow(A, B, borrow_in, &borrow_out, &result);
    printf("Test Case 6: A = %u, B = %u, borrow_in = %u => result = %u, borrow_out = %u\n", 
           A, B, borrow_in, result, borrow_out);
    // Expected: result = 4294967295 (assuming 32-bit unsigned wrap around), borrow_out = 1
}
*/
void test_subtraction() {
    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    // 큰 정수 x = 123456789, y = 987654321
    bi_new(&x, 2); // 2워드 길이의 빅넘버 생성
    x->a[0] = 123456789; // 낮은 32비트
    x->a[1] = 0; // 높은 32비트 (0으로 초기화)

    bi_new(&y, 2); // 2워드 길이의 빅넘버 생성
    y->a[0] = 987654321; // 낮은 32비트
    y->a[1] = 0; // 높은 32비트 (0으로 초기화)

    // 뺄셈 수행
    sub_core(&x, &y, &z);

    // 결과 출력
    printf("Result: ");
    for (int i = 0; i < z->wordlen; i++) {
        printf("%u ", z->a[i]); // 결과 출력
    }
    printf("\n");

    // 예상 결과 확인
    // 987654321 - 123456789 = 864197532
    // 부호를 무시하고 unsigned로 보면 3435769764가 나옴 (unsigned wrap around)
    if (z->wordlen > 0) {
        printf("Expected Result: 3435769764\n");
    }

    // 메모리 해제
    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&z);
}


void test_add_include_negative()
{
    bigint *x = NULL;
    bigint *y = NULL;
    bigint *z = NULL;

    // 1. 기본 테스트 (부호가 같은 경우)
    int wordlen1 = rand() % 10 + 1; // 1에서 10 사이의 랜덤 길이
    int wordlen2 = rand() % 10 + 1; // 1에서 10 사이의 랜덤 길이
    int sign = NON_NEGATIVE;

    // 같은 부호로 두 bigint 생성
    bi_gen_rand(&x, sign, wordlen1);
    bi_gen_rand(&y, sign, wordlen2);
    printf("Same sign test:\n");
    printf("First bigint: ");
    bi_show_hex(x);
    printf("Second bigint: ");
    bi_show_hex(y);
    sub_core(&x, &y, &z);
    printf("Result (First - Second): ");
    bi_show_hex(z);
    bi_delete(&z); // 결과 메모리 해제

    // 2. 기본 테스트 (부호가 다른 경우)
    sign = NEGATIVE; // 두 번째 bigint에 대해 부호를 바꿔줍니다.
    bi_gen_rand(&y, sign, wordlen2);
    printf("Different sign test:\n");
    printf("First bigint: ");
    bi_show_hex(x);
    printf("Second bigint: ");
    bi_show_hex(y);
    sub_core(&x, &y, &z);
    printf("Result (First - Second): ");
    bi_show_hex(z);
    bi_delete(&z); // 결과 메모리 해제

    // 3. 극단적인 경우 (한 쪽 bigint가 0인 경우)
    bi_delete(&y); // 이전 y 메모리 해제
    y = NULL; // y를 NULL로 초기화
    bi_gen_rand(&x, sign, wordlen1); // 랜덤하게 생성
    printf("Extreme case test (y = 0):\n");
    printf("First bigint: ");
    bi_show_hex(x);
    printf("Second bigint (0): ");
    bi_show_hex(y); // NULL일 경우 어떻게 처리할지 구현해야 함
    sub_core(&x, &y, &z);
    printf("Result (First - Second): ");
    bi_show_hex(z);
    bi_delete(&z); // 결과 메모리 해제

    // 4. 극단적인 경우 (두 bigint 모두 같은 값)
    bi_gen_rand(&y, sign, wordlen1); // 같은 길이와 부호로 생성
    printf("Extreme case test (x = y):\n");
    printf("First bigint: ");
    bi_show_hex(x);
    printf("Second bigint: ");
    bi_show_hex(y);
    sub_core(&x, &y, &z);
    printf("Result (First - Second): ");
    bi_show_hex(z);
    bi_delete(&z); // 결과 메모리 해제

    // 메모리 해제
    bi_delete(&x);
    bi_delete(&y);
}
