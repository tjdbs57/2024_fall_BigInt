#include "config.h"

void bi_new(bigint** x, u32 wordlen);
void bi_delete(bigint** x);

void bi_assign(bigint** y, bigint* x);
void bi_gen_rand(bigint**x, int sign, int wordlen);
void array_rand(word* dst, int wordlen);

