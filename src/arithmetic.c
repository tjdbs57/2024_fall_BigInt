#include "arithmetic.h"

void or(bigint** x, bigint** y, bigint** z) 
{
    int min_len = MINIMUM((*x)->wordlen, (*y)->wordlen);
    int max_len = MAXIMUM((*x)->wordlen, (*y)->wordlen);

    // or
    for (int i = 0; i < min_len; i++)
        (*x)->a[i] = (*x)->a[i] | (*y)->a[i];
    
    // length
    for (int i = min_len; i < max_len; i++) 
    {
        if((*x)->wordlen > (*y)->wordlen)
            (*z)->a[i] = (*x)->a[i];
        else
            (*z)->a[i] = (*y)->a[i];
    }

    (*z)->wordlen = max_len; 

    if ((*x)->sign != (*y)->sign)
        (*z)->sign = NEGATIVE;

}

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
    bi_new(z, max_len); //안되는 중

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

void sub(IN bigint** x, IN bigint** y, OUT bigint** z) {
    bigint* A=NULL;
    bigint* B=NULL;
    bi_assign(&A, *x);
    bi_assign(&B, *y);
    
    // A가 0인 경우 결과는 -B
    if (is_zero(A) == 0) {  // Dereference x and y
        bi_new(z, B->wordlen);
        (*z)->sign = (B->sign == NON_NEGATIVE) ? NEGATIVE : NON_NEGATIVE;
        for (int i = 0; i < B->wordlen; i++) {
            (*z)->a[i] = B->a[i]; 
        }

        bi_refine(*z);
        return;
    }

    // B가 0인 경우 결과는 A
    if (is_zero(B) == 0) {  // Dereference x and y
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
            if(A->sign == B->sign){
                bi_set_zero(z);
                bi_refine(*z);
            }
            //A와 B의 절댓값이 같고 부호가 반대인 경우
            else{
                add_core(&B, &A, z);  // No need to dereference, pass as is
                (*z)->sign = A->sign;
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

    match_wordlen(*x, *y);
    bi_new(z, n+m); 

    bigint* tmp = NULL;
    bigint* wordmul = NULL;
    bi_new(&tmp, n+m);
    
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            bi_new(&wordmul, 2);
            mul_single_word((*x)->a[i], (*y)->a[j], &wordmul);
            left_shift_word(&wordmul, (i+j));
            add_core(z, &wordmul, &tmp);
            bi_assign(z, tmp);
        }
    }

    bi_delete(&wordmul);
    bi_delete(&tmp);
    if((*x)->sign != (*y)->sign)
        (*z)->sign = NEGATIVE;
}


void mul_core_improved(IN bigint** x, IN bigint** y, OUT bigint** z)
{
    match_wordlen(*x, *y);
    makeEven(*x);
    makeEven(*y);

    int n = (*x)->wordlen; //2n
    int m = (*x)->wordlen; //2m

    bi_new(z, n + m);

    int p = n / 2;

    bigint *T0 = NULL, *T1 = NULL, *T = NULL;
    bigint *tmp0 = NULL, *tmp1 = NULL, *tmp = NULL;

    bi_new(&T0, 2*p);
    bi_new(&T1, 2*p+1);
    bi_new(&T , n + m);

    bi_new(&tmp0, 2*p);
    bi_new(&tmp1, 2*p+1);
    bi_new(&tmp , (*z)->wordlen);

    for (int i = 0; i < 2 * p; i++)
    {
        bi_reset(T0); 
        bi_reset(T1);

        for(int k = 0; k < p ; k++) {
            bi_reset(tmp0);
            bi_reset(tmp1);

            mul_single_word((*x)->a[2*k], (*y)->a[i], &tmp0);
            mul_single_word((*x)->a[2*k+1], (*y)->a[i], &tmp1);

            left_shift_word(&tmp0, 2*k);
            bi_refine_word(tmp0, 2*k);
            or(&tmp0, &T0, &T0);

            bi_assign(&T0, tmp0);
       
            left_shift_word(&tmp1, 2*k);
            bi_refine_word(tmp1, 2*k);
            or(&tmp1, &T1, &T1);

            bi_assign(&T1, tmp1);

        }
        left_shift_word(&T1, 1);
        add_core(&T1, &T0, &T);

        left_shift_word(&T, i);
        bi_assign(&tmp, *z);
        add_core(&tmp, &T, z);
    }

    // Clean up 
    bi_delete(&T0);
    bi_delete(&T1);
    bi_delete(&T);    
    bi_delete(&tmp0);
    bi_delete(&tmp1);
    bi_delete(&tmp);
    bi_refine(*x);
    bi_refine(*y);

    if ((*x)->sign != (*y)->sign)
        (*z)->sign = NEGATIVE;
 
}


void mul_core_karatsuba(IN bigint** x, IN bigint** y, OUT bigint** z)
{
    int n =  (*x)->wordlen;
    int m =  (*y)->wordlen;
        
    if (FLAG >= MINIMUM(n, m)) {
        mul_core_improved(x, y, z);
        return;
    }

    bi_new(z, n + m);
    int l = ((MAXIMUM(n, m)) + 1) >> 1;

    bigint* A0 = NULL; bigint* A1 = NULL;
    bigint* B0 = NULL; bigint* B1 = NULL;
    bigint* T0 = NULL; bigint* T1 = NULL;
    bigint* shiftT1 = NULL;
    bigint* S  = NULL; bigint* S0 = NULL; bigint* S1 = NULL;
    bi_new(&S, 2 * l);

    bigint* C  = NULL; 
    bi_new(&C, (*z)->wordlen);
    bigint* tmpST1 = NULL; bigint* tmpST0 = NULL;

    match_wordlen(*x, *y);
    
    bi_assign(&A1, *x); right_shift_word(&A1, l);
    bi_assign(&A0, *x); reduction(&A0, l * WORD_BITLEN);


    bi_assign(&B1, *y); right_shift_word(&B1, l);     
    bi_assign(&B0, *y); reduction(&B0, l * WORD_BITLEN);
   
    // T1 = A1 * B1, T0 = A0 * B0
    mul_core_karatsuba(&A1, &B1, &T1);
    mul_core_karatsuba(&A0, &B0, &T0);


    bi_assign(&shiftT1, T1);
    left_shift_word(&shiftT1, 2 * l);
    add(&shiftT1, &T0, &C);

    // S1 = A0 - A1, S0 = B1 - B0
    sub(&A0, &A1, &S1);
    sub(&B1, &B0, &S0);

    bool sign_S = ((S0)->sign) ^ ((S1)->sign);
    S0->sign = NON_NEGATIVE;
    S1->sign = NON_NEGATIVE;

    // S = S1 * S0
    mul_core_karatsuba(&S1, &S0, &S);
    S->sign = sign_S;


    add(&S, &T1, &tmpST1);
    bi_assign(&S, tmpST1);

    bi_assign(&tmpST0, S);
    add(&tmpST0, &T0, &S);

    left_shift_word(&S, l);

    add(&C, &S, z);

    // clean up
    bi_delete(&A0); bi_delete(&A1);
    bi_delete(&B0); bi_delete(&B1);
    bi_delete(&T0); bi_delete(&T1);
    bi_delete(&S0); bi_delete(&S1);
    bi_delete(&shiftT1);
    bi_delete(&S);
    bi_delete(&C);
    bi_delete(&tmpST0);
    bi_delete(&tmpST1);
    bi_refine(*x); bi_refine(*y);
}

void squ_single_word(IN word A, OUT bigint** result)
{
    word w = WORD_BITLEN/2;
    word A1 = A >> w;
    word A0 = A & ((ONE << w) - 1);

    // C[0], C[1]
    bigint *C = NULL;
    bi_new(&C, 2);

    // T[0], T[1]
    bigint *T = NULL;
    bi_new(&T, 2);

    // cross multiplication
    C->a[0] = A0 * A0;
    C->a[1] = A1 * A1;

    T->a[0] = A0 * A1;
    left_shift_bit(T, w+1);

    bigint *tmp = NULL;
    bi_new(&tmp, 2);
    add(&C, &T, &tmp);

    bi_assign(result, tmp);
    
    bi_delete(&C);
    bi_delete(&T);
    bi_delete(&tmp);
}


void squ_tx(IN bigint** x, OUT bigint** result)
{
    int t = (*x)->wordlen;

    bigint *C1 = NULL;
    bi_new(&C1, 1);
    bigint *C2 = NULL;
    bi_new(&C2, 1);

    bigint *T1 = NULL;
    bi_new(&T1, 2);
    bigint *T2 = NULL;
    bi_new(&T2, 2);
    bigint *tmp1 = NULL;
    bi_new(&tmp1, 2);
    bigint *tmp2 = NULL;
    bi_new(&tmp2, 2);

    for(int j=0; j<= t-1; j++)
    {   
        squ_single_word((*x)->a[j], &T1);
        left_shift_word(&T1, 2*j);

        add(&C1, &T1, &tmp1);
        bi_assign(&C1, tmp1);

        for(int i=j+1; i<=t-1; i++)
        {
            mul_single_word((*x)->a[j], (*x)->a[i], &T2);
            left_shift_word(&T2, i+j);
           
            add(&C2, &T2, &tmp2);
            bi_assign(&C2, tmp2);

            bi_new(&T2, 2);
        }
    }
    left_shift_bit(C2, 1);
    add_core(&C1, &C2, result);

    bi_delete(&C1);
    bi_delete(&C2);
    bi_delete(&T1);
    bi_delete(&T2);
    bi_delete(&tmp1);
    bi_delete(&tmp2);

}


void squaring(IN bigint** x, OUT bigint** result) 
{
    if(is_zero(*x)==0){
        bi_assign(result, *x);
        (*result)->sign = NON_NEGATIVE;
    }
    else{
        squ_tx(x, result);
    }
}

void squ_karatsuba(IN bigint** x, OUT bigint** z)
{
    int n = (*x)->wordlen;

    static int lenZ = -1;
    if (lenZ == -1)
    {
        lenZ = 2 * n;
        bi_new(z, lenZ);
    }

    if (FLAG >= n)
    {
        squ_tx(x, z);
        return;
    }

    bi_new(z, 2*n);

    int l = (n+1) >> 1;

    bigint* A0 = NULL;    bigint* A1 = NULL;
    bigint* T0 = NULL;    bigint* T1 = NULL;
    bigint* shiftT1 = NULL;  
    bigint* R = NULL;
    bigint* S = NULL;
    bi_new(&S, 2*l);

    bi_assign(&A1, *x); right_shift_word(&A1, l);
    bi_assign(&A0, *x); reduction(&A0, l * WORD_BITLEN);

    squ_karatsuba(&A1, &T1);
    squ_karatsuba(&A0, &T0);

    bi_assign(&shiftT1, T1);
    left_shift_word(&shiftT1, 2*l);
    add(&shiftT1, &T0, &R);

    mul_core_karatsuba(&A1, &A0, &S);
    left_shift_word(&S, l);
    left_shift_bit(S, 1);

    add(&R, &S, z);

    bi_delete(&A0); bi_delete(&A1);
    bi_delete(&T0); bi_delete(&T1);
    bi_delete(&shiftT1);
    bi_delete(&S);
    bi_delete(&R);
}


void bi_long_div(IN bigint** x, IN bigint** y, OUT bigint** q, OUT bigint** r)
{
    
    bigint* tmp=NULL;
    bi_new(&tmp,1);

    if(is_zero(*y)==0){ // A / 0 = INVALID
        INVAILD_DATA;
        return;
    }

    if(is_zero(*x)==0){ // 0 / B = 0...0
        
        bi_set_zero(q);
        bi_set_zero(r);
        return;
    }
    
    if(compareABS(*x,*y)==0){ //A < B -> Q=0, R=A 
        bi_set_zero(q);
        bi_assign(r,*x);

        if((*x)->sign==NEGATIVE && (*y)->sign==NON_NEGATIVE){
            bigint* one=NULL;
            bi_set_one(&one);
        
            // Q <- -Q-1
            add(q,&one,&tmp);
            bi_assign(q,tmp);
            (*q)->sign=NEGATIVE;

            //R <- B-R
            sub_core(y,r,&tmp);
            bi_assign(r,tmp);
        }

        return;
    }

    if(compareABS(*x,*y)==-1){ // |A| == |B| =>Q=1, R=0
        bi_set_one(q);
        bi_set_zero(r);

        if((*x)->sign==NEGATIVE && (*y)->sign==NON_NEGATIVE){
            (*q)->sign=NEGATIVE;
        }

        return;
    }

    int q_len=(*x)->wordlen - (*y)->wordlen +1;
    int r_len=(*y)->wordlen;
    bi_new(q, q_len);
    bi_new(r,r_len);
    
    for(int i=(*x)->wordlen*WORD_BITLEN-1; i>=0; i--){

        //R<-2R+a_j
        left_shift_bit(*r,1); // 2R

        (*r)->a[0] ^= (((*x)->a[i / WORD_BITLEN] >> (i % WORD_BITLEN)) & 1); // +a_j

        //if R >= B
        int comp=compareABS(*r,*y);
        if (comp==1 || comp==-1) { //r >= B
            
            //Q <- Q+2^j = Q^(1<<j)
            int word_index = i / WORD_BITLEN;       
            int bit_index = i % WORD_BITLEN;        

            (*q)->a[word_index] ^= (ONE << bit_index);
     
            //r <- r-b
            sub_core(r,y,&tmp);
            bi_assign(r,tmp);
        }
    }

    if((*x)->sign==NEGATIVE){

        if(is_zero(*r)==0){

            //q <- -q
            (*q)->sign=NEGATIVE;
            return;
        }
        else{
            
            //R <- B-R
            sub_core(y,r,&tmp);
            bi_assign(r,tmp);
            
            bigint* one=NULL;
            bi_set_one(&one);
        
            // Q <- -Q-1
            add(q,&one,&tmp);
            bi_assign(q,tmp);
            (*q)->sign=NEGATIVE;
        }

    }
       
    bi_refine(*q);
    bi_refine(*r);

}

word quotient(word dividend1, word dividend0, word divisor) 
{
    word Q = 0;
    word R = dividend1;

    int w1 = WORD_BITLEN;
    for(int i = WORD_BITLEN-1; i>=0; i--) {
        if((divisor >> i) == 0)
            w1 -= 1;
        else
            break;
    }

    for(int j = w1; j>=0; j--) {
        if(R >= (word)(ONE << w1)) {
            Q += (ONE << j);
            R += (dividend0 >> j);
            R += (R-divisor);
        } else {
            R += (dividend0 >> j);
            R += R;
            if(R >= divisor) {
                Q += (ONE << j);
                R -= divisor;
            }
        }
    }
    return Q;
}

void div_long_core(IN bigint** x, IN bigint** y, IN bigint** Q, IN bigint** R)
{

    bi_new(Q, 1);  // Q 초기화

    bi_new(R, (*x)->wordlen);  // R 초기화

    int n = (*x)->wordlen;
    int m = (*y)->wordlen;

    word W = ONE << (WORD_BITLEN - 1);

    word x_m  = get_word(*x, m);
    word x_m1 = get_word(*x, m-1);
    word y_m1 = get_word(*y, m-1);


    // 몫 계산: n == m or n == m+1에 따라 몫 계산
    if (n == m) {
        (*Q)->a[0] = x_m1 / y_m1;
    }

    if (n == m+1) {
        if (x_m == y_m1) {
            (*Q)->a[0] = W - 1;  // 최댓값
        } else {
            (*Q)->a[0] = quotient(x_m, x_m1, y_m1);  // quotient 함수 사용
        }
    }

    // 나머지 계산: R = X - Y * Q
    bigint* YQ = NULL;
    bi_new(&YQ, (*y)->wordlen);
    mul_core_tx(y, Q, &YQ);  // Y * Q 계산

    sub(x, &YQ, R);  // X - YQ 계산

    // R이 음수일 때 보정
    bigint* one = NULL;
    bigint* tmpQ = NULL;
    bigint* tmpR = NULL;
    bigint* tmpY = NULL;
    bi_new(&one, (*Q)->wordlen);
    one->a[0] = ONE;

    // R이 음수일 때 Q와 R 수정
    while ((*R)->sign == NEGATIVE) {
        bi_assign(&tmpY, *y);

        sub(Q, &one, &tmpQ);  // Q에서 1 빼기
        bi_assign(Q, tmpQ);

        add(R, &tmpY, &tmpR);  // 나머지에 Y 더하기
        bi_assign(R, tmpR);

            // R이 이제 양수가 되었는지 확인
        if ((*R)->sign != NEGATIVE) {
            break;  // R이 더 이상 음수가 아니면 루프 종료
        }

        bi_refine(*Q);  // Q를 정리
        bi_refine(*R);  // R을 정리
    }

    // 메모리 해제
    bi_delete(&YQ);
    bi_delete(&one);
    bi_delete(&tmpQ);
    bi_delete(&tmpR);
    bi_delete(&tmpY);

}



void L2R(IN bigint** x, IN bigint** y, IN bigint** z, OUT bigint** M)
{
    int bit_len = get_bit_length(*y);
    bigint* t0 = NULL;
    bigint* temp = NULL;
    bigint* temp2 = NULL;
    bigint* Q1 = NULL;
    bi_new(&t0, 1);
    t0->a[0] = ONE;

    for (int i = bit_len-1; i >= 0; i--){
        bi_new(&temp,1);
        bi_new(&temp2,1);
        bi_new(&Q1,1);
        if (get_jth_bit(*y,i)){
            squaring(&t0,&temp);
            bi_long_div(&temp,M,&Q1,&temp2);
            mul_core_improved(&temp2,x,&temp);
            bi_long_div(&temp,M,&Q1,&t0);
        }
        else{
            squaring(&t0,&temp);
            bi_long_div(&temp, M, &Q1, &t0);
        }
    }
    bi_refine(t0);
    bi_assign(z,t0);

    bi_delete(&Q1);
    bi_delete(&t0);
    bi_delete(&temp);
    bi_delete(&temp2);
    bi_refine(*M);
}


void R2L(bigint** x, bigint** y, bigint** z, bigint** M) 
{
    int bit_len = get_bit_length(*y);

    bigint* t0 = NULL;
    bigint* t1 = NULL;
    bigint* temp = NULL;
    bigint* Q = NULL;
    bi_new(&t0, 1);
    t0->a[0] = ONE;
    bi_assign(&t1, *x);

    for (int i = 0; i < bit_len; i++) {
        bi_new(&temp, 1);
        if (get_jth_bit(*y, i)) {
            mul_core_improved(&t0, &t1, &temp);
            bi_long_div(&temp, M, &Q, &t0);
            squaring(&t1, &temp);
            bi_long_div(&temp, M, &Q, &t1);
        } else {
            squaring(&t1, &temp);
            bi_long_div(&temp, M, &Q, &t1);
        }
    }

    bi_long_div(&t0, M, &Q, &temp);
    bi_refine(temp);

    bi_assign(z, temp);
    bi_delete(&Q);
    bi_delete(&t0);
    bi_delete(&t1);
    bi_delete(&temp);
    bi_refine(*M);
}

void exp_mod_montgomery(bigint** x, bigint** y, bigint** z, bigint** M) 
{
    int bit_len = get_bit_length(*y);
    bigint* t0 = NULL; bigint* t1 = NULL;
    bigint* temp = NULL; bigint* temp2 = NULL;
    bigint* Q1 = NULL; bigint* Q2 = NULL;
   
    bi_new(&t0,1);
    t0->a[0] = ONE;
    bi_assign(&t1,*x);
    
    for (int i= bit_len-1 ; i >= 0 ;i--){
        bi_new(&temp,1);
        bi_new(&temp2,1);
        bi_new(&Q1,1);
        bi_new(&Q2,1);

        if (get_jth_bit(*y,i) == 0){
            mul_core_improved(&t0,&t1,&temp);
            bi_long_div(&temp,M,&Q1,&t1);
            squaring(&t0,&temp2);
            bi_long_div(&temp2,M,&Q2,&t0);
        }
        else{
            mul_core_improved(&t0,&t1,&temp);
            bi_long_div(&temp,M,&Q1,&t0);
            squaring(&t1,&temp2);
            bi_long_div(&temp2,M,&Q2,&t1);
        }

    }
    bi_assign(z,t0);
    bi_refine(*M);
    bi_refine(*z);
    bi_delete(&t0); bi_delete(&t1);
    bi_delete(&temp); bi_delete(&temp2);
    bi_delete(&Q1); bi_delete(&Q2);
}

void barret_reduction(IN bigint** x, IN bigint** y, IN bigint** z, OUT bigint** result)
{
    
    if(compare(*x, *y)==0){ // A < N
        bi_assign(result,*x);
        return;
    }

    else if(compare(*x, *y)==-1){ // A = N
        bi_set_zero(result);
        return;
    }

    bigint* Q=NULL;
    bigint* R=NULL;
    bigint* tmp=NULL;
    bigint* N=NULL;

    bi_assign(&Q, *x);

    int n= (*y)->wordlen;
    
    right_shift_word(&Q, (n-1)); // Q <- A >> w(n-1) 

    mul_core_improved(&Q, z, &tmp); // Q <- Q x T
    bi_assign(&Q, tmp);

    right_shift_word(&Q, (n+1)); // Q >> w(n+1)


    bi_assign(&N, *y);
    mul_core_improved(&N, &Q, &R); // R <- N x Q

    sub(x, &R, &tmp); // R <- A - R
    bi_assign(&R, tmp);
    bi_refine(R);


    while(compare(R, N)!=0){

        bi_assign(&N, *y);
        sub(&R, &N, &tmp);
        bi_assign(&R, tmp);
        bi_refine(R);
       
    }

    bi_assign(result, R);

    bi_delete(&N);
    bi_delete(&Q);
    bi_delete(&R);
    bi_delete(&tmp);
}
