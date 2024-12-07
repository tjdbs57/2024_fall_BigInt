/**
 * @file bigint.h
 * @brief This file defines the data structures and macros for representing and manipulating
 *        big integers.
 * 
 * It provides a flexible data type definition for large integers (big integers) using 
 * conditional compilation based on the word size (8, 32, or 64 bits). The `bigint` structure
 * is used to represent big integers, which consist of a sign, the length of the number in 
 * words, and a pointer to the array holding the actual big integer value.
 * 
 * Key components:
 * - Conditional compilation to define the `word` type based on the word size.
 * - The `bigint` structure to represent a big integer with a sign, word length, and the integer's value.
 * - Macros for calculating the maximum and minimum of two values.
 * 
 * The `bigint` structure allows for flexible handling of large integer arithmetic.
 */


#ifndef BIGINT_H
#define BIGINT_H
#include "config.h"

/**
 * @brief Defines the bit length of a word.
 * @details Specifies the number of bits in a single word used for the `bigint` data type.
 * @note This macro controls the word size, which impacts memory usage and computation speed.
 */
#define WORD_BITLEN 8

/**
 * @brief Defines the word type based on the `WORD_BITLEN`.
 * @details Allows flexibility in choosing the word size for the `bigint` structure.
 * - If `WORD_BITLEN` is 8, the word type is `u8` (8-bit unsigned integer).
 * - If `WORD_BITLEN` is 32, the word type is `u32` (32-bit unsigned integer).
 * - For any other `WORD_BITLEN`, the word type defaults to `u64` (64-bit unsigned integer).
 * @note Ensure that `WORD_BITLEN` is properly defined to avoid compilation errors.
 */
#if WORD_BITLEN == 8
typedef u8 word;  /**< Word is defined as an 8-bit unsigned integer. */
#elif WORD_BITLEN == 32
typedef u32 word; /**< Word is defined as a 32-bit unsigned integer. */
#else
typedef u64 word; /**< Word is defined as a 64-bit unsigned integer. */
#endif

/**
 * @brief Macro for representing the constant one (`1`) in the `word` type.
 * @details Casts the value `1` to the current `word` type.
 */
#define ONE (word)1

/**
 * @brief Macro for representing the constant zero (`0`) in the `word` type.
 * @details Casts the value `0` to the current `word` type.
 */
#define ZERO (word)0

/**
 * @struct bigint
 * @brief Represents a big integer for arbitrary-precision arithmetic.
 * @details The `bigint` structure contains:
 * - `sign`: The sign of the integer (`NEGATIVE` or `NON_NEGATIVE`).
 * - `wordlen`: The number of words used to represent the integer (must be non-negative).
 * - `a`: A pointer to an array of `word` elements that store the integer's value.
 */
typedef struct {
    int sign;      /**< The sign of the big integer (`NEGATIVE` or `NON_NEGATIVE`). */
    int wordlen;   /**< The length of the `a` array in words. */
    word* a;       /**< Pointer to the array storing the big integer's data. */
} bigint;

/**
 * @brief Returns the maximum of two values.
 * @details A macro to compute the greater of two values `x1` and `x2`.
 * @param x1 The first value.
 * @param x2 The second value.
 * @return The maximum of `x1` and `x2`.
 * @note Both `x1` and `x2` should be of the same data type and comparable.
 */
#define MAXIMUM(x1, x2) (x1 > x2 ? x1 : x2)

/**
 * @brief Returns the minimum of two values.
 * @details A macro to compute the smaller of two values `x1` and `x2`.
 * @param x1 The first value.
 * @param x2 The second value.
 * @return The minimum of `x1` and `x2`.
 * @note Both `x1` and `x2` should be of the same data type and comparable.
 */
#define MINIMUM(x1, x2) (x1 < x2 ? x1 : x2)

#endif