/**
 * @file test.h
 * @brief This file declares the functions and definitions used for testing and measuring 
 *        the performance of various arithmetic operations in the BigInt library.
 * 
 * It includes function prototypes for testing basic arithmetic operations, exponentiation, 
 * division, squaring, and modular operations. Additionally, it provides functions for measuring 
 * the execution time and clock cycles for these operations.
 * 
 * Key components:
 * - Definitions like `TEST_CASE` and `MAX_BIT_LEN` for configuring test parameters.
 * - Test functions for basic arithmetic operations, exponentiation, division, squaring, and Barret reduction.
 * - Performance measurement functions that evaluate execution time and clock cycles.
 * 
 * The file is intended to be used in unit tests and performance benchmarking for the BigInt 
 * library's arithmetic operations.
 */

#ifndef TEST_H

#include "arithmetic.h"
#define TEST_CASE 1
#define MAX_BIT_LEN    256

void test();

void test_basic_operation(void(*operation)(IN bigint** , IN bigint**, OUT bigint**), const char* operator, const char* operate);
void test_exp_mod(void(*operation)(IN bigint** , IN bigint**, OUT bigint**, IN bigint**),const char* operate);
void test_div(const char* operate);
void test_barret(const char* operate);
void test_squ(void(*operation)(IN bigint** , OUT bigint**), const char* operate);

double measure_execution_time(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);
void measure_cycles(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);

void measure();
void measure_clock_cycles();

#endif
