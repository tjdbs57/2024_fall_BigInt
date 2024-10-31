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



void sub_borrow(IN word A, IN word B, IN word borrow_in, OUT word* borrow_out, OUT word* result) {
    // borrow_out을 0으로 초기화
    *borrow_out = ZERO;

    // A에서 borrow_in을 뺀 값을 temp_A에 저장
    word temp_A = A - borrow_in;

    // borrow_in이 A보다 큰 경우 빌리기가 필요하므로 borrow_out을 1로 설정
    if (temp_A > A) {
        *borrow_out = ONE; // 빌리기 필요
        temp_A = A; // 빌리기가 필요한 경우, temp_A는 A로 설정
    }

    // temp_A에서 B를 빼고 결과를 result에 저장
    *result = temp_A - B;

    // 결과가 temp_A보다 큰 경우 추가로 빌리기가 발생했음을 의미
    if (*result > temp_A) {
        *borrow_out += ONE; // 추가 빌리기 발생
    }
}

void sub_core(bigint** x, bigint** y, bigint** z) 
{
    
    int n = (*x)->wordlen; // 첫 번째 빅넘버의 워드 길이
    int m = (*y)->wordlen; // 두 번째 빅넘버의 워드 길이

    // 더 긴 워드 길이에 맞춰 z 초기화
    int max_len = MAXIMUM(n, m);
    bi_new(z, max_len + 1); // 캐리를 위해 1 추가

    word borrow_out = ZERO; 
    word res       = ZERO; 

    // 워드 길이에 맞추기 위해 짧은 쪽에 0 추가
    for (int i = 0; i < max_len; i++) {

        word x_word = (i < n) ? (*x)->a[i] : 0; 
        word y_word = (i < m) ? (*y)->a[i] : 0; 

        // 뺄셈 수행
        sub_borrow(x_word, y_word, borrow_out, &borrow_out, &res);
        (*z)->a[i] = res; // 결과 저장
        printf("a[%d] : %u\n", i,(*z)->a[i]);
    }
    int l = n - 1;
    while (l >= 0 && (*z)->a[l] == 0) {
        l--;
    }
    l+=1;

    (*z)->wordlen = l;

    bi_refine(*z);
}