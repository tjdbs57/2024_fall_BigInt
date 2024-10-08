#include "utils.h"

//2.2
void bi_new(bigint** x, u32 wordlen)
{
    if(*x != NULL)
        bi_delete(x);

    *x = (bigint*)malloc(sizeof(bigint));
    (*x)->sign = NON_NEGATIVE; // 0
    (*x)->wordlen = wordlen;
    (*x)->a = (word*)calloc(wordlen, sizeof(word));
}

void bi_delete(bigint** x)
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

/*
//2.6
void bi_assign(bigint** y, bigint* x)
{
    if(*y != NULL)
        bi_delete(y);

    bi_new(y, x->wordlen);
    (*y)->sign = x->sign;
    array_copy((*y)->a, x->a, x->wordlen);  //array_copy 
}

//2.7

void bi_gen_rand(bigint**x, int sign, int wordlen)
{
    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);                          //bi_refine 
}
*/
//byte--> u8
void array_rand(word* dst, int wordlen)
{
    u8* p = (u8*)dst;
    int cnt = wordlen * sizeof(word);
    while(cnt > 0)
    {
        *p = rand() & 0xff; // rand = DRBG
        p++;
        cnt--;
    }
 }

 void bi_assign(bigint** dest, bigint* src)
{
    if(*dest != NULL)
        bi_delete(dest);
    
    bi_new(dest, src->wordlen);

    (*dest)->sign = src->sign;
    (*dest)->wordlen = src->wordlen;

    //array_copy()
    for (int i = 0; i < src->wordlen; i++) {
        (*dest)->a[i] = src->a[i]; // 각 요소를 복사
    }

}
 

void bi_show_hex(bigint* x) {

    if (x == NULL) {
        printf("Invalid bigint\n");
        return;
    }

    // 추가 검사
    if (x->a == NULL || x->wordlen == 0) {
        printf("Invalid bigint data\n");
        return;
    }

    // 부호 출력
    if (x->sign == -1) {
        printf("-");
    }

    // 비트 출력
    for (int i = x->wordlen - 1; i >= 0; i--) {
        printf("0x%08x ", x->a[i]);
    }
    printf("\n");

}

int get_jth_bit(bigint* x, unsigned int j) {

    if (x == NULL || x->a == NULL || j >= ((unsigned int)x->wordlen * 32)) {
        return -1;  // Invalid input
    }

    unsigned int word_index = j / 32;      // Determine which word contains the bit
    unsigned int bit_index = j % 32;       // Determine the position of the bit in the word

    if (word_index >= (unsigned int)x->wordlen) {
        return -1;  // Out of bounds
    }

    // Extract the bit value
    word mask = (1 << bit_index);
    return (x->a[word_index] & mask) ? 1 : 0;
}

int get_sign_bit(bigint* x) {

    if (x == NULL) {
        fprintf(stderr, "Invalid bigint\n"); // Print error message for NULL input
        return 1; //error 
    }
    return x->sign; // Return the sign field of the bigint
}

void flip_sign_bit(bigint* x) {

    if (x == NULL) {
        fprintf(stderr, "Invalid bigint\n"); // Print error message for NULL input
        return; // Exit the function if input is NULL
    }

    // Toggle the sign
    x->sign = (x->sign == 0) ? -1 : 0; // If sign is 0, set to 1; otherwise, set to 0
}


int get_word_length(bigint* a) {
    
    if (a == NULL) {
        fprintf(stderr, "Invalid bigint\n");
        return -1; // 잘못된 입력에 대한 에러 코드 반환
    }
    return a->wordlen; // wordlen을 반환
}


int get_bit_length(bigint* a) {
    
    if (a == NULL) {
        fprintf(stderr, "Invalid bigint\n");
        return -1; // Return error code for invalid input
    }

    // If the bigint is zero, it requires 1 bit to represent
    //if (a->wordlen == 0 || (a->wordlen == 1 && a->a[0] == 0)) {
    //    return 1;
    //}

    // Calculate the bit length of the absolute value
    int bit_length = (a-> wordlen + 1) * 32; // Each word is 32 bits
    if (a->wordlen == 0 || (a->wordlen == 1 && a->a[0] == 0)) {
        bit_length = 1;
    }
    word last_word = a->a[a->wordlen - 1];

    // Count leading zeros in the last word
    while (last_word > 0) {
        last_word <<= 1; // Shift right to remove the least significant bit
        bit_length--; // Reduce the bit length count
    }

    // If the bigint is negative, add an extra bit for the sign
    if (a->sign == NEGATIVE) {
        bit_length++; // Account for the sign bit
    }

    return bit_length; // Return the total bit length
}

void left_shift(bigint* a, int shift) {
    if (a == NULL || shift <= 0) return;

    int word_shift = shift / 32;  // 얼마나 워드 단위로 이동하는지
    int bit_shift = shift % 32;   // 워드 내에서의 비트 이동
    int old_wordlen = a->wordlen;
    int new_wordlen = old_wordlen + word_shift + 1;

    // 메모리 확장 (직접 할당)
    word* new_array = (word*)calloc(new_wordlen, sizeof(word));
    if (new_array == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // 각 워드를 왼쪽으로 시프트
    for (int i = 0; i < old_wordlen; i++) {
        new_array[i + word_shift] |= (a->a[i] << bit_shift); // 왼쪽 시프트
        if (i + word_shift + 1 < new_wordlen && bit_shift > 0) {
            new_array[i + word_shift + 1] |= (a->a[i] >> (32 - bit_shift)); // 상위 비트
        }
    }

    // 기존 값 해제 및 새로운 배열로 교체
    free(a->a);
    a->a = new_array;
    a->wordlen = new_wordlen;

    // 불필요한 상위 워드가 0인 경우 제거
    while (a->wordlen > 1 && a->a[a->wordlen - 1] == 0) {
        a->wordlen--;
    }
}

void right_shift(bigint* a, int shift) {
    if (a == NULL || shift <= 0) return;

    int word_shift = shift / 32;  // 얼마나 워드 단위로 이동하는지
    int bit_shift = shift % 32;   // 워드 내에서의 비트 이동
    int old_wordlen = a->wordlen;
    
    // 오른쪽으로 시프트 한 후 남은 워드 수 계산
    int new_wordlen = old_wordlen - word_shift;
    if (new_wordlen < 1) {
        new_wordlen = 1; // 최소 1 워드는 남겨둡니다.
    }

    // 메모리 확장 (직접 할당)
    word* new_array = (word*)calloc(new_wordlen, sizeof(word));
    if (new_array == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // 각 워드를 오른쪽으로 시프트
    for (int i = 0; i < new_wordlen; i++) {
        new_array[i] = a->a[i + word_shift] >> bit_shift; // 오른쪽 시프트
        if (i + word_shift + 1 < old_wordlen && bit_shift > 0) {
            new_array[i] |= (a->a[i + word_shift + 1] << (32 - bit_shift)); // 하위 비트
        }
    }

    // 기존 값 해제 및 새로운 배열로 교체
    free(a->a);
    a->a = new_array;
    a->wordlen = new_wordlen;

    // 불필요한 상위 워드가 0인 경우 제거
    while (a->wordlen > 1 && a->a[a->wordlen - 1] == 0) {
        a->wordlen--;
    }
}


// Function to perform the modulus operation
void reduction(bigint* A, int r, bigint* result) {
    if (A == NULL || result == NULL || r <= 0) return;

    int word_size = sizeof(word) * 8; // Number of bits in a word
    int word_shift = r / word_size; // Number of full words to shift
    int bit_shift = r % word_size; // Remaining bits to shift

    // Handle the case when r is greater than or equal to the total bit length
    if (word_shift >= A->wordlen) {
        result->sign = A->sign;
        result->wordlen = 1; // Result is zero
        result->a = (word*)calloc(1, sizeof(word)); // Allocate one word
        result->a[0] = 0; // Result is zero
        return;
    }

    // Initialize result bigint
    result->sign = A->sign;

    // Set the word length of the result to word_shift (can also include a partial word)
    result->wordlen = word_shift + (bit_shift > 0 ? 1 : 0); 
    result->a = (word*)calloc(result->wordlen + 1, sizeof(word)); // Allocate space for result

    // Copy the relevant parts of A into result
    for (int i = 0; i < word_shift; i++) {
        result->a[i] = A->a[i]; // Copy full words
    }

    // Handle remaining bits
    if (bit_shift > 0 && word_shift < A->wordlen) {
        result->a[word_shift] = A->a[word_shift] & ((1 << bit_shift) - 1); // Get lower bits
    }

    // Clean up result if needed
    while (result->wordlen > 1 && result->a[result->wordlen - 1] == 0) {
        result->wordlen--; // Reduce the word length if upper words are zero
    }
}