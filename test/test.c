#include "test.h"
#include "utils.h"

//test functions

// bi_show_hex 테스트 함수 구현
void test_bi_show_hex() {

   // 1. 양수 big integer 테스트
    printf("Test 1: Positive bigint\n");
    bigint* x1 = NULL;
    bi_new(&x1, 3);
    bi_gen_rand(&x1,NON_NEGATIVE,3);
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

}
