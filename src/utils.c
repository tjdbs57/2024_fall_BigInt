#include "utils.h"


void bi_new(bigint** x, u32 wordlen)
{
    //if(*x != NULL)
    //bi_delete(x);
    *x = (bigint*)malloc(sizeof(bigint));
    (*x)->sign = NON_NEGATIVE; // 0
    (*x)->wordlen = wordlen;
    (*x)->a = (word*)calloc(wordlen, sizeof(word));
}
