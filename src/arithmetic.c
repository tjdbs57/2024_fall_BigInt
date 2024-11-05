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


void mul_single_word(IN word A, IN word B, OUT bigint** result)
{
    const int half_word = WORD_BITLEN / 2;
    const word mask     = (ONE << half_word) - 1;

    word A0 = A & mask;
    word A1 = A >> half_word;
    
    word B0 = B & mask;
    word B1 = B >> half_word;

    word T0 = A0 * B1;
    word T1 = A1 * B0;

    T0 = T0 + T1;
    T1 = T0 < T1;

    word C0 = A0 * B0;
    word C1 = A1 * B1;

    word T = C0;
    C0 += (T0 << half_word);
    C1 += (T1 << half_word) + (T0 >> half_word) + (C0 < T);
    
    (*result)->a[0] = C0;
    (*result)->a[1] = C1;

}


void mul_core_tx(IN bigint** x, IN bigint** y, OUT bigint** z)
{
    int n = (*x)->wordlen;
    int m = (*y)->wordlen; 

    int mul_wordlen = m + n;

    bi_new(z, mul_wordlen); 

    bigint* tmp = NULL;
    bigint* wordmul = NULL;
    bi_new(&tmp, mul_wordlen);
    
    for (int j = 0; j < n; j++)
    {
        for(int i = 0; i < m; i++)
        {
            bi_new(&wordmul, mul_wordlen);
            mul_single_word((*x)->a[i], (*y)->a[j], &wordmul);
            left_shift_word(wordmul, i+j);
            add_core(z, &wordmul, &tmp);
            bi_assign(z, tmp);
        }
    }

    bi_delete(&wordmul);
    bi_delete(&tmp);
    if((*x)->sign != (*y)->sign)
        (*z)->sign = NON_NEGATIVE;
}
