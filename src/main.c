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
    test_add();
    return 0;
}