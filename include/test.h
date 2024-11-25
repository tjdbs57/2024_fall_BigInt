#ifndef TEST_H

#include "arithmetic.h"
#define TEST_CASE 10000

void test_set_by_array();
void test_bi_string();
void test_add();
void print_bi_hex_py(IN const bigint* x);
void test_sub_core(); 
void test_mul();
//void mul_core_improved(IN bigint** x, IN bigint** y, OUT bigint** z);
double measure_time(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
void measure_cycles(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
//measure_cycles(add, x, y, z);
void measure();
void test_bi_long_div();
int measure_mem(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
#endif
