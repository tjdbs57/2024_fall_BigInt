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

void sub_single_word(IN word A, IN word B, IN word borrow_in, OUT word* borrow_out, OUT word* result) 
{
    word sub = A - borrow_in;
    *borrow_out = ZERO;
    
    if (A < borrow_in) {
        *borrow_out = ONE;
    }

    if (sub < B) {
        *borrow_out += ONE;
    }
    
    *result = sub - B;  
}

void sub_core(IN bigint** x, IN bigint** y, OUT bigint** z) 
{
    
    int n = (*x)->wordlen; 
    int m = (*y)->wordlen; 

    int max_len = MAXIMUM(n, m);
    bi_new(z, max_len); 

    word borrow_out = ZERO; 
    word res        = ZERO; 

    for (int i = 0; i < max_len; i++) {

        word x_word = (i < n) ? (*x)->a[i] : 0; 
        word y_word = (i < m) ? (*y)->a[i] : 0; 

        sub_single_word(x_word, y_word, borrow_out, &borrow_out, &res);
        (*z)->a[i] = res; 

    }

    bi_refine(*x);
    bi_refine(*y);
    bi_refine(*z);

}

/*
void mul_single_word(IN word A, IN word B, OUT word* result)
{
    const int half_bit = WORD_BITLEN / 2;
    int mask           = ONE << half_bit;

    word A0 = A & mask;
    word A1 = A >> half_bit;
    
    word B0 = B & mask;
    word B1 = B >> half_bit;

    word T0 = A0 * B1;
    word T1 = A1 * B0;

    T0 = T0 + T1;
    T1 = T0 < T1;

    word C0 = B & mask;
    word C1 = B >> half_bit;
    

}
*/


void sub(IN bigint** x, IN bigint** y, OUT bigint** z) {
    bigint* A = *x;
    bigint* B = *y;

    // A가 0인지 확인 (모든 요소가 0인지 검사)
    int is_A_zero = 1; // A가 0인지 여부
    for (int i = 0; i < A->wordlen; i++) {
        if (A->a[i] != ZERO) {
            is_A_zero = 0; // A가 0이 아니면 0으로 설정
            break;
        }
    }

    // B가 0인지 확인 (모든 요소가 0인지 검사)
    int is_B_zero = 1; // B가 0인지 여부
    for (int i = 0; i < B->wordlen; i++) {
        if (B->a[i] != ZERO) {
            is_B_zero = 0; // B가 0이 아니면 0으로 설정
            break;
        }
    }

    // A가 0인 경우 결과는 -B
    if (is_A_zero) {
        bi_new(z, B->wordlen);
        (*z)->sign = (B->sign == NON_NEGATIVE) ? NEGATIVE : NON_NEGATIVE;
        memcpy((*z)->a, B->a, B->wordlen * sizeof(word));
        bi_refine(*z);
        return;
    }

    // B가 0인 경우 결과는 A
    if (is_B_zero) {
        bi_assign(z, A);
        bi_refine(*z);
        return;
    }

    // A와 B의 절댓값이 같은 경우
    if (A->wordlen == B->wordlen) {
        int equal = 1; // 두 bigint가 같은지 여부
        for (int i = 0; i < A->wordlen; i++) {
            if (A->a[i] != B->a[i]) {
                equal = 0; // 다르면 equal을 0으로 설정
                break;
            }
        }

        if (equal) {
            //A와 B의 절댓값과 부호 모두 같은 경우
            if(A->sign==B->sign){
                bi_set_zero(z);
                bi_refine(*z);
            }
            //A와 B의 절댓값이 같고 부호가 반대인 경우
            else{
                add_core(&B,&A,z);
                (*z)->sign=A->sign;
            }

            return;
        }
    }

    // 둘 다 양수인 경우
    if (A->sign == NON_NEGATIVE && B->sign == NON_NEGATIVE) {

        if (compareABS(A, B) > 0) {
            sub_core(&A, &B, z);
            (*z)->sign = NON_NEGATIVE;
        } else {
            sub_core(&B, &A, z);
            (*z)->sign = NEGATIVE;
        }
        bi_refine(*z);
        return;
    }

    // 둘 다 음수인 경우
    if (A->sign == NEGATIVE && B->sign == NEGATIVE) {
        if (compareABS(A, B) > 0) {
            sub_core(&A, &B, z);
            (*z)->sign = NEGATIVE;
        } else {
            sub_core(&B, &A, z);
            (*z)->sign = NON_NEGATIVE;
        }
        bi_refine(*z);
        return;
    }

    // A와 B의 부호가 반대인 경우
    if (A->sign != B->sign) {
        add_core(&A, &B, z);
        (*z)->sign = A->sign;
        bi_refine(*z);
        return;
    }
}



/*void add(IN bigint **x, IN bigint **y, OUT bigint **z) {

    bigint *A = *x;
    bigint *B = *y;

    // A가 0인지 확인
    if (A->wordlen == 1 && A->a[0] == ZERO) {
        bi_assign(z, B); // A가 0이면 B 반환
        return;
    }

    // B가 0인지 확인
    if (B->wordlen == 1 && B->a[0] == ZERO) {
        bi_assign(z, A); // B가 0이면 A 반환
        return;
    }

    // Case: A가 양수이고 B가 음수인 경우
    if (A->sign == NON_NEGATIVE && B->sign == NEGATIVE) {
        B->sign=NON_NEGATIVE;
        sub(&A, &B, z);

        return;
    }

    // A가 음수이고 B가 양수인 경우
    if (A->sign == NEGATIVE && B->sign == NON_NEGATIVE) {
        
        A->sign=NON_NEGATIVE;
        sub(&B, &A, z);
        return;
    }

    // A와 B의 부호가 같은 경우
    if (A->sign == B->sign) {
        if(A->wordlen >= B->wordlen){
            add_core(&A,&B,z);
        }
        else{
            add_core(&B,&A,z);
        }
        (*z)->sign = A->sign;
    }

}*/
