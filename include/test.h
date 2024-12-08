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
/**
 * @brief Number of test cases for testing functions.
 * @details This macro defines the total number of iterations or test cases 
 *          that will be executed during the testing phase.
 * @note Increasing the value of `TEST_CASE` can lead to more thorough testing 
 *       but may significantly increase execution time.
 */
#define TEST_CASE 10000

/**
 * @brief Maximum bit length for `bigint` operations.
 * @details This macro sets the upper limit on the bit length of the numbers 
 *          generated for testing or operations. It determines the maximum size 
 *          of the `bigint` structure's data array.
 * @note Adjusting `MAX_BIT_LEN` affects memory usage and computational overhead. 
 *       Larger values allow for testing larger integers but require more resources.
 */
#define MAX_BIT_LEN 128

/**
 * @brief Calculates the time difference between two clock values.
 * @details This macro computes the elapsed time in seconds by subtracting the 
 *          starting clock value (`start`) from the ending clock value (`end`) 
 *          and dividing by `CLOCKS_PER_SEC`.
 * @param start The clock value at the start of the timed operation.
 * @param end The clock value at the end of the timed operation.
 * @return The elapsed time in seconds as a `double`.
 * @note This macro relies on the standard library macro `CLOCKS_PER_SEC` 
 *       to convert clock ticks into seconds.
 * @warning Ensure that `start` and `end` are valid `clock_t` values obtained 
 *          from `clock()` or similar functions.
 */
#define TIME(start, end) ((double)((end) - (start))) / CLOCKS_PER_SEC


/**
 * @brief Prints a bigint in hexadecimal format (Python-style).
 * @details This function prints the hexadecimal representation of the given `bigint` object. 
 *          The format is similar to Python's hexadecimal literals, prefixed with "0x".
 *          If the number is negative, a "-" sign is prepended.
 * @param[in] x A pointer to the `bigint` to be printed.
 * @pre `x` must point to a valid `bigint` object with an initialized array `a` and word length.
 * @post The hexadecimal representation of the `bigint` is printed to the standard output.
 * @note This function handles different word lengths based on the `WORD_BITLEN` macro.
 */
void print_bi_hex_py(IN const bigint* x) ;

/**
 * @brief Tests basic arithmetic operations on randomly generated `bigint` values.
 * @details This function generates random `bigint` values, performs the specified operation, and prints 
 *          the inputs and results in the format: `x [operator] y == z`.
 * @param[in] operation A function pointer to the arithmetic operation being tested.
 * @param[in] operator A string representing the symbol of the operation (e.g., "+", "-", "*").
 * @param[in] operate A string representing the name of the operation (used in output).
 * @pre `operation` must point to a valid arithmetic function accepting input and output `bigint` pointers.
 * @post The inputs, operation, and results are printed to the standard output.
 */
void test_basic_operation(void(*operation)(IN bigint** , IN bigint**, OUT bigint**), const char* operator, const char* operate);

/**
 * @brief Tests modular exponentiation with randomly generated values.
 * @details This function computes `(x^y) % mod` using the specified operation and prints the result.
 * @param[in] operation A function pointer to the modular exponentiation function being tested.
 * @param[in] operate A string representing the name of the operation (used in output).
 * @pre `operation` must point to a valid modular exponentiation function accepting input and output `bigint` pointers.
 * @post The inputs and the modular exponentiation result are printed to the standard output.
 */
void test_exp_mod(void(*operation)(IN bigint** , IN bigint**, OUT bigint**, IN bigint**),const char* operate);

/**
 * @brief Tests division by verifying the quotient and remainder.
 * @details This function generates random `bigint` values, computes the quotient and remainder using long 
 *          division, and verifies that the equation `x == q * y + r` holds.
 * @param[in] operate A string representing the name of the operation (used in output).
 * @pre The `bigint` inputs must be non-negative, and the divisor `y` must not be zero.
 * @post The division operation and verification results are printed to the standard output.
 */
void test_div(const char* operate);

/**
 * @brief Tests Barrett reduction for modular arithmetic.
 * @details This function verifies the Barrett reduction method for efficient modular reduction. It generates 
 *          random `bigint` values and precomputes necessary constants for Barrett reduction.
 * @param[in] operate A string representing the name of the operation (used in output).
 * @pre Input values must be valid `bigint` objects, and the modulus must be non-zero.
 * @post The reduction results are printed, verifying correctness.
 */
void test_barret(const char* operate);

/**
 * @brief Tests the squaring operation on randomly generated `bigint` values.
 * @details This function generates random `bigint` values, computes their square using the given 
 *          operation, and prints the inputs and results for verification.
 * @param[in] operation A function pointer to the squaring operation being tested.
 * @param[in] operate A string representing the name of the operation (used in output).
 * @pre `operation` must point to a valid squaring function accepting input and output `bigint` pointers.
 * @post The inputs and squared results are printed to the standard output.
 */
void test_squ(void(*operation)(IN bigint** , OUT bigint**), const char* operate);

/**
 * @brief Measures the average number of CPU cycles for a specific operation.
 * @details This function times the execution of a specified operation over multiple iterations to compute 
 *          the average number of CPU cycles consumed.
 * @param[in] func A function pointer to the operation being measured.
 * @param[in] x A pointer to the first `bigint` input.
 * @param[in] y A pointer to the second `bigint` input.
 * @param[out] z A pointer to the `bigint` where the result will be stored.
 * @pre `func` must be a valid function pointer, and all input/output `bigint` pointers must be initialized.
 * @post The average CPU cycles are printed to the standard output.
 */
void measure_cycles(void(*func)(IN bigint** , IN bigint**, OUT bigint**), IN bigint** x, IN bigint** y, OUT bigint** z);

/**
 * @brief Measures and prints the clock cycles for various operations.
 * @details This function executes different arithmetic operations multiple times and measures the 
 *          average CPU clock cycles required for each operation.
 * @pre All operations to be measured must have properly defined function implementations.
 * @post The average clock cycles for each operation are printed to the standard output.
 */
void measure_clock_cycles();

#endif
