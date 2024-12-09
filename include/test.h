#ifndef TEST_H

#include "arithmetic.h"
#define TEST_CASE 10000
void test();
void test_div();

void test_basic_operation(void(*operation)(IN bigint** , IN bigint**, OUT bigint**), const char* operator, const char* operate);
void test_exp_mod(void(*operation)(IN bigint** , IN bigint**, OUT bigint**, IN bigint**),const char* operate);
//void test_div(const char* operate);
void test_barret(const char* operate);
void test_squ(void(*operation)(IN bigint** , OUT bigint**), const char* operate);
double measure_execution_time(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
void measure_cycles(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
//measure_cycles(add, x, y, z);
void test_barret_reduction();

void measure();
void measure_clock_cycles();
size_t measure_memory_usage(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
#endif
