#include "prime.h"

#define WORD_BIT     64
void bi_set_random_in_range(bigint **b, char *lower_bound, bigint *upper_bound) {
    bigint *lower = NULL, *tmp = NULL, *range = NULL, *rand_value = NULL;

    bi_set_by_string(&lower, NON_NEGATIVE, lower_bound, 16);
    //bi_show_hex(lower);
    sub(&upper_bound, &lower, &range);
    //bi_show_hex(range);
    if (range == NULL || range->wordlen == 0) {
        fprintf(stderr, "Error: Invalid range.\n");
        bi_delete(&lower);
        return;
    }

    do {
        bi_gen_rand(&rand_value, NON_NEGATIVE, range->wordlen);
        //printf("comapre\n");
        //bi_show_hex(rand_value);
        if (compare(rand_value, range) > 0) 
        {break;}
        //printf("ldkfs\n");
        bi_delete(&rand_value);

    } while (1);

    add(&rand_value, &lower, &tmp);
    bi_assign(b, tmp);

    bi_delete(&lower);
    bi_delete(&tmp);
    bi_delete(&range);
    bi_delete(&rand_value);
}

// Miller-Rabin Test
int miller_rabin_test(bigint *n, bigint *b, int s, bigint *t) {
    bigint *x = NULL;
    exp_mod_montgomery(&b, &t, &x, &n);  // x = b^t % n
    bigint *n_minus_1 = NULL;
    bigint *one = NULL, *two = NULL;
    bi_set_one(&one);
    bi_set_by_string(&two, NON_NEGATIVE, "2", 16);
    sub(&n, &one, &n_minus_1);  // n_minus_1 = n - 1

    // 초기 조건: x == 1 or x == n - 1
    if (compare(x, one) == 0 || compare(x, n_minus_1) == 0) {
        bi_delete(&x);
        bi_delete(&n_minus_1);
        bi_delete(&one);
        return 1;  // n은 소수일 가능성 있음
    }

    // 반복적으로 제곱
    bigint *tmp_x = NULL;
    for (int i = 0; i < s - 1; i++) {
        exp_mod_montgomery(&x, &two, &tmp_x, &n);  // x = x^2 % n
        bi_assign(&x, tmp_x);

        if (compare(x, n_minus_1) == 0) {
            bi_delete(&x);
            bi_delete(&n_minus_1);
            bi_delete(&one);
            return 1;  // n은 소수일 가능성 있음
        }
    }

    // n이 합성수일 가능성이 높음
    bi_delete(&x);
    bi_delete(&n_minus_1);
    bi_delete(&one);
    return 0;
}

int is_even(const bigint *x) {
    if (x == NULL || x->wordlen == 0) return 0;  // NULL 포인터 처리 및 빈 bigint 처리

    // BigInt의 가장 마지막 워드 확인 (x->a[x->wordlen - 1]은 마지막 워드)
    word last_word = x->a[x->wordlen - 1];

    // 마지막 워드의 최하위 비트가 0이면 짝수
    return (last_word & 1) == 0;
}


int is_prime(bigint *n, int k) {
    bigint *two = NULL;
    bi_set_by_string(&two, NON_NEGATIVE, "2", 16);

    if (compare(n, two) == 0) {
        //printf("Special case: n == 2 (prime).\n");
        return 1;  // 2는 소수
    }
    if (compare(n, two) == -1) {
        //printf("Special case: n < 2 (not prime).\n");
        return 0;  // 2보다 작은 수는 소수가 아님
    }

    bigint *n_minus_1 = NULL, *t = NULL, *r = NULL;
    bi_set_one(&t);
    sub(&n, &t, &n_minus_1);  // n_minus_1 = n - 1
    int s = 0;
    //int res;
    //res = is_even(n_minus_1);
    //printf("is_even : %d\n", res);
    // n-1을 2^s * t 형태로 분해
    while (is_even(n_minus_1)) {
        bigint *tmp = NULL;
        bi_long_div(&n_minus_1, &two, &tmp, &r);  // n_minus_1 / 2
        bi_assign(&n_minus_1, tmp);
        bi_delete(&tmp);
        s++;  // s를 증가시킴
    }

    // `s` 값이 0일 경우를 처리합니다.
    if (s == 0) {
        bi_assign(&t, n_minus_1);
        //printf("n-1 is odd, s = 0\n");
    } 

    // Miller-Rabin 테스트 반복
    for (int i = 0; i < k; i++) {
        bigint *b = NULL;
        bi_set_random_in_range(&b, "2", n);
        //bi_show_hex(b);
        if (!miller_rabin_test(n, b, s, n_minus_1)) {
            //printf("Failed Miller-Rabin test. Composite detected.\n");
            bi_delete(&b);
            bi_delete(&n_minus_1);
            bi_delete(&t);
            bi_delete(&r);

            return 0;  // 합성수로 판별됨
        }
        bi_delete(&b);

    }

    bi_delete(&n_minus_1);
    bi_delete(&t);
    bi_delete(&r);
    printf("\nPassed all Miller-Rabin tests. Likely prime.\n");
    return 1;  // 소수일 가능성이 있음
}

void generate_large_prime(bigint **prime, int bit_length, int k) {
    bigint *candidate = NULL;

    //srand((unsigned int)time(NULL));
    do {
        bi_gen_rand(&candidate, NON_NEGATIVE, (bit_length + WORD_BIT - 1) / WORD_BIT);
        candidate->a[0] |= 1;

        printf(".");
        //bi_show_hex(candidate);

        if (is_prime(candidate, k)) {
            printf("Found prime!\n");
            bi_show_hex(candidate);
            break;
        }

        bi_delete(&candidate);
        candidate = NULL;
    } while (1);

    bi_assign(prime, candidate);
    bi_delete(&candidate);
}



/*
int main() {
    bigint *prime = NULL;

    // Generate a 256-bit prime number
    generate_large_prime(&prime, 256, 25);  // 256-bit prime, 25 iterations for Miller-Rabin

    // Print the prime number in hexadecimal format
    printf("Generated 256-bit Prime: ");
    bi_show_hex(prime);

    // Free memory
    bi_delete(&prime);

    return 0;
}
*/