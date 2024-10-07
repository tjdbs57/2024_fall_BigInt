#include "utils.h"

//2.2
void bi_new(bigint** x, u32 wordlen)
{
    if(*x != NULL)
        bi_delete(x);

    *x = (bigint*)malloc(sizeof(bigint));
    (*x)->sign = NON_NEGATIVE; // 0
    (*x)->wordlen = wordlen;
    (*x)->a = (word*)calloc(wordlen, sizeof(word));
}

void bi_delete(bigint** x)
{
    if(*x == NULL)
        return;
#ifdef ZERORIZE
    array_init((*x)->a, (*x)->wordlen);
#endif


    free((*x)->a);
    free(*x);
    *x = NULL;
}

/*
//2.6
void bi_assign(bigint** y, bigint* x)
{
    if(*y != NULL)
        bi_delete(y);

    bi_new(y, x->wordlen);
    (*y)->sign = x->sign;
    array_copy((*y)->a, x->a, x->wordlen);  //array_copy 
}

//2.7

void bi_gen_rand(bigint**x, int sign, int wordlen)
{
    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);                          //bi_refine 
}
*/
//byte--> u8
void array_rand(word* dst, int wordlen)
{
    u8* p = (u8*)dst;
    int cnt = wordlen * sizeof(word);
    while(cnt > 0)
    {
        *p = rand() & 0xff; // rand = DRBG
        p++;
        cnt--;
    }
 }
 

void bi_show_hex(bigint* x) {

    if (x == NULL) {
        printf("Invalid bigint\n");
        return;
    }

    // 추가 검사
    if (x->a == NULL || x->wordlen == 0) {
        printf("Invalid bigint data\n");
        return;
    }

    // 부호 출력
    if (x->sign == 1) {
        printf("-");
    }

    // 비트 출력
    for (int i = x->wordlen - 1; i >= 0; i--) {
        printf("0x%08x ", x->a[i]);
    }
    printf("\n");

}

int get_jth_bit(bigint* x, unsigned int j) {

    if (x == NULL || x->a == NULL || j >= (x->wordlen * 32)) {
        return -1;  // Invalid input
    }

    unsigned int word_index = j / 32;      // Determine which word contains the bit
    unsigned int bit_index = j % 32;       // Determine the position of the bit in the word

    if (word_index >= x->wordlen) {
        return -1;  // Out of bounds
    }

    // Extract the bit value
    word mask = (1 << bit_index);
    return (x->a[word_index] & mask) ? 1 : 0;
}

int get_sign_bit(bigint* x) {

    if (x == NULL) {
        fprintf(stderr, "Invalid bigint\n"); // Print error message for NULL input
        return -1; // Return -1 in case of an error
    }
    return x->sign; // Return the sign field of the bigint
}

void flip_sign_bit(bigint* x) {

    if (x == NULL) {
        fprintf(stderr, "Invalid bigint\n"); // Print error message for NULL input
        return; // Exit the function if input is NULL
    }

    // Toggle the sign
    x->sign = (x->sign == 0) ? 1 : 0; // If sign is 0, set to 1; otherwise, set to 0
}