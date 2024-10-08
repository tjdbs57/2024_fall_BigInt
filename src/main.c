#include <stdio.h>
#include "config.h"
#include "utils.h"
#include "test.h"

int main()
{
<<<<<<< Updated upstream
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
    //test_get_jth_bit();
    //test_get_sign_bit();
    //test_flip_sign_bit();
    //test_get_word_length();
    //test_get_bit_length();
    //test_left_shift();
    //test_right_shift();
    test_reduction();
=======
    printf("Hello world\n");
    
    test_bi_show_hex();
>>>>>>> Stashed changes
    
    return 0;
}

