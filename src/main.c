#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

#include "bigint.h"


#define MEASURE_CLOCK_CYCLES(func, x, y, z) \
    measure_cycles(func, &x, &y, &z);

#define TEST_CLOCK_CYCLES() \
    for(int i = 0; i < TEST_CASE; i++) { \
        bigint *x = NULL; \
        bigint *y = NULL; \
        bigint *z = NULL; \
        \
        int sign1 = rand() % 2; \
        int sign2 = rand() % 2; \
        int wordlen = (MAX_BIT_LEN / WORD_BITLEN); \
        bi_gen_rand(&x, sign1, wordlen); \
        bi_gen_rand(&y, sign2, wordlen); \
        \
        MEASURE_CLOCK_CYCLES(mul_core_tx, x, y, z); \
        MEASURE_CLOCK_CYCLES(mul_core_improved, x, y, z); \
        MEASURE_CLOCK_CYCLES(mul_core_karatsuba, x, y, z); \
        /*MEASURE_CLOCK_CYCLES(add, x, y, z);*/ \
        \
        bi_delete(&x); \
        bi_delete(&y); \
        bi_delete(&z); \
    }


int main()
{
    srand((u32)time(NULL));
    //test();
    //test_basic_operation(add, "+", "add");
    //test_basic_operation(sub, "-", "sub");
    // test_basic_operation(mul_core_tx, "*", "mul_tx");
    // test_basic_operation(mul_core_improved, "*", "mul_improved");
    //test_basic_operation(mul_core_karatsuba, "*", "mul_karatsuba");
    //test_div("div");
    //test_exp_mod(L2R, "L2R");
    //test_exp_mod(R2L, "R2L");
    //test_exp_mod(exp_mod_montgomery, "exp_mod_montgomery");
    //test_barret("reduction");
    ////TEST_CLOCK_CYCLES();
    //test_squ(squaring, "squ_tx");
    //test_squ(squ_karatsuba, "squ_karatsuba");
    //measure();
    
    //measure_clock_cycles();
    //test_set_by_array();
    //test_bi_string();
    return 0;
}
