#ifndef ARITHMETIC_H
#include "utils.h"

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
 * @brief Adds two big integers and stores the result in a third bigint.
 *
 * This function takes two big integers represented as `bigint` structures, 
 * adds them together, and stores the resulting bigint in a third `bigint`.
 * It ensures that both numbers have the same length by padding with zeros 
 * if necessary. The addition is performed in reverse order to handle 
 * carry appropriately.
 *
 * @param x Pointer to the first bigint to add.
 * @param y Pointer to the second bigint to add.
 * @param z Pointer to the result bigint where the sum will be stored.
 *
 * This function performs the following steps:
 * - Determines the lengths of the input big integers.
 * - Allocates memory for the result bigint based on the maximum length of the inputs.
 * - Iteratively adds each corresponding word from the two input big integers,
 *   starting from the least significant word, while handling any carry from previous additions.
 * - Stores the result in the appropriate position in the result bigint.
 * - Handles any carry that may remain after the final addition.
 * - Calls `bi_refine` to remove any leading zeros from the result.
 *
 * @note The input big integers should be properly initialized and should not be NULL.
 *       The result bigint will be allocated and must be freed by the caller when no longer needed.
 */
void add_core(bigint** x, bigint** y, bigint** z);

#endif