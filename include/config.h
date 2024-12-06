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
/**
 * @brief Indicates that the program is being compiled for a 32-bit x86 architecture.
 * @details This block is conditionally included when the compiler detects a 32-bit x86 architecture.
 */
#elif defined(__x86_64)
/**
 * @brief Indicates that the program is being compiled for a 64-bit x86 architecture.
 * @details This block is conditionally included when the compiler detects a 64-bit x86 architecture.
 * @note Includes the `<x86intrin.h>` header file to provide access to x86-specific intrinsics.
 */
#include <x86intrin.h>
#endif

/**
 * @brief Defines input/output direction macros for function parameters.
 * @details These macros provide clarity about the purpose of function parameters:
 * - `IN`: Indicates that the parameter is used as input and will not be modified.
 * - `OUT`: Indicates that the parameter is used as output and will be modified by the function.
 * - `INOUT`: Indicates that the parameter is both input and output, meaning it will be modified.
 * @note These are purely for documentation and readability purposes and do not affect the code behavior.
 */
#define IN
#define OUT
#define INOUT

/**
 * @brief Sign definitions for bigint values.
 * @details These macros define constants to represent the sign of a bigint:
 * - `NON_NEGATIVE`: Represents non-negative integers (0 or positive values).
 * - `NEGATIVE`: Represents negative integers.
 */
#define NON_NEGATIVE    0
#define NEGATIVE        1

/**
 * @brief Defines a generic flag constant.
 * @details This macro defines a constant value `FLAG` with a default value of `20`. 
 * @note The purpose of this flag may vary depending on the specific implementation.
 */
#define FLAG    20

/**
 * @brief Type aliases for unsigned integers.
 * @details Provides platform-independent type definitions for unsigned integers:
 * - `u8`: 8-bit unsigned integer (alias for `uint8_t`).
 * - `u32`: 32-bit unsigned integer (alias for `uint32_t`).
 * - `u64`: 64-bit unsigned integer (alias for `uint64_t`).
 */
typedef uint8_t   u8;
typedef uint32_t u32;
typedef uint64_t u64;

/**
 * @brief Type aliases for signed integers.
 * @details Provides platform-independent type definitions for signed integers:
 * - `i8`: 8-bit signed integer (alias for `int8_t`).
 * - `i32`: 32-bit signed integer (alias for `int32_t`).
 * - `i64`: 64-bit signed integer (alias for `int64_t`).
 */
typedef int8_t    i8;
typedef int32_t  i32;
typedef int64_t  i64;


#endif 