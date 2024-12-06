#include "library.c"

// miller_rabin
bool miller_rabin(bigint** n, int k)
{
    // n <= 1은 소수가 아님
    if (compare(*n, 1) <= 0) return false;
    // 2와 3은 소수
    if (bigint_cmp(*n, bigint_from_int(2)) == 0 || bigint_cmp(*n, bigint_from_int(3)) == 0) return true;

    // n-1 = 2^s * d 형태로 분해
    bigint* d = bigint_copy(*n);
    bigint_sub(&d, bigint_from_int(1), &d);  // n-1
    int s = 0;
    while (bigint_is_even(&d)) {
        bigint_divide_by_two(&d, &d); // d /= 2
        s++;
    }

    for (int i = 0; i < k; i++) {
        // 2 <= a <= n-2 범위에서 a를 랜덤하게 선택
        bigint* a = bigint_random_less_than(*n);
        bigint* x = bigint_copy(a);
        exp_mod_montgomery(&a, &d, &x, *n);  // x = a^d % n

        if (bigint_cmp(x, bigint_from_int(1)) == 0 || bigint_cmp(x, bigint_sub(*n, bigint_from_int(1))) == 0) {
            continue;
        }

        // x = x^2 % n 연산을 s번 반복
        for (int r = 0; r < s; r++) {
            exp_mod_montgomery(&x, bigint_from_int(2), &x, *n);  // x = x^2 % n
            if (bigint_cmp(x, bigint_sub(*n, bigint_from_int(1))) == 0) {
                break;
            }
        }

        // 소수가 아니면 false 반환
        if (bigint_cmp(x, bigint_sub(*n, bigint_from_int(1))) != 0) {
            return false;
        }
    }

    return true; // 소수일 확률 높음
}
// big prime 
u64 generate_big_prime(u32 n_bits)
{
    u64 num;
    int max_value = (1 << (n_bits - 1));

    do{
        num = rand() % max_value + (1 << (n_bits - 1));
    }while(!miller_rabin(num,5));

    return  num;
}