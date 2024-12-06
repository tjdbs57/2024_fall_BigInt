
#include <stdio.h>
#include <openssl/bn.h>
#include "library.c"

// Diffie-Hellman 소수 생성 함수
void generate_prime(bigint *q, u32 N) {
    BN_CTX *ctx = NULL;
    BIGNUM *prime = NULL;

    // OpenSSL 초기화
    ctx = BN_CTX_new();
    prime = BN_new();

    if (ctx == NULL || prime == NULL) {
        printf("Error initializing OpenSSL BIGNUMs.\n");
        return;
    }

    // 소수 q를 생성 (N 비트 크기)
    if (BN_generate_prime_ex(prime, N, 1, NULL, NULL, ctx) == 0) {
        printf("Error generating prime number for q.\n");
        return;
    }

    // OpenSSL BIGNUM을 bigint로 변환하여 q에 저장
    bn_to_bigint(prime, q); // bn_to_bigint는 OpenSSL의 BIGNUM을 bigint로 변환하는 함수

    // 생성된 소수 출력
    printf("Generated prime q: ");
    bi_show_hex(q); // print_bigint는 bigint 값을 출력하는 함수

    // 자원 해제
    BN_free(prime);
    BN_CTX_free(ctx);
}

int main() {
    bigint *q = NULL; // Diffie-Hellman에서 사용하는 소수 q
    u32 N;

    // 키 비트 크기 입력
    printf("Enter key bit size: ");
    scanf("%u", &N);

    // q에 소수 생성
    q = create_bigint(); // create_bigint는 빈 bigint 객체를 생성하는 함수
    generate_prime(q, N); // 소수 생성

    // 추가적인 Diffie-Hellman 계산 및 사용자 키 생성 등을 여기서 처리

    // 자원 해제
    free_bigint(q); // free_bigint는 bigint 객체를 해제하는 함수

    return 0;
}

/*
#include "library.c"

typedef struct user
{
    bigint** private_key;
    bigint** public_key;
}User;

void generate_key(bigint q, bigint a, User *user, u32 bit_size);

int main() {
    bigint** q = NULL; bigint** a = NULL;
    User user1, user2;

    user1.private_key = NULL;
    user1.public_key = NULL;
    user2.private_key = NULL;
    user2.public_key = NULL;

    // Diffie-Hellman initiation
    u32 N;
    printf("Enter key bit size: ");
    scanf("%u", &N);

    // q와 a 값 생성
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());

    mpz_rrandomb(q, state, N);  // 임의의 큰 수 생성 (q)
    mpz_set_ui(a, 3);           // a = 3 (기본값)

    // 사용자 키 생성
    generate_key(q, a, &user1, N);
    generate_key(q, a, &user2, N);

    // 사용자1의 공개키와 사용자2의 공개키 출력
    gmp_printf("User1's public key: %Zd\n", user1.public_key);
    gmp_printf("User2's public key: %Zd\n", user2.public_key);

    // 공유 비밀키 생성
    mpz_t secretA, secretB;
    mpz_inits(secretA, secretB, NULL);

    compute_shared_key(q, user2.public_key, user1.private_key, secretA);
    compute_shared_key(q, user1.public_key, user2.private_key, secretB);

    gmp_printf("Shared key for User1: %Zd\n", secretA);
    gmp_printf("Shared key for User2: %Zd\n", secretB);

    // 두 공유 비밀키가 같은지 확인
    if (mpz_cmp(secretA, secretB) == 0) {
        printf("Shared keys match!\n");
    } else {
        printf("Shared keys do not match!\n");
    }

    // 키 복호화 시도 (단순히 예시로 crack_key 함수 사용)
    crack_key(q, a, user1.public_key, user2.public_key);

    // 메모리 해제
    mpz_clears(q, a, user1.private_key, user1.public_key, user2.private_key, user2.public_key, secretA, secretB, NULL);
    gmp_randclear(state);

    return 0;
}
// 키 생성 함수
void generate_key(mpz_t q, mpz_t a, User *user, unsigned int bit_size) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());

    // private_key 생성
    mpz_rrandomb(user->private_key, state, bit_size);

    // public_key = a^private_key mod q
    mpz_powm(user->public_key, a, user->private_key, q);
}

// 공유 비밀키 계산 함수
void compute_shared_key(mpz_t q, mpz_t public_key, mpz_t private_key, mpz_t shared_key) {
    mpz_powm(shared_key, public_key, private_key, q);
}

// 키를 깨는 함수 (단순한 Brute Force 예시)
void crack_key(mpz_t q, mpz_t a, mpz_t ya, mpz_t yb) {
    mpz_t counter, key1;
    mpz_inits(counter, key1, NULL);
    mpz_set_ui(counter, 0);

    // 브루트 포스 방식으로 y1을 찾음
    while (mpz_cmp(counter, q) < 0) {
        mpz_powm(key1, a, counter, q);
        if (mpz_cmp(key1, ya) == 0) {
            printf("Found the key: %Zd\n", counter);
            break;
        }
        mpz_add_ui(counter, counter, 1);
    }

    // 메모리 해제
    mpz_clears(counter, key1, NULL);
}
*/