#include "test.h"
#include "utils.h"

void test_bi_show_hex() {

    // 1. 양수 big integer 테스트
    printf("Test 1: Positive bigint\n");
    bigint* x1 = NULL;
    bi_gen_rand(&x1, 0, 3);  // 양수, 워드 길이 3
    bi_show_hex(x1);  // 랜덤한 3워드 양수 출력
    bi_delete(&x1);

    // 2. 음수 big integer 테스트
    printf("Test 2: Negative bigint\n");
    bigint* x2 = NULL;
    bi_gen_rand(&x2, 1, 1);  // 음수, 워드 길이 1
    bi_show_hex(x2);  // 랜덤한 1워드 음수 출력
    bi_delete(&x2);

    // 3. 0 big integer 테스트
    printf("Test 3: Zero bigint\n");
    bigint* x3 = NULL;
    bi_new(&x3, 1);
    x3->a[0] = 0x00000000;  // 직접 0 할당
    bi_show_hex(x3);  // 0 출력
    bi_delete(&x3);

    // 4. NULL 포인터에 대한 테스트
    printf("Test 4: NULL bigint\n");
    bigint* x4 = NULL;
    bi_show_hex(x4);  // "Invalid bigint" 출력

    // 5. 음수 big integer 테스트 (여러 word)
    printf("Test 5: Negative bigint with multiple words\n");
    bigint* x5 = NULL;
    bi_gen_rand(&x5, 1, 2);  // 음수, 워드 길이 2
    bi_show_hex(x5);  // 랜덤한 2워드 음수 출력
    bi_delete(&x5);

    // 6. 2진수 입력 테스트
    printf("Test 6: Binary input\n");
    bigint* x6 = NULL;
    bi_new(&x6, 1);
    x6->a[0] = 0b110111;  // 직접 2진수 할당 (55 in decimal)
    x6->sign = 0;  // 양수
    bi_show_hex(x6);  // 0x00000037 출력
    bi_delete(&x6);

    // 7. 10진수 입력 테스트
    printf("Test 7: Decimal input\n");
    bigint* x7 = NULL;
    bi_new(&x7, 1);
    x7->a[0] = 12345;  // 12345 in decimal
    x7->sign = 0;  // 양수
    bi_show_hex(x7);  // 0x00003039 출력
    bi_delete(&x7);
}