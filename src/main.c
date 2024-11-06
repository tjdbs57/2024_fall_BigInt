#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

#include "bigint.h"

int main()
{
    srand(time(NULL));
    //test_set_by_array();
    //test_bi_string();
    //test_sub_single_word();
    //test_add();
    //test_sub_core();
    //test_sub(); 
    test_addition();
    //test_right_shift_word();
    //test_left_shift_word();

    /*bigint* A=NULL;
    bi_new(&A,1);
    A->a[0]=0xf0000000;
    int is_A_zero = is_zero(A);
    printf("%d\n",is_A_zero);*/
    

    measure_time();
    return 0;
}