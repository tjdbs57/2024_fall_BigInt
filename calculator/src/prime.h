#ifndef PRIME_H  // 중복 정의 방지
#define PRIME_H

#include "bigint.h"
#include "arithmetic.h"
#include "utils.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
void bi_set_random_in_range(bigint **b, char *lower_bound, bigint *upper_bound);
int miller_rabin_test(bigint *n, bigint *b, int s, bigint *t);
int is_even(const bigint *x);
int is_prime(bigint *n, int k);
void generate_large_prime(bigint **prime, int bit_length, int k);

#endif 