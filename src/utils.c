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
        printf("%x", x->a[i]);
    }

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


int bi_set_by_string(OUT bigint** x, IN int sign, IN char* str, IN int base) {
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
        return;
    
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
    if(x == NULL) return;

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
            if(x->a[j]>y->a[j]){
                return 1;
            }
            else if(x->a[j]<y->a[j]){
                return 0;
            }
        }
    }
    return 0;
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


int get_jth_bit(IN bigint* x, IN word j) 
{
    if (j >= ((word)x->wordlen * sizeof(word))) 
    { 
        INVAILD_DATA;
        exit(1);
    }

    word word_index = j / sizeof(word); 
    word bit_index = j % sizeof(word);  

    if (word_index >= (word)x->wordlen)
    { 
        INVAILD_DATA;
        exit(1);
    }

    word mask = (1 << bit_index);
    return (x->a[word_index] & mask) ? 1 : 0;
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

void right_shift_word(INOUT bigint* x, IN int shift_words) 
{   
    if (x == NULL || x->a == NULL) 
    {
        MEM_ALLOCATION_FAIL;
        exit(1);
    }
    
    int old_wordlen=x->wordlen;
    int new_wordlen = old_wordlen + shift_words*(WORD_BITLEN/4);
    
    bigint* new=NULL;
    bi_new(&new, new_wordlen);
    new->sign=x->sign;
    
    for(int i=0; i<old_wordlen; i++){
        new->a[i]=x->a[i];
    }
    for(int i=old_wordlen; i<new_wordlen; i++){
        new->a[i]=ZERO;
    }

    free(x->a);
    x->a = (word*)calloc(new_wordlen, sizeof(word));

    bi_assign(&x,new);
    bi_delete(&new);

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

void left_shift_word(INOUT bigint** x, IN int shift_words) 
{


    if (shift_words < 0) {
        fprintf(stderr, "Error: shift_amount is negative in 'left_shift_word'\n");
        return;
    }

    int new_wordlen =  (*x)->wordlen + shift_words;
    word *new_array = (word *) realloc((*x)->a, new_wordlen * sizeof(word));
    if (!new_array) {
        fprintf(stderr, "Error: Memory reallocation failed in 'left_shift_word'\n");
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

 