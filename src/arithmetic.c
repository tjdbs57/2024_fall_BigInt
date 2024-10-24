#include "arithmetic.h"

// ADD function
void add_carry(word A, word B, word carry_in, word* carry_out, word* result)
{

    word sum = A + B;  
    *carry_out = ZERO;  

    // A와 B의 합에서 overflow가 발생했을 경우 carry_out을 1로 설정
    if (sum < A) {
        *carry_out = ONE;
    }

    // 이전의 carry_in 값을 sum에 더함 (이전 덧셈에서 발생한 carry 반영)
    sum += carry_in;



    // carry_in을 더했을 때 overflow가 발생하면 carry_out을 1 증가시킴
    if (sum < carry_in) {
        *carry_out += ONE;
    }

    *result = sum;

}

void add_core(bigint** x, bigint** y, bigint** z) {
    
    int n = (*x)->wordlen; // 첫 번째 빅넘버의 워드 길이
    int m = (*y)->wordlen; // 두 번째 빅넘버의 워드 길이

    // 더 긴 워드 길이에 맞춰 z 초기화
    int max_len = MAXIMUM(n, m);
    bi_new(z, max_len + 1); // 캐리를 위해 1 추가

    word carry_out = ZERO; 
    word res       = ZERO; 

    // 워드 길이에 맞추기 위해 짧은 쪽에 0 추가
    for (int i = 0; i < max_len; i++) {

        word x_word = (i < n) ? (*x)->a[i] : 0; 
        word y_word = (i < m) ? (*y)->a[i] : 0; 

        // 덧셈 수행
        add_carry(x_word, y_word, carry_out, &carry_out, &res);
        (*z)->a[i] = res; // 결과 저장
    }

    // 마지막 워드에서 캐리 처리
    if (carry_out != 0) {
        (*z)->a[max_len] = carry_out; // 마지막 워드에 캐리 저장
        (*z)->wordlen = max_len + 1; // 길이 증가
    } else {
        (*z)->wordlen = max_len; // 길이 유지
    }

    // 불필요한 워드가 없도록 bi_refine 호출
    bi_refine(*z);
}