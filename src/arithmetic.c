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

/**
 * @brief Subtracts big integer y from big integer x and stores the result in z.
 * 
 * This function handles various cases depending on the signs and values of x and y, 
 * including when either operand is zero, when their absolute values are equal, 
 * and when their signs differ.
 * 
 * @param[in] x Pointer to a pointer of the first bigint structure (minuend).
 * @param[in] y Pointer to a pointer of the second bigint structure (subtrahend).
 * @param[out] z Pointer to a pointer of the result bigint structure.
 * 
 * - If x is zero, the function sets z to -y.
 * - If y is zero, the function sets z to x.
 * - If the absolute values of x and y are equal:
 *   - If x and y have the same sign, z is set to zero.
 *   - If x and y have opposite signs, their absolute values are added, and z has the sign of x.
 * - If x and y are both positive, the function subtracts the smaller magnitude from the larger one and assigns the appropriate sign to z.
 * - If x and y are both negative, it subtracts the smaller magnitude from the larger and assigns a negative sign if x has a larger magnitude, or a positive sign if y is larger.
 * - If x and y have opposite signs, their absolute values are added and z has the sign of x.
 */
void sub(IN bigint** x, IN bigint** y, OUT bigint** z) {
    
    // A가 0인 경우 결과는 -B
    if (is_zero(*x) == 0) {  // Dereference x and y
        bi_new(z, (*y)->wordlen);
        (*z)->sign = ((*y)->sign == NON_NEGATIVE) ? NEGATIVE : NON_NEGATIVE;
        for (int i = 0; i < (*y)->wordlen; i++) {
            (*z)->a[i] = (*y)->a[i]; 
        }

        bi_refine(*z);
        return;
    }

    // B가 0인 경우 결과는 A
    if (is_zero(*y) == 0) {  // Dereference x and y
        bi_assign(z, *x);
        bi_refine(*z);
        return;
    }

    // A와 B의 절댓값이 같은 경우
    if ((*x)->wordlen == (*y)->wordlen) {
        int equal = 1; // 두 bigint가 같은지 여부
        for (int i = 0; i < (*x)->wordlen; i++) {
            if ((*x)->a[i] != (*y)->a[i]) {
                equal = 0; // 다르면 equal을 0으로 설정
                break;
            }
        }

        if (equal) {
            //A와 B의 절댓값과 부호 모두 같은 경우
            if((*x)->sign == (*y)->sign){
                bi_set_zero(z);
                bi_refine(*z);
            }
            //A와 B의 절댓값이 같고 부호가 반대인 경우
            else{
                add_core(y, x, z);  // No need to dereference, pass as is
                (*z)->sign = (*x)->sign;
            }
            return;
        }
    }

    // 둘 다 양수인 경우
    if ((*x)->sign == NON_NEGATIVE && (*y)->sign == NON_NEGATIVE) {
        if (compareABS(*x, *y) > 0) {  // Dereference x and y
            sub_core(x, y, z);
            (*z)->sign = NON_NEGATIVE;
        } else {
            sub_core(y, x, z);
            (*z)->sign = NEGATIVE;
        }
        bi_refine(*z);
        return;
    }

    // 둘 다 음수인 경우
    if ((*x)->sign == NEGATIVE && (*y)->sign == NEGATIVE) {
        if (compareABS(*x, *y) > 0) {  // Dereference x and y
            sub_core(x, y, z);
            (*z)->sign = NEGATIVE;
        } else {
            sub_core(y, x, z);
            (*z)->sign = NON_NEGATIVE;
        }
        bi_refine(*z);
        return;
    }

    // A와 B의 부호가 반대인 경우
    if ((*x)->sign != (*y)->sign) {
        add_core(x, y, z);
        (*z)->sign = (*x)->sign;
        bi_refine(*z);
        return;
    }
}


void add(IN bigint **x, IN bigint **y, OUT bigint **z) {

    // A가 0인지 확인
    if(is_zero(*x)==0){
        bi_assign(z, *y); // A가 0이면 B 반환
        return;
    }

    if(is_zero(*y)==0){
        bi_assign(z, *x); // B가 0이면 A 반환
        return;
    }

    // Case: A가 양수이고 B가 음수인 경우
    if ((*x)->sign == NON_NEGATIVE && (*y)->sign == NEGATIVE) {
        (*y)->sign=NON_NEGATIVE; //절댓값으로 변환
        sub(x, y, z);
        (*y)->sign=NEGATIVE; //부호 되돌리기
        return;
    }

    // A가 음수이고 B가 양수인 경우
    if ((*x)->sign == NEGATIVE && (*y)->sign == NON_NEGATIVE) {
        
        (*x)->sign=NON_NEGATIVE; //절댓값으로 변환
        sub(y, x, z);
        (*x)->sign=NEGATIVE; //부호 되돌리기
        return;
    }

    // A와 B의 부호가 같은 경우
    if ((*x)->sign == (*y)->sign) {
    
        if((*x)->wordlen >= (*y)->wordlen){
            add_core(x,y,z);
        }
        else{
            add_core(y,x,z);
        }
        (*z)->sign = (*x)->sign;
    }

}