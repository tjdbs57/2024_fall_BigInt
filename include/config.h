#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#ifdef __i386
#elif defined(__x86_64)
#include <x86intrin.h>
#endif

#include <windows.h>
#include <psapi.h>

#define     IN
#define     OUT
#define     INOUT

#define NON_NEGATIVE    0
#define NEGATIVE        1

// at least (wordlen / 2)
#define FLAG    48

typedef uint8_t   u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t    i8;
typedef int32_t  i32;
typedef int64_t  i64;


#endif 