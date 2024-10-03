#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

int main()
{
    /* 
    int a;
    printf("size : %llu\n", sizeof(bigint));
    printf("hello world!\n");

    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    bi_new(&x, 4);
    */
   
    //test_bi_show_hex();

    test_get_jth_bit();

    return 0;
}

