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