#include "bigint.h"
#include "arithmetic.h"
#include "utils.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define WORD_BIT     64
void bi_set_random_in_range(bigint **b, char *lower_bound, bigint *upper_bound) {
    bigint *lower = NULL, *tmp = NULL, *range = NULL, *rand_value = NULL;

    // lower_bound 값을 bigint로 설정 (기본적으로 "2" 사용)
    bi_set_by_string(&lower, NON_NEGATIVE, lower_bound, 16);

    // upper_bound - lower_bound 계산하여 범위를 구함
    sub(&upper_bound, &lower, &range); // range = upper_bound - lower_bound

    // 난수 초기화
    srand((unsigned int)time(NULL));

    // [0, range) 범위의 난수 생성
    do {
        int wordlen = range->wordlen;  // upper_bound와 동일한 워드 길이로 난수 생성
        bi_gen_rand(&rand_value, NON_NEGATIVE, wordlen);

        // 난수 값이 range 이상이면 다시 생성
        if (compare(rand_value, range) != -1) {
            bi_delete(&rand_value);
        } else {
            break;
        }
    } while (1);

    // rand_value를 lower_bound만큼 올려서 최종 범위로 설정
    add(&rand_value, &lower, &tmp);  // tmp = rand_value + lower

    // 결과를 b에 저장
    bi_assign(b, tmp);

    // 메모리 해제
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
    if (compare(x, one) == -1 || compare(x, n_minus_1) == -1) {
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

        if (compare(x, n_minus_1) == -1) {
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
    if (x == NULL) return 0;  // NULL 포인터 처리

    // BigInt의 가장 마지막 워드 확인 (x->a[x->wordlen - 1]은 마지막 워드)
    word last_word = x->a[x->wordlen - 1];

    // 마지막 워드의 최하위 비트가 0이면 짝수
    return (last_word & 1) == 0;
}

// 소수 테스트 (is_prime)
int is_prime(bigint *n, int k) {
    bigint *two = NULL;
    bi_set_by_string(&two, NON_NEGATIVE, "2", 16);

    if (compare(n, two) == 0) {
        return 0;  // 1과 0은 소수가 아님
    }
    if (compare(n, two) == -1) {
        return 1;  // 2는 소수
    }

    // n - 1 = 2^s * t로 분해
    bigint *n_minus_1 = NULL;
    bigint *t = NULL;
    bi_set_one(&t);  // t = 1
    sub(&n, &t, &n_minus_1);  // n_minus_1 = n - 1
    int s = 0;


    while (is_even(n_minus_1)) {
        bigint *tmp = NULL;
        
        bi_long_div(&n_minus_1, &two, &tmp, NULL);  // n_minus_1 /= 2
        bi_assign(&n_minus_1, tmp);
        s++;
    }

    // k번 테스트 수행
    for (int i = 0; i < k; i++) {
        bigint *b = NULL;
        bi_set_random_in_range(&b, "2", n);  // b를 [2, n-2]에서 랜덤 선택

        if (!miller_rabin_test(n, b, s, n_minus_1)) {
            bi_delete(&b);
            bi_delete(&n_minus_1);
            bi_delete(&t);
            return 0;  // 합성수
        }
        bi_delete(&b);
    }

    bi_delete(&n_minus_1);
    bi_delete(&t);
    return 1;  // 소수
}

// Function to generate a random large prime number
void generate_large_prime(bigint **prime, int bit_length, int k) {
    bigint *candidate = NULL;

    // Initialize random number generator
    srand((unsigned int)time(NULL));

    do {
        // Generate a random number with the specified bit length
        bi_gen_rand(&candidate, NON_NEGATIVE, (bit_length + WORD_BIT - 1) / WORD_BIT);  // Bit length to word conversion
        // Ensure the number is odd (necessary for primality testing)
        candidate->a[0] |= 1;

        // Perform Miller-Rabin primality test
        if (is_prime(candidate, k)) {
            break;  // Found a prime number
        }

        bi_delete(&candidate);  // Delete candidate if not prime
        candidate = NULL;

    } while (1);

    // Assign the generated prime to the output
    bi_assign(prime, candidate);

    // Free memory
    bi_delete(&candidate);
}

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
