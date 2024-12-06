#include "../include/arithmetic.h"  // libbigint.so의 헤더 파일 경로

int main()
{
    bigint** num1 = NULL;
    bigint** num2 = NULL;
    bigint** result = NULL;

    char* input1; 
    scanf("%x", &input1);
    
    bi_set_by_string(num1, sign, input1, 16);
}