#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"
#include "bigint.h"

int main()
{
    //test_bi_refine();
    test_add_core();
    test_swap_bigint();

    return 0;
}