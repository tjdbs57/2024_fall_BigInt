#include "test.h"


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
    char* test_str2 = "12345678abcdef5469210732abcdef64";  //5word 
    char* test_str1 = "1234567812345678123456781234567812345678";  //5word 

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

    div_long_core(&x, &y, &q, &r);

    printf("\nResult : ");
    bi_show_hex(q);
    bi_show_hex(r);

    bi_delete(&x);
    bi_delete(&y);
  //  bi_delete(&z);
}

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

void test_add() 
{  
    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *z = NULL;

        int wordlen1 = rand() % 96 + 1; 
        int wordlen2 = rand() % 96 + 1;  
        int sign_x = (rand() % 2 == 0);
        int sign_y = (rand() % 2 == 0);
        //bi_gen_rand(&x, sign_x, wordlen1);
        //bi_gen_rand(&y, sign_y, wordlen2);*/
        //int sign  = NON_NEGATIVE;

        bi_gen_rand(&x, sign_x, wordlen1);
        bi_gen_rand(&y, sign_y, wordlen2);
        
        sub(&x, &y, &z);
        print_bi_hex_py(x);                         
        printf(" - "); print_bi_hex_py(y);                    
        printf(" == "); print_bi_hex_py(z);                            
        printf("\n");  

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
    }
}

void test_sub_core() 
{  
    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *z = NULL;

        int wordlen1 = rand() % 96 + 1; 
        int wordlen2 = rand() % 96 + 1;  
        //int wordlen1= 32;
        //int wordlen2=32;
        int sign = rand() % 2;

        bi_gen_rand(&x, sign, wordlen1);
        bi_gen_rand(&y, sign, wordlen2);
        
        if (compare(x, y) == 0) {
            bigint *temp = x;
            x = y;
            y = temp;
        }

        sub_core(&x, &y, &z);
        print_bi_hex_py(x);                         
        printf(" - "); print_bi_hex_py(y);                    
        printf(" == "); print_bi_hex_py(z);                            
        printf("\n"); 
        
        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
    }

}

void test_mul() 
{  
    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        //bigint *z = NULL;
        bigint *q = NULL;
        bigint *r = NULL;
        //int wordlen = 5;
        int wordlen1 = rand() % 96 + 1; 
        int wordlen2 = rand() % 96 + 1;  
        //int sign = NON_NEGATIVE;
        int sign = rand() % 2;
        //int sign1 = rand() % 2;


        bi_gen_rand(&x, sign, wordlen1);
        bi_gen_rand(&y, NON_NEGATIVE, wordlen2);
        

        div_long_core(&x, &y, &q, &r);
        print_bi_hex_py(x);                         
        printf(" // "); print_bi_hex_py(y);                    
        printf(" == "); print_bi_hex_py(q);                            
        printf("\n");  

        printf(" %% "); 
        print_bi_hex_py(y);                    
        printf(" == "); 
        print_bi_hex_py(r);                            
        printf("\n");

        bi_delete(&x);
        bi_delete(&y);
        //bi_delete(&z);
    }
}

void test_addition() //test_add는 add_core에 대한 함수이고, test_addition은 음수에 대한 처리까지 포함한 add에 대한 테스트임
{  
    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *z = NULL;
        

        int wordlen1 = rand() % 96 + 1; 
        int wordlen2 = rand() % 96 + 1;
        //int wordlen1 = 32; 
        //int wordlen2 = 32;
    
        // sign을 랜덤으로 선택 (0이면 NON_NEGATIVE, 1이면 NEGATIVE)
        int sign_x = (rand() % 2 == 0) ? NON_NEGATIVE : NEGATIVE;
        int sign_y = (rand() % 2 == 0) ? NON_NEGATIVE : NEGATIVE;
        
        bi_gen_rand(&x, sign_x, wordlen1);
        bi_gen_rand(&y, sign_y, wordlen2);

        add(&x, &y, &z);
        print_bi_hex_py(x);                         
        printf(" + "); print_bi_hex_py(y);                    
        printf(" == "); print_bi_hex_py(z);                            
        printf("\n"); 
        
        
        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
    }
}


void test_left_shift_word() {

    bigint* x = NULL;

    bi_gen_rand(&x, NON_NEGATIVE, 2); 
    x->a[0]=0x87654321;
    x->a[1]=0x00000009;

    bi_show_hex(x);
    printf("=>\n");
    left_shift_word(&x,1);
    bi_show_hex(x);

    bi_delete(&x);
}

void test_bi_long_div()
{         
    for(int i = 0 ; i < TEST_CASE; i++)
    {    
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *q = NULL;
        bigint *r = NULL;
        
        int wordlen2 = rand() % 128 + 1;
        bi_gen_rand(&y, NON_NEGATIVE, wordlen2);
        
        bigint* temp=NULL;
        bigint* two=NULL;
        bi_new(&two, 1);
        two->a[0]=0x00000002;

        mul_core_improved(&y, &two, &temp);
        bi_assign(&x, temp); //x = 2y
        x->sign=NON_NEGATIVE;
   //x->sign = NEGATIVE;
        
        bi_long_div(&x,&y,&q,&r);
        print_bi_hex_py(x);                         
        printf(" // "); 
        print_bi_hex_py(y);                    
        printf(" == "); 
        print_bi_hex_py(q);                            
        printf("\n"); 

        print_bi_hex_py(x);                         
        printf(" %% "); 
        print_bi_hex_py(y);                    
        printf(" == "); 
        print_bi_hex_py(r);                            
        printf("\n"); 
       
        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&q);
        bi_delete(&r);
    }
}


#define TIME(start, end) ((double)((end) - (start))) / CLOCKS_PER_SEC * 1000.0

double measure_time(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z) 
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
    const int num = 10000;
    func(x, y, z);

    start = _rdtscp(&ui);
    for(int i = 0; i < num; i++)
    {
        func(x, y, z);
    }
    end = _rdtscp(&ui);
    printf("%llu\n", (end - start) / num);
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
        
        total_time_tx += measure_time(mul_core_tx, &x, &y, &z);
        total_time_improved += measure_time(mul_core_improved, &x, &y, &z);
        total_time_karatsuba += measure_time(mul_core_karatsuba, &x, &y, &z);

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
    }
    printf("Average time for mul_core_tx: %.2f ms\n", total_time_tx / TEST_CASE);
    printf("Average time for mul_core_improved: %.2f ms\n", total_time_improved / TEST_CASE);
    printf("Average time for mul_core_karatsuba: %.2f ms\n", total_time_karatsuba / TEST_CASE);
}


int measure_mem(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z)
{
    PROCESS_MEMORY_COUNTERS pmc;
    func(x, y, z);

    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
    {
        printf("Working Set Size : %llu KB\n", pmc.WorkingSetSize / 1024);
    }   
    return 0;
}