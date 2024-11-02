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


void sub(bigint** x, bigint** y, bigint** z) {
    // Retrieve A and B from the pointers
    bigint* A = *x;
    bigint* B = *y;

    // Check if A is zero
    if (is_zero(A)) {
        // If A is zero, return -B (negate B)
        bi_new(z, B->wordlen);  // Create a new bigint for result
        (*z)->sign = (B->sign == NON_NEGATIVE) ? NEGATIVE : NON_NEGATIVE; // Negate the sign of B
        for (int i = 0; i < B->wordlen; i++) {
            (*z)->a[i] = B->a[i]; // Copy the words from B
        }
        return;
    }

    // Check if B is zero
    if (is_zero(B)) {
        // If B is zero, return A
        bi_assign(z, A);
        return;
    }

    // Check if A and B are equal
    if (A->wordlen == B->wordlen && memcmp(A->a, B->a, A->wordlen * sizeof(word)) == 0) {
        // If A == B, return 0
        bi_new(z, 1);  // Create a new bigint for zero
        (*z)->a[0] = ZERO; // Set result to zero
        return;
    }

    // If 0 < B =< A
    if ((B->sign == NON_NEGATIVE && !is_zero(B)) && (A->sign == NON_NEGATIVE && !is_zero(A))) { // B > 0 && A > 0
        // Use compare to check if B <= A
        int comparison_result = compareABS(B, A);
        if (comparison_result == -1 || comparison_result == 0) {  // B < A or B == A
            sub_core(&A, &B, z);  // Subtract B from A and store the result in result
            (*z)->sign=NON_NEGATIVE;
            return;
        }
    }
    // If 0 < A < B
    else if ((B->sign == NON_NEGATIVE && !is_zero(B)) && (A->sign == NON_NEGATIVE && !is_zero(A))) { // B > 0 && A > 0

        int comparison_result = compareABS(B, A);
        if (comparison_result == 1) {  // A < B
            sub_core(&B, &A, z);  // Subtract B from A and store the result in result
            (*z)->sign=NEGATIVE;
            return;
        }
    }
    // If B <= A < 0
    if ((B->sign == NEGATIVE && !is_zero(B)) && (A->sign == NEGATIVE && !is_zero(A))) { // B > 0 && A > 0
        
        int comparison_result = compareABS(B, A);
        if (comparison_result == 1 || comparison_result == 0) {  // |A| < |B| or |B| == |A|
            sub_core(&B, &A, z);  // Subtract B from A and store the result in result
            (*z)->sign=NON_NEGATIVE;
            return;
        }
    }
    //If A < B < 0
    else if ((B->sign == NEGATIVE && !is_zero(B)) && (A->sign == NEGATIVE && !is_zero(A))) { // B > 0 && A > 0
        
        int comparison_result = compareABS(B, A);
        if (comparison_result == -1) {  // |B| < |A|
            sub_core(&A, &B, z);  // Subtract B from A and store the result in result
            (*z)->sign=NEGATIVE;
            return;
        }
    }
    
    //If A > 0 and B < 0
    if ((B->sign == NEGATIVE && !is_zero(B)) && (A->sign == NON_NEGATIVE && !is_zero(A))) { // B > 0 && A > 0
        add_core(&A,&B,z); //add_core가 맞을지 add가 맞을지
        (*z)->sign=NON_NEGATIVE;
        return;
    }
    
    // If A < 0 and B > 0
    else{
        add_core(&A,&B,z); //add_core가 맞을지 add가 맞을지
        (*z)->sign=NEGATIVE;
        return;
    }

}

/*
void add(bigint** x, bigint** y, bigint** z) {

    bigint* A = *x;
    bigint* B = *y;

    // Check for zero cases
    if (is_zero(A) == ZERO) {
        bi_assign(z,B); // x가 0이면 y 반환
        return;
    }
    if (is_zero(B) == ZERO) {
        bi_assign(z,A); // y가 0이면 x 반환
        return;
    }

    // A > 0 and B < 0
    if ((A->sign = NON_NEGATIVE) && (B->sign=NEGATIVE)) {
        B->sign = NON_NEGATIVE; // y를 양수로 변환
        sub(A, B, z); // x가 양수이고 y가 음수일 때 x - |y|
        (*z)->sign=NON_NEGATIVE;
        return;
    }
    else if ((A->sign =NEGATIVE) && (B->sign = NON_NEGATIVE)) {
        A->sign = NON_NEGATIVE;
        sub_core(B, A, z); // x가 음수이고 y가 양수일 때 y - |x|
        (*z)->sign=NEGATIVE;
        return;
    }

    if(A->sign=B->sign){
        if(A->wordlen >= B->wordlen){
            add_core(A,B,z);
        }
        else{
            add_core(B,A,z);

        }
        (*z)->sign = (*x)->sign; // 두 수가 같은 부호이므로 x의 부호를 사용        
    }
}

//z의 부호 처리!!

*/

