#include "config.h"


/* Return Maximum of x1 and x2. */
#define MAXIMUM(x1, x2) (x1 > x2 ? x1 : x2)

/* Return Minimum of x1 and x2. */
#define MINIMUM(x1, x2) (x1 < x2 ? x1 : x2)

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

/**
 * @brief Swaps two bigint pointers.
 *
 * This function takes two pointers to bigint structures and swaps
 * their values. It is useful for ensuring that the larger bigint
 * is processed first in operations like addition.
 *
 * @param x Pointer to the first bigint pointer.
 * @param y Pointer to the second bigint pointer.
 */
void swap_bigint(bigint** x, bigint** y);

/**
 * @brief Adds two words with carry handling.
 *
 * This function performs addition on two words (A and B) and incorporates
 * a carry from a previous addition. It checks for overflow and sets the
 * carry_out accordingly.
 *
 * @param A First word to add.
 * @param B Second word to add.
 * @param carry_in Carry input from a previous addition.
 * @param carry_out Pointer to store the carry output.
 * @param result Pointer to store the result of the addition.
 */
void add_carry(word A, word B, word carry_in, word* carry_out, word* result);

/**
 * @brief Core function for adding two bigints.
 *
 * This function adds two bigints (x and y) and stores the result in z.
 * It handles carry propagation and ensures the result has the correct length.
 *
 * @param x Pointer to the first bigint (augend).
 * @param y Pointer to the second bigint (addend).
 * @param z Pointer to the bigint where the result will be stored.
 */
void add_core(bigint** x, bigint** y, bigint** z);