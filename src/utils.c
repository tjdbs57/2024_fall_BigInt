#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void bi_new(IN bigint** x, IN int wordlen)
{    
    if (*x != NULL)
        bi_delete(x);
    *x = (bigint*)calloc(1, sizeof(bigint));
    if (*x == NULL) 
    {
        MEM_ALLOCATION_FAIL;
        exit(1);
    }

    (*x)->sign = NON_NEGATIVE; // 0
    (*x)->wordlen = wordlen;

    (*x)->a = (word*)calloc(wordlen, sizeof(word));
    
    if ((*x)->a == NULL) 
    {
        MEM_ALLOCATION_FAIL;
        exit(1);
    }
}

void array_init(IN word* array, IN int length) 
{
    for (int i = 0; i < length; i++) {
        array[i] = ZERO;  
    }
}

void bi_delete(INOUT bigint** x)
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

void bi_show_hex(IN bigint* x) 
{
    if (x == NULL) {
        INVAILD_DATA;
        exit(1);
    }

    if (x->a == NULL || x->wordlen == 0) {
        INVAILD_DATA;
        exit(1);
    }

    if (x->sign == NEGATIVE) {
        printf("-");
    }
    
    for (int i = x->wordlen - 1; i >= 0; i--) {
#if WORD_BITLEN == 8
        // For 8-bit words, use %02x format specifier for printing
        printf("%x", (x)->a[i]);
#elif WORD_BITLEN == 64
        // For 64-bit words, use %016llx format specifier for printing
        printf("%I64x", (x)->a[i]);
#else
        // For other word sizes (typically 32-bit), use %08x format specifier for printing
        printf("%x", (x)->a[i]);
#endif
    }
    printf("\n");
}

int bi_set_by_array(OUT bigint** x, IN int sign, IN word* a, IN int wordlen) 
{
    if (a == NULL || wordlen <= 0) {
        SET_ARRAY_FAIL;
        exit(1);
    }
    bi_new(x, wordlen);
    
    (*x)->sign = (sign == NEGATIVE) ? NEGATIVE : NON_NEGATIVE; 
    
    for (int i = 0; i < wordlen; i++) {
        (*x)->a[i] = a[i];
    }

    return 0;
}


int bi_set_by_string(OUT bigint** x, IN int sign, IN char* str, IN int base) 
{
    if (str == NULL || base < 2 || base > 16) {
        SET_STRING_FAIL;
        exit(1);
    }

    size_t len = strlen(str);
    size_t wordlen = (len * 4 + (WORD_BITLEN - 1)) / WORD_BITLEN;
    
    bi_new(x, wordlen);
    (*x)->sign = sign; 

    for (size_t current_word = 0; current_word < wordlen; current_word++) {
        (*x)->a[current_word] = 0; 

        size_t bits_per_word = WORD_BITLEN / 4;
        for (size_t j = 0; j < bits_per_word; j++) {
            size_t index = len - 1 - (current_word * bits_per_word + j); 
            if (index >= len) break;
            char c = str[index];
            int value;

            if (isdigit(c)) {
                value = c - '0'; 
            } else if (isxdigit(c)) {
                value = tolower(c) - 'a' + 10;
            } else {
                INVAILD_DATA;
                exit(1);
            }

            (*x)->a[current_word] |= (value << (j * 4)); 
    
        }
    }

    return 0; 
}


void bi_refine(INOUT bigint* x)
{
    // NULL 체크
    if(x == NULL)
    {
        INVAILD_DATA;
        return;
    }

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
    
    if((x->wordlen == 1) && (x->a[0] == ZERO))
        x->sign = NON_NEGATIVE;
}

void bi_refine_word(IN bigint* x, IN int num_words) 
{
    if(x == NULL)
    {
        INVAILD_DATA;
        return;
    }

    int new_wordlen = x->wordlen - num_words;
    
    // Update the word length and reallocate memory if necessary
    if(x->wordlen != new_wordlen) 
    {
        x->wordlen = new_wordlen;
        x->a = (word*)realloc(x->a, sizeof(word)*new_wordlen);
    }

    // Reset the sign to false if the BINT represents zero
    if((x->wordlen == 1) && (x->a[0] == ZERO))
        x->sign = NON_NEGATIVE;
}

void bi_assign(OUT bigint** dest, IN bigint* src)
{
    if(*dest != NULL)
        bi_delete(dest);
    
    bi_new(dest, src->wordlen);

    (*dest)->sign = src->sign;
    (*dest)->wordlen = src->wordlen;

    //array_copy()
    for (int i = 0; i < src->wordlen; i++) {
        (*dest)->a[i] = src->a[i]; 
    }

}

void bi_gen_rand(OUT bigint** x, IN int sign, IN int wordlen)
{
    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);
}

void array_rand(OUT word* dst, IN int wordlen)
{
    u8* p = (u8*)dst;
    int cnt = wordlen * sizeof(word);
    while(cnt > 0)
    {
        *p = rand() & 0xff; 
        p++;
        cnt--;
    }
}

void bi_set_one(OUT bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = ONE;
}

void bi_set_zero(OUT bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = ZERO;
}

int compareABS(IN bigint* x, IN bigint* y)
{
    //x>y => return 1
    //x<y => return 0
    //x=y => return -1
    int n = x->wordlen;
    int m = y->wordlen;

    if (n>m){
        return 1;
    }
    else if(n<m){
        return 0;
    }
    else{
        for(int j=n-1; j>=0; j--){
            if(x->a[j] > y->a[j]){
                return 1;
            }
            else if(x->a[j]<y->a[j]){
                return 0;
            }
        }
    }
    return -1; //같을 땐 -1 반환
}

int compare(IN bigint* x, IN bigint* y)
{
    if(x->sign == NON_NEGATIVE && y->sign == NEGATIVE){
        return 1;
    }
    else if(x->sign == NEGATIVE && y->sign == NON_NEGATIVE){
        return 0;
    }
    else{
        int ret = compareABS(x, y);

        if(x->sign == NON_NEGATIVE){

            return ret;
        }
        else{
            return ret;
        }
    } 
}

int get_bit_length(IN bigint* x) {
    int bit_length = x->wordlen * WORD_BITLEN;

    // 가장 높은 비트부터 차례대로 검사
    for (int i = bit_length - 1; i >= 0; i--) {
        if (get_jth_bit(x, i) == 0)
            bit_length = bit_length - 1;
        else break;
    }
    return bit_length;
}

bool get_jth_bit(IN bigint* x, IN int j) 
{
    if (j >= WORD_BITLEN) {
        return (((x)->a[j / WORD_BITLEN] >> (j % WORD_BITLEN)) & ONE);  // 비트 시프트 후 AND 연산으로 1인지 0인지 확인
    }
    return ((x)->a[0] >> j) & ONE;  // 가장 첫 번째 워드에서 비트 확인
}

void right_shift_bit(INOUT bigint* x, IN int shift) 
{
    int word_shift = shift / (8*sizeof(word));  
    int bit_shift = shift % (8*sizeof(word));  
    int old_wordlen = x->wordlen;
    
    int new_wordlen = old_wordlen - word_shift;
    if (new_wordlen < 1) {
        new_wordlen = 1; 
    }

    word* new_array = (word*)calloc(new_wordlen, sizeof(word));
    if (new_array == NULL) 
    { 
        MEM_ALLOCATION_FAIL;
        exit(1);
    }

    for (int i = 0; i < new_wordlen; i++) {
        new_array[i] = x->a[i + word_shift] >> bit_shift; 
        if (i + word_shift + 1 < old_wordlen && bit_shift > 0) {
            new_array[i] |= (x->a[i + word_shift + 1] << ((8*sizeof(word)) - bit_shift)); 
        }
    }
    free(x->a);
    x->a = new_array;
    x->wordlen = new_wordlen;

    bi_refine(x);
}

void left_shift_bit(bigint* pptrBint, int shift_amount) {
    if (!(pptrBint) || !pptrBint) {
        fprintf(stderr, "Parameter is NULL in 'left_shift_bit'\n");
        return; // Invalid parameters or no shift needed.
    }
    if (shift_amount <= 0) {
        return; // No shift is needed for non-positive shift amounts
    }

    // Shift loop
    while (shift_amount > 0) {
        word carry = 0;  // Carry bit for the shift
        for (int i = 0; i < (pptrBint)->wordlen; ++i) {
            // Extract the bit that will be shifted out
            word next_carry = ((pptrBint)->a[i] >> (WORD_BITLEN - 1)) & ONE;
            // Perform the shift left operation
            (pptrBint)->a[i] = ((pptrBint)->a[i] << 1) | carry;
            carry = next_carry; // Update carry for the next iteration
        }
        if (carry) {
            // We need to increase the size of val to accommodate the new bit.
            word* new_val = (pptrBint)->a;
            new_val = realloc((pptrBint)->a, ((pptrBint)->wordlen + 1) * sizeof(word));
            if (new_val) {
                (pptrBint)->a = new_val;
                (pptrBint)->a[(pptrBint)->wordlen] = carry; // Add the carried bit in the new WORD
                (pptrBint)->wordlen++; // Increment word length
                // (*pptrBint)->val = new_val;
                // (*pptrBint)->val[(*pptrBint)->wordlen] = 0; // Initialize the new WORD to zero before setting the carry bit.
                // (*pptrBint)->val[(*pptrBint)->wordlen] |= carry; // Add the carried bit in the new WORD.
                // (*pptrBint)->wordlen++;
            } else {
                fprintf(stderr, "Memory allocation failure during left shift operation.\n");
                return; // Stop the function upon allocation failure.
            }
        }
        shift_amount--;
    }

}
void reduction(bigint** x, int r) {
    // If the desired bit length is greater than the current bit length, no reduction is needed
    if (r > get_bit_length(*x) ) return; // Trivial Case

    // Check if the power of 2 is a multiple of WORD_BITLEN and less than current bit length
    if (r % WORD_BITLEN == 0 && r < get_bit_length(*x)) {
#if WORD_BITLEN == 8
        // For 8-bit words, allocate memory for pwOf2/8 words
        word* tmp = (*x)->a;
        tmp = (word*)realloc(tmp, (r / WORD_BITLEN));
        (*x)->a = tmp;
#elif WORD_BITLEN == 64
        // For 64-bit words, allocate memory for 8 times (pwOf2/64) words
        word* tmp = (*x)->a;
        tmp = (word*)realloc(tmp, 8 * (r / WORD_BITLEN));
        (*x)->a = tmp;
#else
        // For other word sizes (typically 32-bit), allocate memory for 4 times (pwOf2/WORD_BITLEN) words
        word* tmp = (*x)->a;
        tmp = (word*)realloc(tmp, 4 * (r / WORD_BITLEN));
        (*x)->a = tmp;
#endif
        // Update the word length of the BINT structure
        (*x)->wordlen = r / WORD_BITLEN;
        return;
    }

    // Adjust the most significant word to fit the reduction
    (*x)->a[r / WORD_BITLEN] = (*x)->a[r / WORD_BITLEN] && (0xFF >> (r % WORD_BITLEN));

#if WORD_BITLEN == 8
    word* tmp = (*x)->a;
    tmp = (word*)realloc(tmp, (r / WORD_BITLEN));
    (*x)->a = tmp;
#elif WORD_BITLEN == 64
    word* tmp = (*x)->a;
    tmp = (word*)realloc(tmp, 8 * (r / WORD_BITLEN));
    (*x)->a = tmp;
#else
    word* tmp = (*x)->a;
    tmp = (word*)realloc(tmp, 4 * (r / WORD_BITLEN));
    (*x)->a = tmp;
#endif

    // Update the word length to reflect the new size
    (*x)->wordlen = (r / WORD_BITLEN) + 1;
    return;
}


void left_shift_word(INOUT bigint** x, IN int shift_words) 
{
    if (shift_words < 0) {
        fprintf(stderr, "Error: shift_amount is negative in 'left_shift_word'\n");
        return;
    }

    int new_wordlen =  (*x)->wordlen + shift_words;
    word *new_array = (*x)->a; 
    new_array = (word *) realloc((*x)->a, new_wordlen * sizeof(word));
    if (!new_array) {
        MEM_ALLOCATION_FAIL;
        exit(1);
    }
    (*x)->a = new_array;

    // Shift the existing words to the left by the shift amount
    for (int i = new_wordlen - 1; i >= shift_words; i--) {
        (*x)->a[i] = (*x)->a[i - shift_words];
    }

    // Initialize the newly created space with zeros
    for (int i = 0; i < shift_words; i++) {
        (*x)->a[i] = ZERO;
    }

    // Update the word length
    (*x)->wordlen = new_wordlen;
}


void right_shift_word(INOUT bigint** x, IN int shift_words) 
{   
    if (shift_words < 0) {
        fprintf(stderr, "Error: shift_amount is negative in 'right_shift_word'\n");
        return;
    }

    if (shift_words >= (*x)->wordlen)
        return;
    
    int new_wordlen = (*x)->wordlen - shift_words;

    for (int i = 0; i < new_wordlen; i++) 
    {
        (*x)->a[i] = (*x)->a[i + shift_words];
    }
    for (int i = new_wordlen; i < (*x)->wordlen; i++)
    {
        (*x)->a[i] = 0;
    }

        // Reallocate memory for the new word length
    word* new_val = (*x)->a;
    new_val = (word*)realloc((*x)->a, new_wordlen * sizeof(word));
    if (!new_val) {
        fprintf(stderr, "Error: Memory reallocation failed in 'right_shift_word'\n");
        exit(1);
    }
    (*x)->a = new_val;
    // Update the word length
    (*x)->wordlen = new_wordlen;
}

int is_zero(IN bigint* x) {

    word result = 0;

    for (int i = 0; i < x->wordlen; i++) {
        word temp = x->a[i]; // x->a[i]를 직접 수정하지 않도록 임시 변수 사용

        for (int j = 0; j < WORD_BITLEN; j++) {

            result |= (temp & 1); // temp의 가장 낮은 비트를 확인
            temp >>= 1;           // temp를 시프트하여 다음 비트를 확인

            if (result == 1) {
                return result; // 1인 비트가 있는 경우
            }
        }
    }
    return result;
}

void swap_bigint(INOUT bigint** x, INOUT bigint** y)
{
    bigint* tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;
}

void makeEven(INOUT bigint* x) 
{
    // Check if wordlen is odd
    if ((x)->wordlen % 2 == 1) {
        (x)->wordlen++;

        // Reallocate memory for val
        (x)->a = realloc((x)->a, (x)->wordlen * sizeof(word));
        if (!(x)->a) {
            MEM_ALLOCATION_FAIL;
            exit(1); 
        }

        // Fill the new WORD with 0
        (x)->a[(x)->wordlen - 1] = ZERO;
    }
}

void match_wordlen(INOUT bigint* x, INOUT bigint* y)
{
    int max_wordlen = MAXIMUM(x->wordlen, y->wordlen);

    // Resize x if its wordlen is smaller than max_wordlen
    if(x->wordlen < max_wordlen) {
        word *tmp = x->a;
        tmp = (word*)realloc(x->a, max_wordlen * sizeof(word));
        if (!tmp) {
            MEM_ALLOCATION_FAIL;
            exit(1);
        }
        x->a = tmp;

        // Initialize the newly allocated WORDs with 0
        for(int i = x->wordlen; i < max_wordlen; i++)
            x->a[i] = ZERO;

        x->wordlen = max_wordlen;
    }

    // Resize y if its wordlen is smaller than max_wordlen
    if(y->wordlen < max_wordlen) {
       word *tmp = y->a;
        tmp = (word*)realloc(y->a, max_wordlen * sizeof(word));
        if (!tmp) {
            MEM_ALLOCATION_FAIL;
            exit(1);
        }
        y->a = tmp;
    

        // Initialize the newly allocated WORDs with 0
        for(int i = y->wordlen; i < max_wordlen; i++)
            y->a[i] = ZERO;

        y->wordlen = max_wordlen;
    }
}

void bi_reset(INOUT bigint* x)
{
    for (int i = 0; i < x->wordlen; i++)
        x->a[i] = ZERO;
}

word get_word(IN bigint* x, IN int m_th) 
{
    // Check if the requested word index is out of bounds
    if (m_th < 0 || m_th >= x->wordlen) {
        // fprintf(stderr, "Error: Requested word index %d is out of bounds.\n", m_th);
        return 0;
    }

    // Return the m_th word from the val array
    return x->a[m_th];
}

void bi_extend(bigint** bi, int new_len) {
    if (new_len <= (*bi)->wordlen) return;

    // 새 크기의 워드 배열 할당
    word* new_array = (word*)calloc(new_len, sizeof(word));
    // 기존 데이터를 복사
    memcpy(new_array, (*bi)->a, (*bi)->wordlen * sizeof(word));
    // 기존 배열 해제
    free((*bi)->a);
    // 새 배열과 길이 설정
    (*bi)->a = new_array;
    (*bi)->wordlen = new_len;
}

void bi_set_bit(bigint** bi, int k) {
    // k번째 비트를 설정하기 위해 워드와 비트 위치를 계산
    int word_idx = k / WORD_BITLEN;       // 워드 배열 내에서의 인덱스
    int bit_idx = k % WORD_BITLEN;        // 워드 내에서의 비트 위치

    // 현재 워드 길이가 부족하면 확장
    if (word_idx >= (*bi)->wordlen) {
        bi_extend(bi, word_idx + 1);      // bi_extend는 BigInt의 길이를 확장
    }

    // k번째 비트를 설정
    (*bi)->a[word_idx] |= (1ULL << bit_idx);

    // 필요하면 wordlen 갱신
    bi_refine(*bi);
}