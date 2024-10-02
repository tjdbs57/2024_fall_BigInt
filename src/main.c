#include <stdio.h>
#include "config.h"
#include "utils.h"

int main()
{
    int a;
    printf("size : %llu\n", sizeof(bigint));
    printf("hello world!\n");
    printf("hello jieon!\n");

    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    bi_new(&x, 4);
    return 0;
}