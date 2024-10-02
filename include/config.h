#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef CONFIG_H
#define CONFIG_H

#define NON_NEGATIVE    0
#define NEGATIVE       -1

typedef int8_t    i8;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t   u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u32     word;

typedef struct{
    int         sign; // NEGATIVE or NON-NEGATIVE
    int     wordlen; // wordlen >= 0
    word*         a; // address for big integer
} bigint;

#endif 