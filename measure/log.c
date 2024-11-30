#include "log.h"

// void log_performance_results() {

//     bigint *x = NULL;
//     bigint *y = NULL;
//     bigint *z = NULL;

//     int sign1 = rand() % 2;
//     int sign2 = rand() % 2;
//     int wordlen1 = rand() % 96 + 1;
//     int wordlen2 = rand() % 96 + 1;
//     bi_gen_rand(&x, sign1, wordlen1);
//     bi_gen_rand(&y, sign2, wordlen2);
//     FILE *file = fopen("performance_results.csv", "w");
//     fprintf(file, "Algorithm,Execution Time (s),CPU Cycles,Memory Usage (KB)\n");

//     // 예시로 세 개의 곱셈 함수에 대해 성능을 측정
//     fprintf(file, "mul_core_tx,%f,%llu,%zu\n", 
//             measure_execution_time(mul_core_tx, &x, &y, &z),
//             measure_cycles(mul_core_tx, &x, &y, &z),
//             measure_memory_usage(mul_core_tx, &x, &y, &z));

//     fprintf(file, "mul_core_improved,%f,%llu,%zu\n", 
//             measure_execution_time(mul_core_improved, &x, &y, &z),
//             measure_cycles(mul_core_improved, &x, &y, &z),
//             measure_memory_usage(mul_core_improved, &x, &y, &z)
//             //measure_cache_misses(mul_core_improved)
//             );

//     fprintf(file, "mul_core_karatsuba,%f,%llu,%zu\n", 
//             measure_execution_time(mul_core_karatsuba, &x, &y, &z),
//             measure_cycles(mul_core_karatsuba, &x, &y, &z),
//             measure_memory_usage(mul_core_karatsuba, &x, &y, &z)
//             //measure_cache_misses(mul_core_karatsuba)
//             );

//     fclose(file);

//     bi_delete(&x);
//     bi_delete(&y);
//     bi_delete(&z);

// }
