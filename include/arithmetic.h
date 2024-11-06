#ifndef ARITHMETIC_H
#include "utils.h"

/**
 * @brief Adds two words with carry handling.
 *
 * This function performs addition on two words (A and B) and incorporates
 * a carry from a previous addition. It checks for overflow and sets the
 * carry_out accordingly.
 *
 * @param[in] A First word to add.
 * @param[in] B Second word to add.
 * @param[in] carry_in Carry input from a previous addition.
 * @param[out] carry_out Pointer to store the carry output.
 * @param[out] result Pointer to store the result of the addition.
 */
void add_single_word(IN word A, IN word B, IN word carry_in, OUT word* carry_out, OUT word* result);

/**
 * @brief Adds two big integers and stores the result in a third bigint.
 *
 * This function takes two big integers represented as `bigint` structures, 
 * adds them together, and stores the resulting bigint in a third `bigint`.
 * It ensures that both numbers have the same length by padding with zeros 
 * if necessary. The addition is performed in reverse order to handle 
 * carry appropriately.
 *
 * @param[in] x Pointer to the first bigint to add.
 * @param[in] y Pointer to the second bigint to add.
 * @param[out] z Pointer to the result bigint where the sum will be stored.
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
void add_core(IN bigint** x, IN bigint** y, OUT bigint** z);

/**
 * @brief Performs subtraction of B from A and manages borrowing.
 *
 * This function subtracts the second word (B) from the first word (A),
 * taking into account any previous borrowing. It updates the result 
 * and the borrow status accordingly.
 *
 * @param A          The first word, the value from which to subtract.
 * @param B          The second word, the value to be subtracted.
 * @param borrow_in  The previous borrow value.
 * @param borrow_out Pointer to store the resulting borrow status.
 * @param result     Pointer to store the subtraction result.
 */
void sub_single_word(IN word A, IN word B, IN word borrow_in, OUT word* borrow_out, OUT word* result);

/**
 * @brief Performs subtraction of two big integers.
 *
 * This function subtracts the big integer y from the big integer x and stores 
 * the result in z. It handles borrowing during subtraction and manages the 
 * necessary memory for the result.
 *
 * @param x Pointer to the first bigint, the minuend.
 * @param y Pointer to the second bigint, the subtrahend.
 * @param z Pointer to store the result of the subtraction.
 */
void sub_core(IN bigint** x, IN bigint** y, OUT bigint** z);

/**
 * @brief Subtracts big integer y from big integer x and stores the result in z.
 * 
 * This function handles various cases depending on the signs and values of x and y, 
 * including when either operand is zero, when their absolute values are equal, 
 * and when their signs differ.
 * 
 * @param[in] x Pointer to a pointer of the first bigint structure (minuend).
 * @param[in] y Pointer to a pointer of the second bigint structure (subtrahend).
 * @param[out] z Pointer to a pointer of the result bigint structure.
 * 
 * - If x is zero, the function sets z to -y.
 * - If y is zero, the function sets z to x.
 * - If the absolute values of x and y are equal:
 *   - If x and y have the same sign, z is set to zero.
 *   - If x and y have opposite signs, their absolute values are added, and z has the sign of x.
 * - If x and y are both positive, the function subtracts the smaller magnitude from the larger one and assigns the appropriate sign to z.
 * - If x and y are both negative, it subtracts the smaller magnitude from the larger and assigns a negative sign if x has a larger magnitude, or a positive sign if y is larger.
 * - If x and y have opposite signs, their absolute values are added and z has the sign of x.
 */
void sub(bigint** x, bigint** y, bigint** z);



/**
 * @brief Adds two big integers x and y, and stores the result in z.
 * 
 * The function handles cases where one or both of the operands are zero, 
 * as well as when the signs of x and y are different. If the signs of x and y 
 * are different, it performs subtraction instead. If both have the same sign, 
 * it performs addition.
 * 
 * @param[in] x Pointer to a pointer of the first bigint structure.
 * @param[in] y Pointer to a pointer of the second bigint structure.
 * @param[out] z Pointer to a pointer of the result bigint structure.
 * 
 * - If x is zero, the function assigns y to z.
 * - If y is zero, the function assigns x to z.
 * - If x and y have opposite signs, the function subtracts the smaller magnitude from the larger one.
 * - If x and y have the same sign, the function adds them and assigns the resulting sign to z.
 */
void add(IN bigint **x, IN bigint **y, OUT bigint **z);

#endif