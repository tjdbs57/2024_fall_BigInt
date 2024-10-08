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
    // 문자열이 NULL인지 확인
    CHECK_SET_ARRAY(str, base);

    // 문자열 길이 계산
    size_t len = strlen(str);
    // 최대 word 개수는 문자열 길이보다 작거나 같음
    u32 wordlen = (len + (sizeof(word) * 2 - 1)) / (sizeof(word) * 2);
    // 새로운 bigint 생성
    bi_new(x, wordlen);
    (*x)->sign = sign; // 부호 설정

    // 배열 a 할당
    word* a = (word*)calloc(wordlen, sizeof(word));
    CHECK_MEM_ALLOCATION(a);
    CHECK_SET_ARRAY(a, wordlen);

    // 문자열을 단어로 변환
    size_t current_word = 0;
    size_t digit_pos = 0;

    // 문자열을 역순으로 읽어 단어를 구성
    for (size_t i = len; i > 0; i--) {
        char c = str[i - 1];
        int value;

        // 문자를 숫자 값으로 변환
        if (isdigit(c)) {
            value = c - '0'; // 0-9의 경우
        } else if (isxdigit(c)) {
            value = tolower(c) - 'a' + 10; // 16진수 A-F의 경우
        } else {
            free(a); // 잘못된 문자가 있는 경우 메모리 해제
            printf("Invalid character in string: %c\n", c);
            return -1; // 잘못된 문자에 대한 오류 반환
        }

        // 현재 단어에 값 업데이트
            a[current_word] |= (value << (digit_pos * 4));

        // 자리수를 증가시키고 단어 오버플로우 체크
        if (++digit_pos >= sizeof(word) * 2) {
            current_word++;
            digit_pos = 0;

            // 현재 단어의 인덱스가 wordlen을 초과하지 않도록 검사
            if (current_word >= wordlen) {
                free(a);
                printf("Overflow: too many digits for allocated word length.\n");
                return -1; // 오버플로우 오류 반환
            }
        }
    }

    // bi_set_by_array 함수를 사용하여 bigint에 배열 저장
    if (bi_set_by_array(x, sign, a, wordlen) != 0) {
        free(a); // 오류 발생 시 메모리 해제
        return -1; // 배열 설정 실패
    }

    // 할당한 메모리 해제
    free(a);
    return 0; // 성공적으로 수행됨
}




void bi_refine(bigint* x)
{
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
    if(x->sign == 0 && y->sign ==1){
        return 1;
    }
    else if(x->sign == 1 && y->sign == 0){
        return -1;
    }
    else{
        int ret = compareABS(x, y);
        if(x->sign == 0){
            return ret;
        }
        else{
            return ret*(-1);
        }
    } 
}

int get_jth_bit(bigint* x, u32 j) {

    if (x == NULL || x->a == NULL || j >= ((u32)x->wordlen * 32)) {
        return -1;  // Invalid input
    }

    u32 word_index = j / 32;      // Determine which word contains the bit
    u32 bit_index = j % 32;       // Determine the position of the bit in the word

    if (word_index >= (u32)x->wordlen) {
        return -1;  // Out of bounds
    }

    // Extract the bit value
    word mask = (1 << bit_index);
    return (x->a[word_index] & mask) ? 1 : 0;
}

int get_sign_bit(bigint* x) {

    if (x == NULL) {
        fprintf(stderr, "Invalid bigint\n"); // Print error message for NULL input
        return -1; // Return -1 in case of an error
    }
    return x->sign; // Return the sign field of the bigint
}

void flip_sign_bit(bigint* x) {

    if (x == NULL) {
        fprintf(stderr, "Invalid bigint\n"); // Print error message for NULL input
        return; // Exit the function if input is NULL
    }

    // Toggle the sign
    x->sign = (x->sign == 0) ? 1 : 0; // If sign is 0, set to 1; otherwise, set to 0
}

 