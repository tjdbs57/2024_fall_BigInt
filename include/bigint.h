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

#define WORD_BITLEN 64

// Conditional compilation based on the word size for data type flexibility
#if WORD_BITLEN == 8
typedef u8 word;
#elif WORD_BITLEN == 32
typedef u32 word;
#else
typedef u64 word;
#endif

#define ONE     (word)1
#define ZERO    (word)0

// Big Integer Structure
typedef struct{
    int         sign; // NEGATIVE or NON-NEGATIVE
    int     wordlen; // wordlen >= 0
    word*         a; // address for big integer
} bigint;

/* Return Maximum of x1 and x2. */
#define MAXIMUM(x1, x2) (x1 > x2 ? x1 : x2)

/* Return Minimum of x1 and x2. */
#define MINIMUM(x1, x2) (x1 < x2 ? x1 : x2)

#endif