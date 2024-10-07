/*
#include <stdio.h>
#include <stdlib.h>
#include "utils.h" // 필요 시 수정
#include "config.h" // 필요 시 수정

void test_compare() {
    bigint *x = NULL;
    bigint *y = NULL;

    // 1. bi_new로 두 bigint 구조체를 생성합니다.
    bi_new(&x, 4);
    bi_new(&y, 4);

    // 2. x와 y에 값 설정
    x->sign = NON_NEGATIVE; // x는 비부호
    y->sign = NON_NEGATIVE; // y는 비부호
    x->a[0] = 5; // x = 5
    y->a[0] = 10; // y = 10

    // 3. x와 y 비교
    printf("Comparing x and y (x < y):\n");
    int result = compare(x, y);
    if (result < 0) {
        printf("x is less than y\n");
    } else if (result > 0) {
        printf("x is greater than y\n");
    } else {
        printf("x is equal to y\n");
    }

    // 4. x를 y와 같은 값으로 설정
    y->a[0] = 5; // y = 5

    // 5. x와 y 비교
    printf("Comparing x and y (x == y):\n");
    result = compare(x, y);
    if (result == 0) {
        printf("x is equal to y\n");
    } else {
        printf("x is not equal to y\n");
    }

    // 6. y를 음수로 설정
    y->sign = 1; // y = -5

    // 7. x와 y 비교
    printf("Comparing x and y (x > -y):\n");
    result = compare(x, y);
    if (result > 0) {
        printf("x is greater than y\n");
    } else if (result < 0) {
        printf("x is less than y\n");
    } else {
        printf("x is equal to y\n");
    }

    // 8. 메모리 해제
    bi_delete(&x);
    bi_delete(&y);
}

int test() {
    test_compare(); // 비교 테스트 실행
    return 0;
}
*/
