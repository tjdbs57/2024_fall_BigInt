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
        printf("%016llx", (x)->a[i]);
#else
        // For other word sizes (typically 32-bit), use %08x format specifier for printing
        printf("%08x", (x)->a[i]);
#endif
    }
}

/*
void test_set_by_array() {
    bigint* x = NULL;
    bigint* y = NULL;
    bigint* z = NULL;

    word arr[] = {0xed04eed1, 0x41c55066, 0x3a3181eb, 0x74b06684, 0x451512f1, 0x427430bd, 
    0x8533a1b8, 0xc2e6467a, 0x1a852c23, 0x2e5a9840}; // Example array
    word arr2[] = {0x8dfd1a18, 0x02b33a56, 0x4801e61}; 

    int sign = NON_NEGATIVE; // Set sign
    //int wordlen = sizeof(arr) / sizeof(arr[0]); // Calculate array length

    // Set bigint using bi_set_by_array
    bi_set_by_array(&x, sign, arr, 10);
    printf("input 1 :\n");
    bi_show_hex(x);
    
    bi_set_by_array(&y, sign, arr2, 3);
    printf("\ninput 2: \n");
    bi_show_hex(y);


    add_core(&x, &y, &z);

    printf("output :\n");
    bi_show_hex(z);
    // Free memory
    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&z);

}

void test_bi_string() {

    bigint* x = NULL; 
    bigint* y = NULL; 
    //bigint* z = NULL;
    bigint* q = NULL;
    bigint* r = NULL;

    char* test_str2 = "c645d212";  //5word 
    char* test_str1 = "367c449d1c22ee12";  //5word 

    int base = 16; 
    int sign = NON_NEGATIVE; 


    if (bi_set_by_string(&x, sign, test_str1, base) != 0) {
        printf("Failed to set bigint from string '%s'.\n", test_str1);
        return;
    }
    printf("First bigint : ");
    bi_show_hex(x);
    


    if (bi_set_by_string(&y, sign, test_str2, base) != 0) {
        printf("Failed to set bigint from string '%s'.\n", test_str2);
        bi_delete(&x);
        return;
    }
    printf("\nSecond bigint: ");
    bi_show_hex(y);

    //divc(x, x->wordlen, y, y->wordlen, &q, &r);
    //divc(&x, &y, &q, &r);
    div_long_core(&x, &y, &q, &r);
    printf("\nResult : ");
    bi_show_hex(q);
    bi_show_hex(r);

    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&q);
    bi_delete(&r);
  //  bi_delete(&z);

}

*/
#define MAX_BIT_LEN    256

void test()
{
    test_basic_operation(add, "+");
    //test_basic_operation(sub, "-");
    //test_basic_operation(mul_core_tx, "*");
    //test_basic_operation(mul_core_improved, "*");
    //test_basic_operation(mul_core_karatsuba, "*");
}

void test_basic_operation(void(*operation)(IN bigint** , IN bigint**, OUT bigint**), const char* operator)
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

        print_bi_hex_py(x);                         
        printf(" %s ", operator); print_bi_hex_py(y);                    
        printf(" == "); print_bi_hex_py(z);                            
        printf("\n");  

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
    }
}

void test_div()
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
        //div_long_core(&x, &y, &q, &r);

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

void test_exp_mod()
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

        L2R(&x, &y, &z, &mod);
        R2L(&x, &y, &z, &mod);
        exp_mod_montgomery(&x, &y, &z, &mod);


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

void test_barret(){

    for(int i=0; i<TEST_CASE; i++){

        int N_len = rand() % (MAX_BIT_LEN / WORD_BITLEN) + 1;
        int A_len = rand() % (2*N_len) + 1; // 1부터 2n까지 랜덤 생성

        // A 생성
        bigint* A=NULL;
        bi_gen_rand(&A,NON_NEGATIVE, A_len); //여기서 MEM_ALLOCATION

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

        bigint* R=NULL;
        barret_reduction(&A, &N, &T, &R); 
        
        print_bi_hex_py(A);
        printf(" %% ");
        print_bi_hex_py(N);
        printf(" == ");
        print_bi_hex_py(R);
        printf("\n");

        bi_delete(&A); 
        bi_delete(&N); 
        bi_delete(&T); 
        bi_delete(&W2n);
        bi_delete(&Wn);
        bi_delete(&R);
        bi_delete(&tmp);
    }
}

#define TIME(start, end) ((double)((end) - (start))) / CLOCKS_PER_SEC 

double measure_execution_time(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z) 
{
    srand((u32)time(NULL));

    clock_t start = clock();
    func(x, y, z);
    clock_t end = clock();
    
    return TIME(start, end);
}

void measure_cycles(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z) {
    u32 ui;
    u64 start, end;
    const int num = 100000;
    //func(x, y, z);

    start = _rdtscp(&ui);
    for(int i = 0; i < num; i++)
    {
        func(x, y, z);
    }
    end = _rdtscp(&ui);
    printf("%ld\n", (unsigned long)(end - start) / num);
}

void measure_clock_cycles()
{

    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *z = NULL;

        int sign1 = rand() % 2;
        int sign2 = rand() % 2;
        int wordlen = (MAX_BIT_LEN / WORD_BITLEN);
        bi_gen_rand(&x, sign1, wordlen);
        bi_gen_rand(&y, sign2, wordlen);
        
        //measure_cycles(mul_core_tx, &x, &y, &z);
        //measure_cycles(mul_core_improved, &x, &y, &z);
        //measure_cycles(mul_core_karatsuba, &x, &y, &z);
        measure_cycles(sub, &x, &y, &z);

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
    }
}

void measure()
{
    double total_time_tx = 0.0;
    double total_time_improved = 0.0;
    double total_time_karatsuba = 0.0;


    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *z = NULL;

        //int wordlen1 = rand() % 128 + 1; 
        //int wordlen2 = rand() % 128 + 1;  
        int sign = rand() % 2;
        int wordlen = 128;
        bi_gen_rand(&x, sign, wordlen);
        bi_gen_rand(&y, sign, wordlen);
        
        total_time_tx += measure_execution_time(mul_core_tx, &x, &y, &z);
        total_time_improved += measure_execution_time(mul_core_improved, &x, &y, &z);
        total_time_karatsuba += measure_execution_time(mul_core_karatsuba, &x, &y, &z);

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
    }
    printf("Average time for mul_core_tx: %.2f ms\n", total_time_tx / TEST_CASE);
    printf("Average time for mul_core_improved: %.2f ms\n", total_time_improved / TEST_CASE);
    printf("Average time for mul_core_karatsuba: %.2f ms\n", total_time_karatsuba / TEST_CASE);
}


size_t measure_memory_usage(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z)
{
    PROCESS_MEMORY_COUNTERS pmc;
    func(x, y, z);

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize;  // 메모리 사용량 반환
}