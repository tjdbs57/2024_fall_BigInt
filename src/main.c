#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

#include "bigint.h"

int main()
{
    srand(time(NULL));
    //test_bi_long_div();
    //test_right_shift_word();
    //test_left_shift_word();
   
    measure_time();
    return 0;
}