#include "utils.h"

void bi_new(bigint** x, int wordlen)
{
    if(*x != NULL)
        bi_delete(x);

    *x = (bigint*)calloc(1, sizeof(bigint));
    if (*x == NULL)
    {
        printf("Memeory allocation failed for bigint structure.\n");
        exit(1);
    }

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

void bi_set_by_array(bigint** x, int sign, word* a, int wordlen) {
    // 입력 배열이 NULL인지 확인
    if (a == NULL || wordlen <= 0) 
    {
        printf("set array fail!\n");
        exit(1);
    }

    // bigint 초기화
    bi_new(x, wordlen);
    
    // 부호 설정
    (*x)->sign = (sign == NEGATIVE) ? NEGATIVE : NON_NEGATIVE;  // 부호 설정 (양수 또는 음수)
    
    // 주어진 배열을 bigint의 배열에 복사
    for (int i = 0; i < wordlen; i++) {
        (*x)->a[i] = a[i];
    }
    
}

void bi_refine(bigint* x)
{
    if(x == NULL)
        return;
    int new_wordlen = x->wordlen;
    while(new_wordlen > 1) // at least one word needed
    {
        if(x->a[new_wordlen- 1] != 0)
            break;
        new_wordlen--;
    }

    if (x->wordlen != new_wordlen)
    {
        x->wordlen = new_wordlen;
        x->a = (word*)realloc(x->a, sizeof(word)*new_wordlen);
    }

    if((x->wordlen == 1) && (x->a[0] == 0x0))
        x->sign = NON_NEGATIVE;
 }

void bi_assign(bigint** dest, bigint* src)
{
    if(*dest != NULL)
        bi_delete(dest);
    
    bi_new(dest, src->wordlen);

    (*dest)->sign = src->sign;
    (*dest)->wordlen = src->wordlen;

    //array_copy()
    for (int i = 0; i < src->wordlen; i++) {
        (*dest)->a[i] = src->a[i]; // 각 요소를 복사
    }

}

void bi_gen_rand(bigint**x, int sign, int wordlen)
{
    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);
}

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

void bi_set_one(bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x1;
}

void bi_set_zero(bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x0;
}

int compareABS(bigint* x, bigint* y){
    int n = x->wordlen;
    int m = y->wordlen;

    if (n>m){
        return 1;
    }
    else if(n<m){
        return -1;
    }
    else{
        for(int j=n-1; j>=0; j--){
            if(x->a[j]>y->a[j]){
                return 1;
            }
            else if(x->a[j]<y->a[j]){
                return -1;
            }
        }
    }
    return 0;
}

int compare(bigint* x, bigint* y)
{
    if(x->sign == 0 && y->sign ==1){
        return 1;
    }
    else if(x->sign == 1 && y->sign == 0){
        return -1;
    }
    else{
        int ret = compareABS(x, y);
        if(x->sign == 0){
            return ret;
        }
        else{
            return ret*(-1);
        }
    } 
}

int get_jth_bit(bigint* x, u32 j) {

    if (x == NULL || x->a == NULL || j >= ((u32)x->wordlen * 32)) {
        return -1;  // Invalid input
    }

    u32 word_index = j / 32;      // Determine which word contains the bit
    u32 bit_index = j % 32;       // Determine the position of the bit in the word

    if (word_index >= (u32)x->wordlen) {
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

 