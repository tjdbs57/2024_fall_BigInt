#include "arithmetic.h"

void add_single_word(IN word A, IN word B, IN word carry_in, OUT word* carry_out, OUT word* result)
{
    word sum = A + B;  
    *carry_out = ZERO;  

    if (sum < A) {
        *carry_out = ONE;
    }

    sum += carry_in;

    if (sum < carry_in) {
        *carry_out += ONE;
    }

    *result = sum;
}

void add_core(IN bigint** x, IN bigint** y, OUT bigint** z)
{
    int n = (*x)->wordlen;
    int m = (*y)->wordlen; 

    int max_len = MAXIMUM(n, m);

    bi_new(z, max_len + 1); 

    word carry_out = ZERO; 
    word res       = ZERO; 

  
    for (int i = 0; i < max_len; i++) {

        word x_word = (i < n) ? (*x)->a[i] : 0; 
        word y_word = (i < m) ? (*y)->a[i] : 0; 

        // 덧셈 수행
        add_single_word(x_word, y_word, carry_out, &carry_out, &res);
        (*z)->a[i] = res;
    }

    if (carry_out != 0) {
        (*z)->a[max_len] = carry_out; 
        (*z)->wordlen = max_len + 1; 

    } else (*z)->wordlen = max_len;
    
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

void sub_borrow(IN word A, IN word B, IN word borrow_in, OUT word* borrow_out, OUT word* result) {
    *borrow_out = ZERO;
    word sub = A - B;
    
    if (A < borrow_in){
        *borrow_out = ONE;
    }
    if ( sub < B ){
        *borrow_out += 1;
    }
    
    result = sub;    

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
        //printf("a[%d] : %u\n", i,(*z)->a[i]);
    }
    int l = n - 1;
    while (l >= 0 && (*z)->a[l] == 0) {
        l--;
    }
    l+=1;

    (*z)->wordlen = l;

    bi_refine(*z);
}