/**
 * @file config.h
 * @brief This file contains configuration settings, type definitions, and necessary includes 
 *        for the project.
 * 
 * It includes standard library headers for functionality like input/output, memory management, 
 * and mathematical operations. Conditional compilation is used to include specific headers 
 * based on the target architecture (e.g., x86 or x86_64). It also defines custom type aliases 
 * and symbolic constants for use across the project.
 * 
 * Key components:
 * - Includes common standard libraries like `stdio.h`, `stdint.h`, and `stdlib.h`.
 * - Conditional inclusion of architecture-specific headers for optimizations (e.g., x86intrin.h).
 * - Type definitions for `u8`, `u32`, `u64`, `i8`, `i32`, and `i64` to represent various data types.
 * - Macros defining input (`IN`), output (`OUT`), and input/output (`INOUT`) argument conventions.
 * - Constants to represent non-negative and negative values, along with a `FLAG` definition.
 */


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


#define     IN
#define     OUT
#define     INOUT

#define NON_NEGATIVE    0
#define NEGATIVE        1


#define FLAG    20

typedef uint8_t   u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t    i8;
typedef int32_t  i32;
typedef int64_t  i64;


#endif 