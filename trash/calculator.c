#include "bigint.h"
#include "arithmetic.h"
#include "utils.h"
#include "config.h"
//#include "prime.h"

// Diffie-Hellman 알고리즘을 시연하는 C 프로그램
int main()
{
    srand((u32)time(NULL));
    bigint *p = NULL, *g = NULL, *a = NULL, *b = NULL, *A = NULL, *B = NULL;
    bigint *keyA = NULL, *keyB = NULL;

    int bit_length = 256;
    printf("bit_length =  %d\n", bit_length);

    generate_large_prime(&p, bit_length, 5);
    printf("prime : \n");
    bi_show_hex(p);
    bi_gen_rand(&g, NON_NEGATIVE, 5); //base g
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