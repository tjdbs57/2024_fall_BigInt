#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

#include "bigint.h"
#include "log.h"


int main()
{
    srand(time(NULL));
    //test_set_by_array();
    //test_bi_string();
    //test_sub_single_word();
    //test_add();
    //test_sub_core();
    //measure_time();
    //test_mul_single_word();
    //test_left_shift_word();
    test_mul();
    //test_squc();
    return 0;
}