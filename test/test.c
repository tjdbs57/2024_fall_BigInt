#include "test.h"
//#include "error.h"
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
    bigint* z = NULL;
    char* test_str1 = "987654321"; 
    char* test_str2 = "123456789"; 

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

    sub_core(&x, &y, &z);
    // print_bi_hex_py(x);                         
    // printf(" - "); print_bi_hex_py(y);                    
    // printf(" == "); print_bi_hex_py(z);                            
    // printf("\n"); 
    printf("Result : ");
    bi_show_hex(z);

    bi_delete(&x);
    bi_delete(&y);
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
    if ((x)->sign == -1) { printf("-"); }  
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

        int wordlen1 = rand() % 313 + 1; 
        int wordlen2 = rand() % 313 + 1;  
        int sign = NON_NEGATIVE;

        bi_gen_rand(&x, sign, wordlen1);
        bi_gen_rand(&y, sign, wordlen2);
        
        add_core(&x, &y, &z);
        print_bi_hex_py(x);                         
        printf(" + "); print_bi_hex_py(y);                    
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
        int sign = NON_NEGATIVE;

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


void measure_time()
{
    clock_t start = clock();
    test_add();
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;

    FILE *log_file = fopen("log.txt", "a");
    if (log_file != NULL) {
        fprintf(log_file, "Creating test vector time: %f seconds\n", seconds);
        fclose(log_file);
    }
}