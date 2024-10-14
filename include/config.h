#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef CONFIG_H
#define CONFIG_H

#define NON_NEGATIVE    0
#define NEGATIVE       -1

typedef uint8_t   u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t    i8;
typedef int32_t  i32;
typedef int64_t  i64;

#define WORD_BITLEN 32

// Conditional compilation based on the word size for data type flexibility
#if WORD_BITLEN == 8
typedef u8 word;
#define ONE (u8)1
#elif WORD_BITLEN == 32
typedef u32 word;
#define ONE (u32)1
#else
typedef u64 word;
#define ONE (u64)1
#endif

typedef struct{
    int         sign; // NEGATIVE or NON-NEGATIVE
    int     wordlen; // wordlen >= 0
    word*         a; // address for big integer
} bigint;

#endif 



