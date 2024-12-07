#include "bigint.h"
#include "arithmetic.h"
#include "utils.h"
#include "config.h"


// `a^m mod n`을 계산하는 함수 (bigint를 사용하여 구현)
//exp_mod_!!구현함

// n-2 이하의 임의의 a를 생성하는 함수
void bi_set_random_in_range(bigint **a, char *lower_bound, bigint *n) {
    bigint *tmp = NULL, *lower = NULL;
    int wordlen = n->wordlen;
    bi_set_by_string(&lower, NON_NEGATIVE, lower_bound, 16);
    // 범위 내의 랜덤 값을 생성 (2 이상 n-2 이하)
    do {
        bi_gen_rand(&tmp, NON_NEGATIVE, wordlen);
        //bi_show_hex(tmp);
    } while (compare(tmp, n) != 0 || compare(tmp, lower) != 1);  // n보다 작고 2 이상이어야 함

    bi_assign(a, tmp);  // 생성된 값을 a에 할당

    // tmp 메모리 해제
    bi_delete(&tmp);
    bi_delete(&lower);

}

int is_even(const bigint* x)
{
    if(x == NULL)   return 0;

    word last_word = x->a[x->wordlen - 1];
    return (last_word & 1) == 0;
}

int is_prime(bigint *n, int k)
{    
    if (n == NULL) {
        fprintf(stderr, "Error: NULL input in is_prime.\n");
        return 0; // NULL 포인터 처리
    }
    //bi_show_hex(n);
    int result = 1;
    bigint *one = NULL; bi_set_one(&one);
    bigint *two = NULL; bi_set_by_string(&two, NON_NEGATIVE, "2", 16);
    if(compare(n, one) != 1) { goto cleanup;}
    if(compare(n, two) == -1) { goto cleanup;}

    bigint *d = NULL, *s = NULL, *a = NULL, *x = NULL;
    bi_assign(&s, n);

    // n-1을 2^s * d 형태로 분해
    bigint *tmp_s = NULL;

    sub(&s, &one ,&tmp_s);  //n-1
    bi_assign(&s, tmp_s);
    //bi_show_hex(s);
    int s_count = 0;
    bigint *r = NULL;
    while(is_even(s))
    {   
        //printf("is even!\n");
        bi_long_div(&s, &two, &tmp_s, &r);
        bi_assign(&s, tmp_s);
        s_count++;
    }
    bi_assign(&d, s);
    bi_delete(&tmp_s);
    bi_delete(&r);

    bigint *tmp_x = NULL;
    for (int i = 0; i < k; i++)
    {
        //bi_show_hex(n);
        bi_set_random_in_range(&a, "2", n);
        exp_mod_montgomery(&a, &d, &x, &n);

        if(compare(x, one) == -1 || compare(x, n) == -1)  continue;

        int is_composite = 1;
        for (int r = 1; r < s_count; r++)
        {
            exp_mod_montgomery(&x, &x, &tmp_x, &n);
            bi_assign(&x, tmp_x);
            if(compare(x, n) == -1){
                is_composite = 0;
                break;
            }
        }

        if(is_composite)
        {
            result = 0;
            goto cleanup;    
        }
    }
    cleanup:
    // 모든 동적 메모리 해제
    bi_delete(&one);
    bi_delete(&two);
    bi_delete(&d);
    bi_delete(&s);
    bi_delete(&a);
    bi_delete(&x);
    bi_delete(&tmp_s);
    bi_delete(&tmp_x);
    bi_delete(&r);


    return (compare(n, one) <= 0 || compare(n, two) == 0) ? 0 : 1; // 최종 반환값
}


void test_is_prime() {
    // 작은 소수와 합성수 테스트
    bigint *prime = NULL;
    bi_set_by_string(&prime, NON_NEGATIVE, "11", 16);  // 17은 소수
    if (!is_prime(prime, 1)) {
        printf("Test failed! 17 is a prime number.\n");
        exit(1);
    }

    bi_set_by_string(&prime, NON_NEGATIVE, "12", 16);  // 18은 합성수
    if (is_prime(prime, 1)) {
        printf("Test failed! 18 is not a prime number.\n");
        exit(1);
    }

    printf("is_prime test passed.\n");
    bi_delete(&prime);
}



void generate_large_prime(bigint **p, int bit_length)
{
    bigint *candidate = NULL;

    do {
        int wordlen = (bit_length / 64) + 1;
        bi_gen_rand(&candidate, NON_NEGATIVE, wordlen);
        if (candidate == NULL) {
            fprintf(stderr, "Error: bi_gen_rand returned NULL.\n");
            continue;
        }

        //bi_show_hex(candidate);

        if(is_prime(candidate, 5))
        {
            //bi_show_hex(candidate);
            bi_assign(p, candidate);
            bi_delete(&candidate);
            return ;
        }

        bi_delete(&candidate);

    } while (1);

}
// Diffie-Hellman 알고리즘을 시연하는 C 프로그램
int DH_key()
{
    srand((u32)time(NULL));
    bigint *p = NULL, *g = NULL, *a = NULL, *b = NULL, *A = NULL, *B = NULL;
    bigint *keyA = NULL, *keyB = NULL;

    //const char* str1;
    //const char* str2;
    // Diffie-Hellman 파라미터 설정 (큰 숫자 사용)
    //int bit_length = rand() % 4096 + 1024;
    int bit_length = 128;
    printf("bit_length =  %d\n", bit_length);

    generate_large_prime(&p, bit_length);
    bi_show_hex(p);
    bi_gen_rand(&g, NON_NEGATIVE, 1); //base g
    //bi_show_hex(g);
    // Alice의 개인 키 선택
    bi_set_by_string(&a, NON_NEGATIVE, "10", 16);  // Alice's private key

    // Alice의 공개 키 계산 (A = g^a % p)
    exp_mod_montgomery(&g, &a, &A, &p);
    printf("Alice's public key A: ");
    bi_show_hex(A);  // 16진수 출력

    // Bob의 개인 키 선택
    bi_set_by_string(&b, NON_NEGATIVE, "f", 16);  // Bob's private key

    // Bob의 공개 키 계산 (B = g^b % p)
    exp_mod_montgomery(&g, &b, &B, &p);
    printf("Bob's public key B: ");
    bi_show_hex(B);  // 16진수 출력

    // Alice와 Bob은 서로 공개 키 A와 B를 교환

    // Alice와 Bob은 서로의 공개 키를 사용하여 동일한 비밀 키를 계산
    exp_mod_montgomery(&B, &a, &keyA, &p);
    exp_mod_montgomery(&A, &b, &keyB, &p);

    // 동일한 비밀 키가 계산되었음을 출력
    printf("Alice's computed secret key: ");
    bi_show_hex(keyA);  // 16진수 출력
    printf("\nBob's computed secret key: ");
    bi_show_hex(keyB);  // 16진수 출력

    // 메모리 해제
    bi_delete(&p);
    bi_delete(&g);
    bi_delete(&a);
    bi_delete(&b);
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&keyA);
    bi_delete(&keyB);

    return 0;
}


void test_bi_set_random_in_range() {
    bigint *random_num = NULL;
    bigint *upper = NULL;
    bigint *lower = NULL;

    bi_set_by_string(&upper, NON_NEGATIVE, "a", 16);  // 상한값
    bi_set_by_string(&lower, NON_NEGATIVE, "2", 16);      // 하한값

    bi_set_random_in_range(&random_num, "2", upper);
    printf("finish\n");
    bi_show_hex(random_num);
    printf("bi_set_random_in_range test passed.\n");
    bi_delete(&random_num);
    bi_delete(&upper);
    bi_delete(&lower);
}


// int main()
// {
//     srand((u32)time(NULL));

//     bigint *prime = NULL;
//     generate_large_prime(&prime, 16); // 작은 bit_length로 테스트
//     if (prime) {
//         printf("Generated prime: ");
//         bi_show_hex(prime);
//     } else {
//         printf("Failed to generate a prime.\n");
//     }
//     bi_delete(&prime);

//     //DH_key();
//     //test_bi_set_random_in_range();
//     // printf("Testing is_prime...\n");
//     // test_is_prime();    
//     return 0;
// }