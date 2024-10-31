#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"
<<<<<<< HEAD
#include "bigint.h"

int main()
{
    srand(time(NULL));
    //test_set_by_array();
    test_bi_string();
    //test_add();
=======

int main()
{
    //printf("Hello world jieon\n");
    //test_bi_show_hex();
    //test_get_sign_bit();
    //test_flip_sign_bit();
    //test_get_word_length();
    //test_get_bit_length();
    //test_left_shift();
    //test_right_shift();
    test_reduction();

>>>>>>> 4a215fd562bfc2edbb6beaf9a17c670ae369546c
    return 0;
}