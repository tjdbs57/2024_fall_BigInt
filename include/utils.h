#include "config.h"

void bi_new(bigint** x, u32 wordlen);
void bi_delete(bigint** x);

void bi_assign(bigint** y, bigint* x);
void bi_gen_rand(bigint**x, int sign, int wordlen);
void array_rand(word* dst, int wordlen);

/**
 * Compare the absolute values of two bigint numbers.
 * 
 * @param x Pointer to the first bigint number.
 * @param y Pointer to the second bigint number.
 * @return 1 if |x| > |y|, -1 if |x| < |y|, or 0 if |x| == |y|.
 */
int compareABS(bigint* x, bigint* y);

/**
 * Compare two bigint numbers, taking into account their signs.
 *
 * @param x Pointer to the first bigint number.
 * @param y Pointer to the second bigint number.
 * @return 1 if x > y, -1 if x < y, or 0 if x == y.
 */
int compare(bigint* x, bigint* y);