#include "test.h"

void print_bi_hex_py(IN const bigint* x) 
{
    if (!(x)) 
    {
        INVAILD_DATA;
        exit(1);
    }
    if (!(x)->a) 
    {
        INVAILD_DATA;
        exit(1);
    }
    if ((x)->sign == NEGATIVE) { printf("-"); }  
    printf("0x");                           
    
    for (int i = (x)->wordlen - 1; i >= 0; i--) 
    {
#if WORD_BITLEN == 8
        // For 8-bit words, use %02x format specifier for printing
        printf("%02x", (x)->a[i]);
#elif WORD_BITLEN == 64
        // For 64-bit words, use %016llx format specifier for printing
        printf("%016lx", (x)->a[i]);
#else
        // For other word sizes (typically 32-bit), use %08x format specifier for printing
        printf("%08x", (x)->a[i]);
#endif
    }
}


void test_squ(void(*operation)(IN bigint** , OUT bigint**), const char* operate)
{

    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        //bigint *y = NULL;
        bigint *z = NULL;

        int wordlen1 = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1; 
        int sign1 = rand() % 2;

        bi_gen_rand(&x, sign1, wordlen1);

        operation(&x, &z);
        printf("%s: ", operate);  

        print_bi_hex_py(x);                         
        printf(" * "); print_bi_hex_py(x);                    
        printf(" == "); print_bi_hex_py(z);                            
        printf("\n");  

        bi_delete(&x);
        bi_delete(&z);
    }
}

void test_basic_operation(void(*operation)(IN bigint** , IN bigint**, OUT bigint**), const char* operator, const char* operate)
{

    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *z = NULL;

        int wordlen1 = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1; 
        int wordlen2 = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1;  
        int sign1 = rand() % 2;
        int sign2 = rand() % 2;

        bi_gen_rand(&x, sign1, wordlen1);
        bi_gen_rand(&y, sign2, wordlen2);

        operation(&x, &y, &z);
        printf("%s: ", operate); 
        print_bi_hex_py(x);                         
        printf(" %s ", operator); print_bi_hex_py(y);                    
        printf(" == "); print_bi_hex_py(z);                            
        printf("\n");  

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
    }
}
void test_div(const char* operate)
{
    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *q = NULL;
        bigint *r = NULL;

        int wordlen1 = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1; 
        int wordlen2 = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1;  
        int sign = NON_NEGATIVE;

        bi_gen_rand(&x, sign, wordlen1);
        bi_gen_rand(&y, sign, wordlen2);

        bi_long_div(&x, &y, &q, &r);

        printf("%s: ", operate);  

        print_bi_hex_py(q);              
        printf(" * ");  print_bi_hex_py(y);                    
        printf(" + ");  print_bi_hex_py(r);
        printf(" == "); print_bi_hex_py(x);                     
        printf("\n");  


        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&q);
        bi_delete(&r);
    }
}

void test_exp_mod(void(*operation)(IN bigint** , IN bigint**, OUT bigint**, IN bigint**),const char* operate)
{

    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *z = NULL;
        bigint *mod = NULL;


        int wordlen1 = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1; 
        int wordlen2 = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1;  
        int sign = NON_NEGATIVE;

        bi_gen_rand(&x, sign, wordlen1);
        bi_gen_rand(&y, sign, 1);
        bi_gen_rand(&mod, sign, wordlen2);


        operation(&x, &y, &z, &mod);

        printf("%s: ", operate);  

        printf("pow("); 
        print_bi_hex_py(x); 
        printf(", "); 
        print_bi_hex_py(y); 
        printf(", "); 
        print_bi_hex_py(mod); 
        printf(") == "); 
        print_bi_hex_py(z); 
        printf("\n"); 


        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&mod);
        bi_delete(&z);
    }
}
void test_barret(const char* operate)
{
    int N_len = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1;

    // W^(n-1) <= N < W^n인 N 생성
    bigint* Wn_1 = NULL; // W^(n-1)
    bi_new(&Wn_1, N_len);
    Wn_1->a[N_len-1]=ONE;
        
    bigint* N = NULL;
    bi_gen_rand(&N,NON_NEGATIVE, N_len);
        
    while(compare(Wn_1, N)==1){ // W^(n-1) > N 이면  N 다시 생성
        bi_gen_rand(&N,NON_NEGATIVE, N_len); 
    }

    // T 사전 계산 ( T = floor(W^2n / N) )
    bigint* T=NULL;
    bigint* tmp=NULL;

    // W = 2^(WORD_BITLEN)
    bigint* W2n=NULL; 
    bigint* Wn=NULL;
    bi_new(&Wn, N_len+1);
    Wn->a[N_len]=ONE;

    squaring(&Wn, &tmp); 
    bi_assign(&W2n, tmp);

    // T 계산
    bi_long_div(&W2n, &N, &T, &tmp); // T = floor( W^2n / N )

    for(int i=0; i<TEST_CASE; i++){

        int A_len = rand() % (2*N_len) + 1; // 1부터 2n까지 랜덤 생성

        // A 생성
        bigint* A=NULL;
        bi_gen_rand(&A,NON_NEGATIVE, A_len); //여기서 MEM_ALLOCATION

        bigint* R=NULL;
        barret_reduction(&A, &N, &T, &R); 
        
        printf("%s:  ", operate);
        print_bi_hex_py(A);
        printf(" %% ");
        print_bi_hex_py(N);
        printf(" == ");
        print_bi_hex_py(R);
        printf("\n");

        bi_delete(&A); 
        bi_delete(&R);
    }

    bi_delete(&Wn_1);
    bi_delete(&N); 
    bi_delete(&T); 
    bi_delete(&W2n);
    bi_delete(&Wn);
    bi_delete(&tmp);
}


void measure_cycles(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z) {
    u32 ui;
    u64 start, end;
    const int num = 100000;

    volatile u64 cycles;

    start = _rdtscp(&ui);
    for(int i = 0; i < num; i++)
    {
        func(x, y, z);
    }
    end = _rdtscp(&ui);

    cycles = end - start;
    printf("%ld\n", (unsigned long)(cycles) / num);
}

#define MEASURE_CLOCK_CYCLES(func, x, y, z) \
    measure_cycles(func, &x, &y, &z);

#define TEST_CLOCK_CYCLES() \
    for(int i = 0; i < TEST_CASE; i++) { \
        bigint *x = NULL; \
        bigint *y = NULL; \
        bigint *z = NULL; \
        \
        int sign1 = rand() % 2; \
        int sign2 = rand() % 2; \
        int wordlen = (MAX_BIT_LEN / WORD_BITLEN); \
        bi_gen_rand(&x, sign1, wordlen); \
        bi_gen_rand(&y, sign2, wordlen); \
        \
        MEASURE_CLOCK_CYCLES(mul_core_tx, x, y, z); \
        MEASURE_CLOCK_CYCLES(mul_core_improved, x, y, z); \
        MEASURE_CLOCK_CYCLES(mul_core_karatsuba, x, y, z); \
        /*MEASURE_CLOCK_CYCLES(add, x, y, z);*/ \
        \
        bi_delete(&x); \
        bi_delete(&y); \
        bi_delete(&z); \
    }


void test_bi_string() {
    bigint* x = NULL; 
    bigint* y = NULL; 
    bigint* z = NULL;
    char* test_str1 = "123456789abcdfed654213560102013fadccaabbccddeeff"; 
    char* test_str2 = "345198450213541acdeffedcbaaabbddeeffcc12540000064"; 

    int base = 16; 
    int sign = NEGATIVE; 


    if (bi_set_by_string(&x, sign, test_str1, base) != 0) {
        printf("Failed to set bigint from string '%s'.\n", test_str1);
        return;
    }
    printf("\n");
    printf("Frist bigint: ");
    bi_show_hex(x);

    if (bi_set_by_string(&y, sign, test_str2, base) != 0) {
        printf("Failed to set bigint from string '%s'.\n", test_str2);
        bi_delete(&x);
        return;
    }
    printf("\nSecond bigint: ");
    bi_show_hex(y);

    add(&x, &y, &z);

    printf("\n");

    print_bi_hex_py(x);                         
    printf(" + "); print_bi_hex_py(y);                    
    printf(" == "); print_bi_hex_py(z);                            
    printf("\n");  

    bi_delete(&x);
    bi_delete(&y);
}