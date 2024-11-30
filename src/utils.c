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
        printf("%016llx", (x)->a[i]);
#else
        // For other word sizes (typically 32-bit), use %08x format specifier for printing
        printf("%x", (x)->a[i]);
#endif
    }
    //printf("\n");
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

int get_bit_length(IN bigint* x) 
{
    if (x->wordlen == 0){ 
        INVAILD_DATA;
        exit(1);
    }

    int total_bit_length = (x->wordlen - 1) * (8 * sizeof(word));

    word last_word = x->a[x->wordlen - 1];

    while (last_word > 0) {
        last_word >>= 1; 
        total_bit_length++; 
    }

    if (x->sign == NEGATIVE) {
        total_bit_length++; 
    }

    return total_bit_length;
}


bool get_jth_bit(IN bigint* x, IN int j) 
{
    if (j >= WORD_BITLEN) {
        return ((x)->a[j / WORD_BITLEN] >> (j % WORD_BITLEN)) & ONE;  // 비트 시프트 후 AND 연산으로 1인지 0인지 확인
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

void left_shift_bit(INOUT bigint* x, IN int shift) 
{
    int word_shift = shift / (8*sizeof(word)); 
    int bit_shift = shift % (8*sizeof(word));   
    int old_wordlen = x->wordlen;
    int new_wordlen = old_wordlen + word_shift + 1;

    word* new_array = (word*)calloc(new_wordlen, sizeof(word));
    if (new_array == NULL) 
    { 
        MEM_ALLOCATION_FAIL;
        exit(1);
    }

    for (int i = 0; i < old_wordlen; i++) {
        new_array[i + word_shift] |= (x->a[i] << bit_shift);
        if (i + word_shift + 1 < new_wordlen && bit_shift > 0) {
            new_array[i + word_shift + 1] |= (x->a[i] >> ((8*sizeof(word)) - bit_shift)); 
        }
    }

    free(x->a);
    x->a = new_array;
    x->wordlen = new_wordlen;

    bi_refine(x);
}


void reduction(IN bigint* x, IN int r, OUT bigint* result) 
{
    int word_size = sizeof(word) * 8; // Number of bits in a word
    int word_shift = r / word_size; // Number of full words to shift
    int bit_shift = r % word_size; // Remaining bits to shift

    // Handle the case when r is greater than or equal to the total bit length
    if (word_shift >= x->wordlen) {
        result->sign = x->sign;
        result->wordlen = 1; // Result is zero
        result->a = (word*)calloc(1, sizeof(word)); // Allocate one word
        if (result->a == NULL) // Check if memory allocation failed
        { 
            MEM_ALLOCATION_FAIL;
            exit(1);
        }        
        result->a[0] = ZERO; // Result is zero
        return;
    }

    // Initialize result bigint
    result->sign = x->sign;

    // Set the word length of the result to word_shift (can also include a partial word)
    result->wordlen = word_shift + (bit_shift > 0 ? 1 : 0); 
    result->a = (word*)calloc(result->wordlen + 1, sizeof(word)); // Allocate space for result
    if (result->a == NULL)
    { 
        MEM_ALLOCATION_FAIL;
        exit(1);
    }  
    // Copy the relevant parts of A into result
    for (int i = 0; i < word_shift; i++) {
        result->a[i] = x->a[i]; // Copy full words
    }

    // Handle remaining bits
    if (bit_shift > 0 && word_shift < x->wordlen) {
        result->a[word_shift] = x->a[word_shift] & ((1 << bit_shift) - 1); // Get lower bits
    }

    // Clean up result if needed
    while (result->wordlen > 1 && result->a[result->wordlen - 1] == ZERO) {
        result->wordlen--; // Reduce the word length if upper words are zero
    }
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


void right_shift_word(INOUT bigint* x, IN int shift_words) 
{
    if (x == NULL || x->a == NULL) 
    {
        MEM_ALLOCATION_FAIL;
        exit(1);
    }

    // shift_words만큼 오른쪽 워드를 없앰
    if (shift_words <= 0) 
    {
        return; // shift_words가 0 이하라면 아무 작업도 하지 않음
    }

    if (shift_words >= x->wordlen) 
    {
        // 모든 워드를 없애야 하는 경우
        free(x->a); 
        x->a = NULL;
        x->wordlen = 0;
        x->sign = 0; // 정수를 0으로 초기화
        return;
    }

    // shift_words만큼 워드를 오른쪽으로 이동
    int new_wordlen = x->wordlen - shift_words;
    for (int i = 0; i < new_wordlen; i++) 
    {
        x->a[i] = x->a[i + shift_words];
    }

    // 메모리 재할당으로 크기 조정
    x->a = (word*)realloc(x->a, new_wordlen * sizeof(word));
    if (x->a == NULL && new_wordlen > 0) 
    {
        MEM_ALLOCATION_FAIL;
        exit(1);
    }

    x->wordlen = new_wordlen;
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
