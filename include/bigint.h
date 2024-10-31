#ifndef BIGINT_H
#define BIGINT_H
#include "config.h"

#define WORD_BITLEN 32

// Conditional compilation based on the word size for data type flexibility
#if WORD_BITLEN == 8
typedef u8 word;
#define ONE     (u8)1
#define ZERO    (u8)0
#elif WORD_BITLEN == 32
typedef u32 word;
#define ONE     (u32)1
#define ZERO    (u32)0
#else
typedef u64 word;
#define ONE     (u64)1
#define ZERO    (u64)0
#endif

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