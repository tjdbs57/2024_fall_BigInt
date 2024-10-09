#include "config.h"
#include "utils.h"
// operator.c --> bigint.c
// ADD function
void swap_bigint(bigint** x, bigint** y) 
{
    bigint* tmp;

    tmp = *x; 
    *x = *y;         
    *y = tmp;       
}

void add_carry(word A, word B, word carry_in, word* carry_out, word* result)
{

    word sum = A + B;  
    *carry_out = 0x00000000;  

    // A와 B의 합에서 overflow가 발생했을 경우 carry_out을 1로 설정
    if (sum < A || sum < B) {
        *carry_out = 0x00000001;
    }

    // 이전의 carry_in 값을 sum에 더함 (이전 덧셈에서 발생한 carry 반영)
    sum += carry_in;

    *result = sum;

    // carry_in을 더했을 때 overflow가 발생하면 carry_out을 1 증가시킴
    if (sum < carry_in) {
        *carry_out += 0x00000001;
    }
}



void add_core(bigint** x, bigint** y, bigint** z)
{
   //y를 작은 값으로
    if(compareABS(*x, *y) == -1) {
        swap_bigint(x, y);
    }

    int n = (*x)->wordlen;
    int m = (*y)->wordlen;

    // carry를 저장하기 위해 n+1로 초기화
    bi_new(z, n+1);

    word res       = 0x00;
    word carry_out = 0x00;
    word carry_in  = 0x00;

    // 워드 길이가 작은것을 기준으로 덧셈 수행
    for (int i = 0; i < m; i++)
    {
        add_carry((*x)->a[i], (*y)->a[i], carry_in, &carry_out, &res);
        (*z)->a[i] = res;
        carry_in = carry_out;
    }
    // 남아있는 워드에 대해서 덧셈 수행
    for (int i = m; i < n; i++)
    {
        add_carry((*x)->a[i], (word)0, carry_in, &carry_out, &res);
        (*z)->a[i] = res;
        carry_in = carry_out;
    }

    //마지막 워드에서 carry 처리
    if(carry_in) {
        (*z)->a[n] = carry_in;
    }
    else {
        (*z)->wordlen = n;
    }
    bi_refine(*z);
}
