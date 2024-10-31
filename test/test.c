#include "test.h"
void test_set_by_array() {
    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    word arr[] = {0x00000000}; // Example array
    word arr2[] = {0x00000001, 0x00000000}; 

    int sign = NON_NEGATIVE; // Set sign
    int wordlen = sizeof(arr) / sizeof(arr[0]); // Calculate array length

    // Set bigint using bi_set_by_array
    bi_set_by_array(&x, sign, arr, wordlen);
    printf("input 1 :\n");
    bi_show_hex(x);
    
    bi_set_by_array(&y, sign, arr2, wordlen);
    printf("\ninput 2: \n");
    bi_show_hex(y);


    add_core(&x, &y, &z);

    printf("output :\n");
    bi_show_hex(z);
    // Free memory
    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&z);

}

void test_bi_string() {
    bigint* x = NULL; // 첫 번째 bigint 구조체 포인터 초기화
    bigint* y = NULL; // 두 번째 bigint 구조체 포인터 초기화
    bigint* z = NULL;
    char* test_str1 = "2e5a9841a852c23c2e6467a8533a1b8427430bd451512f174b066843a3181eb41c55066ed04eed1"; // 첫 번째 테스트할 문자열
    char* test_str2 = "4801e6102b33a568dfd1a18"; // 두 번째 테스트할 문자열
    int base = 16; // 진수 (16진수)
    int sign = NON_NEGATIVE; // 부호 (양수)

    // 첫 번째 빅넘버 설정
    if (bi_set_by_string(&x, sign, test_str1, base) != 0) {
        printf("Failed to set bigint from string '%s'.\n", test_str1);
        return;
    }
    printf("First bigint : ");
    bi_show_hex(x);
    

    // 두 번째 빅넘버 설정
    if (bi_set_by_string(&y, sign, test_str2, base) != 0) {
        printf("Failed to set bigint from string '%s'.\n", test_str2);
        bi_delete(&x); // 첫 번째 빅넘버 메모리 해제
        return;
    }
    printf("\nSecond bigint: ");
    bi_show_hex(y);

    add_core(&x, &y, &z);

    printf("Result : ");
    bi_show_hex(z);

    // 메모리 해제
    bi_delete(&x);
    bi_delete(&y);
}

void test_add()
{  
    bigint *x = NULL;
    bigint *y = NULL;
    bigint *z = NULL;

    int wordlen1 = 10;
    int wordlen2 = 3;
    //int wordlen1 = rand() % 10 + 1;
    //int wordlen2 = rand() % 10 + 1;
    int sign = NON_NEGATIVE;

    bi_gen_rand(&x, sign, wordlen1);
     printf("First bigint: ");
    bi_show_hex(x);

    bi_gen_rand(&y, sign, wordlen2);
    printf("Second bigint: ");
    bi_show_hex(y);

    add_core(&x, &y, &z);
    printf("Result: ");
    bi_show_hex(z);

    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&z);
}