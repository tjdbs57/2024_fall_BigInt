#include "test.h"

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
*/
void test_bi_string() {
    bigint* x = NULL; 
    bigint* y = NULL; 
    bigint* z = NULL;
    char* test_str2 = "1111111111111"; 
    char* test_str1 = "12121212121213456785321"; 

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

    //bi_new(&z, x->wordlen + y->wordlen);

    mul_core_improved(&x, &y, &z);

    //add_core(&x, &y, &z);
//    print_bi_hex_py(x);                         
    // printf(" - "); print_bi_hex_py(y);                    
    // printf(" == "); print_bi_hex_py(z);                            
    // printf("\n"); 
    printf("\nResult : ");
    bi_show_hex(z);

    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&z);
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

        //int wordlen1 = rand() % 96 + 1; 
        //int wordlen2 = rand() % 96 + 1;  
        int wordlen1= 32;
        int wordlen2=32;
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
        bigint *z = NULL;

        int wordlen1 = rand() % 96 + 1; 
        int wordlen2 = rand() % 96 + 1;  
        int sign = NON_NEGATIVE;

        bi_gen_rand(&x, sign, wordlen1);
        bi_gen_rand(&y, sign, wordlen2);
        

        mul_core_improved(&x, &y, &z);
        print_bi_hex_py(x);                         
        printf(" * "); print_bi_hex_py(y);                    
        printf(" == "); print_bi_hex_py(z);                            
        printf("\n");  

        bi_delete(&x);
        bi_delete(&y);
        bi_delete(&z);
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

void test_right_shift_word() {

    bigint *x = NULL;

    printf("case 1:\n");
    bi_gen_rand(&x, NON_NEGATIVE, 5); // 5 워드 길이의 양수
    bi_show_hex(x);
    printf("\n=>\n");
    right_shift_word(x,2);
    bi_show_hex(x);

    printf("\ncase 2: \n");
    bi_gen_rand(&x, NEGATIVE, 5); // 5 워드 길이의 양수
    bi_show_hex(x);
    printf("\n=>\n");
    right_shift_word(x,1);
    bi_show_hex(x);
    bi_delete(&x);
}
/*
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
*/
void test_bi_long_div()
{         
    for(int i = 0 ; i < TEST_CASE; i++)
    {    
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *q = NULL;
        bigint *r = NULL;
        
        int wordlen1 = rand() % 128 + 1; 
        int wordlen2 = rand() % 128 + 1;
        
        int sign_x = (rand() % 2 == 0) ? NON_NEGATIVE : NEGATIVE;
    
        bi_gen_rand(&x, sign_x, wordlen1);
        bi_gen_rand(&y, NON_NEGATIVE, wordlen2);
        
        //r = 0 인 경우 테스트
        /*bigint* temp=NULL;
        bigint* mul=NULL;
        bi_gen_rand(&mul, sign_x, 30);

        mul_core_improved(&y, &mul, &temp);
        bi_assign(&x, temp); //x = 2y*/
        

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

void test_barret_reduction(){

    for(int i=0; i<TEST_CASE; i++){

        int N_len = rand() % 128 + 1;
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

        Squaring(&Wn, &tmp); 
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

void test_sub() 
{  
    for(int i = 0 ; i < TEST_CASE; i++)
    {
        bigint *x = NULL;
        bigint *y = NULL;
        bigint *z = NULL;

        //랜덤 생성
        int wordlen1 = rand() % 128 + 1; 
        int wordlen2 = rand() % 128 + 1;
        int sign_x = (rand() % 2 == 0) ? NON_NEGATIVE : NEGATIVE;
        int sign_y = (rand() % 2 == 0) ? NON_NEGATIVE : NEGATIVE;
        bi_gen_rand(&x, sign_x, wordlen1);
        bi_gen_rand(&y, sign_y, wordlen2);            

        //A가 0
        /*
        bi_set_zero(&x);
        bi_gen_rand(&y, sign_y, wordlen2);
        */

        //B가 0
        /*
        bi_set_zero(&y);
        bi_gen_rand(&x, sign_x, wordlen1);
        */

        //A=B 
        /*
        bi_gen_rand(&x, NEGATIVE, wordlen1);
        bi_assign(&y, x);
        y->sign=NON_NEGATIVE;
        */

        //A= - |B| A<0 B>0
        /*
        bi_gen_rand(&x, NEGATIVE, wordlen1);
        bi_assign(&y, x);
        y->sign=NON_NEGATIVE;
        */

       //A= - |B| A>0 B<0
        /*
        bi_gen_rand(&x, NON_NEGATIVE, wordlen1);
        bi_assign(&y, x);
        y->sign=NEGATIVE;
        */

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