#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

#include "bigint.h"
#include "log.h"
/*
int main() {
    bigint *x = NULL, *y = NULL, *z = NULL;
        int sign = rand() % 2;
        int wordlen = 313;
    // 필요한 초기화 (예: bi_set_by_string 등)
    // ...
    bi_gen_rand(&x, sign, wordlen);
    bi_gen_rand(&y, sign, wordlen);
    // measure_mem을 호출, add_func 전달
    measure_mem(add, &x, &y, &z);

    // 정리
    // bi_delete 등으로 메모리 해제
    return 0;
}
*/
int main()
{
    srand(time(NULL));
    //test_set_by_array();
    //test_bi_string();
    //test_sub_single_word();
    //test_add();
    //test_sub_core();
    //test_sub(); 
    //test_addition();
    //test_right_shift_word();
    //test_left_shift_word();
    //test_mul();
    //test_squc();;
    measure();
    //test_bi_long_div();
    return 0;
}
