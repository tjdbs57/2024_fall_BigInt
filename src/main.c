#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

#include "bigint.h"

#include "arithmetic.h"
int main()
{
    //srand(time(NULL));
    //test_set_by_array();
    //test_bi_string();
    //test_sub_single_word();
    //test_add();
    //test_sub_core();
    test_sub(); 
    //test_addition();

    /*bigint *num1 = NULL, *num2 = NULL, *result = NULL;

    // num1 초기화 및 값 설정
    bi_new(&num1, 1);
    bi_set_one(&num1);
    //num1->a[0] = 0x1;  // num1 = 1
    num1->sign = NEGATIVE;
    bi_show_hex(num1); // 수정: &num1 -> num1
    printf("\n");

    // num2 초기화 및 값 설정
    bi_new(&num2, 1);
    num2->a[0] = 0x1;  // num2 = 1
    num2->sign = NON_NEGATIVE;
    bi_show_hex(num2); // 수정: &num2 -> num2
    printf("\n");

    if (num2->wordlen == 1 && num2->a[0] == 0) {
        bi_set_zero(&num2); // 유지: &num2 사용
    }

    // 두 수의 합 계산
    add(&num1, &num2, &result);

    if (result->wordlen == 1 && result->a[0] == 0) {
        bi_set_zero(&result); // 수정: result -> &result
    }

    // 결과 출력
    printf("result: ");
    bi_show_hex(result);
    printf("\n");

    // 메모리 해제
    bi_delete(&num1);
    bi_delete(&num2);
    bi_delete(&result);*/

    measure_time();
    return 0;
}