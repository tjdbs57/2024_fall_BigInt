#include "utils.h"

void bi_new(bigint** x, int wordlen)
{
    // 기존 객체가 있으면 삭제
    if (*x != NULL)
        bi_delete(x);

    // 매크로를 이용해 메모리 할당 체크
    *x = (bigint*)calloc(1, sizeof(bigint));
    CHECK_MEM_ALLOCATION(*x);

    // 초기화
    (*x)->sign = NON_NEGATIVE; // 0
    (*x)->wordlen = wordlen;

    // wordlen만큼 추가 메모리 할당 및 체크
    (*x)->a = (word*)calloc(wordlen, sizeof(word));
    CHECK_MEM_ALLOCATION((*x)->a);
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
    if (x->sign == 1) {
        printf("-");
    }

    // 비트 출력
    for (int i = x->wordlen - 1; i >= 0; i--) {
        printf("0x%08x ", x->a[i]);
    }
    printf("\n");

}

int bi_set_by_array(bigint** x, int sign, word* a, int wordlen) {
    // 입력 배열이 NULL인지 확인
    CHECK_SET_ARRAY(a, wordlen);

    // bigint 초기화
    bi_new(x, wordlen);
    
    // 부호 설정
    (*x)->sign = (sign == NEGATIVE) ? NEGATIVE : NON_NEGATIVE;  // 부호 설정 (양수 또는 음수)
    
    // 주어진 배열을 bigint의 배열에 복사
    for (int i = 0; i < wordlen; i++) {
        (*x)->a[i] = a[i];
    }

    return 0; // 성공적으로 완료
}

int bi_set_by_string(bigint** x, int sign, char* str, int base) {

    CHECK_SET_STRING(str, base);


    size_t len = strlen(str);
    size_t wordlen = (len + (sizeof(word) * 2 - 1)) / (sizeof(word) * 2);
 
    bi_new(x, wordlen);
    (*x)->sign = sign; 

  
    size_t current_word = 0;

    // 문자열을 8자씩 읽어 단어를 구성
    for (size_t i = 0; i < len; i += 8) {
        size_t remaining = len - i;
        size_t copy_length = remaining < 8 ? remaining : 8;

        // 현재 단어에 값 업데이트
        (*x)->a[current_word] = 0; // 현재 단어 초기화

        // 8자리 부분을 역순으로 처리하여 단어를 구성
        for (size_t j = 0; j < copy_length; j++) {
            char c = str[i + copy_length - 1 - j];
            int value;

            // 문자를 숫자 값으로 변환
            if (isdigit(c)) {
                value = c - '0'; // 0-9의 경우
            } else if (isxdigit(c)) {
                value = tolower(c) - 'a' + 10; // 16진수 A-F의 경우
            } else {
                printf("Invalid character in string: %c\n", c);
                return -1; // 잘못된 문자에 대한 오류 반환
            }

            // 현재 단어에 값 업데이트
            (*x)->a[current_word] |= (value << (j * 4));
        }

        current_word++;
    }

    return 0; // 성공적으로 수행됨
}

void bi_refine(bigint* x)
{
    if(x == NULL)
        return;

    int wl = x->wordlen;
    
    for(int i=0;i<wl;i++){
        if(x->a[i]!=0){
            break;
        }
        wl--;
    }
    int new_wordlen = wl;

    if (x->wordlen != new_wordlen){
        bigint* c = NULL;
        int sign = x->sign;

        bi_new(&c,new_wordlen);
        c->sign = sign;

        for(int i=x->wordlen-1;i>=x->wordlen-new_wordlen;i--){
            c->a[i-(x->wordlen-new_wordlen)]= x->a[i];
        }
        
        bi_delete(&x);
        bi_new(&x, new_wordlen);

        x->sign = sign;
        if (new_wordlen > 0) CHECK_MEM_ALLOCATION(x->a);
        
        bi_assign(&x, c);
        bi_delete(&c);        
    }
    if((x->wordlen == 1) && (x->a[0] == 0x0))
        x->sign = NON_NEGATIVE;
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

void bi_gen_rand(bigint**x, int sign, int wordlen)
{
    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);
}

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

void bi_set_one(bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x1;
}

void bi_set_zero(bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x0;
}

int compareABS(bigint* x, bigint* y){
    int n = x->wordlen;
    int m = y->wordlen;

    if (n>m){
        return 1;
    }
    else if(n<m){
        return -1;
    }
    else{
        for(int j=n-1; j>=0; j--){
            if(x->a[j]>y->a[j]){
                return 1;
            }
            else if(x->a[j]<y->a[j]){
                return -1;
            }
        }
    }
    return 0;
}

int compare(bigint* x, bigint* y)
{
    if(x->sign == NON_NEGATIVE && y->sign == NEGATIVE){
        return 1;
    }
    else if(x->sign == NEGATIVE && y->sign == NON_NEGATIVE){
        return -1;
    }
    else{
        int ret = compareABS(x, y);
        if(x->sign == NON_NEGATIVE){
            return ret;
        }
        else{
            return ret*(-1);
        }
    } 
}

int get_bit_length(bigint* num) {

    if (num->wordlen == 0) {
        return 0; // 단어가 없으면 0을 반환
    }

    // 각 단어의 비트 길이를 WORD_BITLEN으로 정의
    int total_bit_length = (num->wordlen - 1) * (8 * sizeof(word)); // 각 단어의 비트 길이 계산

    word last_word = num->a[num->wordlen - 1];

    // 마지막 단어의 선행 0 비트 계산
    while (last_word > 0) {
        last_word >>= 1; // 가장 낮은 비트를 제거
        total_bit_length++; // 비트 길이 증가
    }

    // 만약 bigint가 음수이면 부호 비트 추가
    if (num->sign == NEGATIVE) {
        total_bit_length++; // 부호 비트를 고려
    }

    return total_bit_length; // 총 비트 길이 반환
}

int get_word_length(bigint* x) {
    
    return x->wordlen; // Return the word length
}

int get_jth_bit(bigint* x, u32 j) {

    // Check if j is within valid range
    if (j >= ((u32)x->wordlen * sizeof(word))) {
        return -1;  // Invalid input
    }

    u32 word_index = j / sizeof(word); // Determine which word contains the bit
    u32 bit_index = j % sizeof(word);  // Determine the position of the bit in the word

    // Check if the calculated word index is out of bounds
    if (word_index >= (u32)x->wordlen) {
        return -1;  // Out of bounds
    }

    // Extract the bit value
    word mask = (1 << bit_index);
    return (x->a[word_index] & mask) ? 1 : 0;
}
int get_sign_bit(bigint* x) {
    return x->sign; // Return the sign field of the bigint
}


void flip_sign_bit(bigint* x) {
    // Toggle the sign
    x->sign = (x->sign == NEGATIVE) ? NON_NEGATIVE : NEGATIVE; // If sign is 0, set to 1; otherwise, set to 0
}

void right_shift(bigint* a, int shift) {

    int word_shift = shift / (8*sizeof(word));  // 얼마나 워드 단위로 이동하는지
    int bit_shift = shift % (8*sizeof(word));   // 워드 내에서의 비트 이동
    int old_wordlen = a->wordlen;
    
    // 오른쪽으로 시프트 한 후 남은 워드 수 계산
    int new_wordlen = old_wordlen - word_shift;
    if (new_wordlen < 1) {
        new_wordlen = 1; // 최소 1 워드는 남겨둡니다.
    }

    // 메모리 확장 (직접 할당)
    word* new_array = (word*)calloc(new_wordlen, sizeof(word));
    CHECK_MEM_ALLOCATION(new_array);

    // 각 워드를 오른쪽으로 시프트
    for (int i = 0; i < new_wordlen; i++) {
        new_array[i] = a->a[i + word_shift] >> bit_shift; // 오른쪽 시프트
        if (i + word_shift + 1 < old_wordlen && bit_shift > 0) {
            new_array[i] |= (a->a[i + word_shift + 1] << ((8*sizeof(word)) - bit_shift)); // 하위 비트
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

void left_shift(bigint* a, int shift) {

    int word_shift = shift / (8*sizeof(word));  // 얼마나 워드 단위로 이동하는지
    int bit_shift = shift % (8*sizeof(word));   // 워드 내에서의 비트 이동
    int old_wordlen = a->wordlen;
    int new_wordlen = old_wordlen + word_shift + 1;

    // 메모리 확장 (직접 할당)
    word* new_array = (word*)calloc(new_wordlen, sizeof(word));
    CHECK_MEM_ALLOCATION(new_array);

    // 각 워드를 왼쪽으로 시프트
    for (int i = 0; i < old_wordlen; i++) {
        new_array[i + word_shift] |= (a->a[i] << bit_shift); // 왼쪽 시프트
        if (i + word_shift + 1 < new_wordlen && bit_shift > 0) {
            new_array[i + word_shift + 1] |= (a->a[i] >> ((8*sizeof(word)) - bit_shift)); // 상위 비트
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


void reduction(bigint* A, int r, bigint* result) {

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
    CHECK_MEM_ALLOCATION(result);

    // Initialize result bigint
    result->sign = A->sign;

    // Set the word length of the result to word_shift (can also include a partial word)
    result->wordlen = word_shift + (bit_shift > 0 ? 1 : 0); 
    result->a = (word*)calloc(result->wordlen + 1, sizeof(word)); // Allocate space for result
    CHECK_MEM_ALLOCATION(result);

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

 