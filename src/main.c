#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"
#include "bigint.h"

int main()
{
    srand(time(NULL));
    //test_bi_string();
    test_swap_bigint();
    //printf("Hello world\n");
    //test_single_word();
    //test_add_carry();
    return 0;
}