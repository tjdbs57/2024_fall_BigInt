#include "bigint.h"
#include "arithmetic.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>  // For dlopen, dlsym, dlclose

typedef void (*Arg_4_function)(bigint **, bigint **, bigint **, bigint **);
typedef void (*Arg_3_function)(bigint **, bigint **, bigint **);
typedef void (*Arg_2_function)(bigint**,  bigint** );

int main() {
    // 동적 라이브러리 로드 (Windows에서는 LoadLibrary, 리눅스에서는 dlopen)
    void *lib = dlopen("./libbigint.so", RTLD_LAZY);  // libbigint.so는 리눅스에서 사용하는 라이브러리 이름
    if (!lib) {
        printf("Failed to load libbigint.so: %s\n", dlerror());
        return 1;
    }

    // 함수 포인터 가져오기 (Windows의 GetProcAddress 대신 dlsym 사용)
    Arg_3_function add = (Arg_3_function)dlsym(lib, "add");
    Arg_3_function sub = (Arg_3_function)dlsym(lib, "sub");
    Arg_3_function mul = (Arg_3_function)dlsym(lib, "mul_core_karatsuba");
    Arg_2_function squ = (Arg_2_function)dlsym(lib, "squ_karatsuba");

    Arg_4_function div = (Arg_4_function)dlsym(lib, "bi_long_div");

    Arg_4_function exp_l2r = (Arg_4_function)dlsym(lib, "L2R");
    Arg_4_function exp_r2l = (Arg_4_function)dlsym(lib, "R2L");
    Arg_4_function exp_montgomery = (Arg_4_function)dlsym(lib, "exp_mod_montgomery");
    Arg_4_function reduction = (Arg_4_function)dlsym(lib, "barret_reduction");

    if (!add || !sub || !mul || !squ || !div || !exp_montgomery || !reduction) {
        printf("Failed to load functions from libbigint.so: %s\n", dlerror());
        dlclose(lib);  // 동적 라이브러리 닫기
        return 1;
    }

    
    // 동적 라이브러리 해제
    dlclose(lib);

    return 0;
}
