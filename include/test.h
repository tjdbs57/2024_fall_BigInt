#ifndef TEST_H

#include "arithmetic.h"
#define TEST_CASE 100
void test();void test_div();void test_exp_mod();void test_barret();;

void test_set_by_array();
void test_bi_string();
void test_basic_operation(void(*operation)(IN bigint** , IN bigint**, OUT bigint**), const char* operator);
double measure_execution_time(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
void measure_cycles(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
//measure_cycles(add, x, y, z);

void measure();
void measure_clock_cycles();
size_t measure_memory_usage(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
#endif
