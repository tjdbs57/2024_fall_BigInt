#ifndef ARITHMETIC_H
#include "utils.h"

/**
 * @brief Performs a bitwise OR operation on two binary integers.
 * @details This function computes the bitwise OR of two `bigint` objects `x` and `y`, and stores the result in `z`.
 *          It handles word-level operations and ensures that the result's word length and sign are properly set.
 * @param[in] x A double pointer to a `bigint` representing the first operand.
 * @param[in] y A double pointer to a `bigint` representing the second operand.
 * @param[out] z A double pointer to a `bigint` where the result of the bitwise OR operation will be stored.
 * @pre `x` and `y` must point to valid `bigint` objects with properly initialized word lengths and data arrays.
 *      `z` must be properly allocated to store the resulting `bigint`.
 * @post The bitwise OR result of `(*x)` and `(*y)` is stored in `(*z)`.
 *       The word length of `(*z)` is set to the maximum of the word lengths of `(*x)` and `(*y)`.
 *       If the signs of `(*x)` and `(*y)` differ, the sign of `(*z)` is set to `NEGATIVE`.
 * @note This function assumes that the `bigint` structure has attributes `a` (an array of words), 
 *       `wordlen` (length of the array), and `sign` (the sign of the integer).
 */
void or(IN bigint** x, IN bigint** y, OUT bigint** z);






/****************************************/
/*          Add operations               */
/****************************************/


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







/****************************************/
/*          Sub operations               */
/****************************************/

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
void sub(IN bigint** x, IN bigint** y, OUT bigint** z);






/****************************************/
/*          Multiplication operations    */
/****************************************/


/**
 * @brief Multiplies two single words and stores the result in a bigint structure.
 *
 * This function performs multiplication on two input words, `A` and `B`,
 * and stores the result as a two-word bigint in the `result` parameter.
 * It breaks down the multiplication into half-word operations to manage
 * potential overflow in high and low parts separately, allowing for more precise
 * handling of large word multiplications.
 *
 * @param[in] A The first input word.
 * @param[in] B The second input word.
 * @param[out] result Pointer to a bigint structure where the result will be stored.
 *                     The bigint structure is expected to have space for two words
 *                     (`a[0]` for the lower part and `a[1]` for the upper part).
 */
void mul_single_word(IN word A, IN word B, OUT bigint** result);

/**
 * @brief Multiplies two big integers and stores the result in the output.
 * 
 * This function performs multiplication of two big integers, `x` and `y`, and stores the result in `z`.
 * It adjusts for the sign of the result and handles multiplication at the word level with shifting and addition.
 * 
 * @param[in] x Pointer to a pointer of the first bigint operand.
 * @param[in] y Pointer to a pointer of the second bigint operand.
 * @param[out] z Pointer to a pointer of the resulting bigint product.
 */
void mul_core_tx(IN bigint** x, IN bigint** y, OUT bigint** z);

/**
 * @brief Multiplies two binary integers using an improved Textbook Multiplication algorithm.
 * @details Wrapper function that performs optimized multiplication of two BINTs pointed to by `x` and `y`, 
 *          and stores the result in `z`. This function improves the standard Textbook Multiplication 
 *          by aligning word lengths, splitting calculations into smaller chunks, and handling shifts efficiently.
 * @param[in] x A double pointer to a BINT representing the first operand.
 * @param[in] y A double pointer to a BINT representing the second operand.
 * @param[out] z A double pointer to a BINT where the result will be stored.
 * @pre Both `x` and `y` must point to valid, initialized BINT objects. 
 *      `z` must be properly allocated to store the resulting BINT.
 * @post The result of multiplication is stored in the location pointed to by `z`.
 * @note This function uses multiple helper functions, such as `mul_single_word`, `add_core`, 
 *       and `left_shift_word`, to manage BINT arithmetic and memory efficiently.
 */
void mul_core_improved(IN bigint** x, IN bigint** y, OUT bigint** z);

/**
 * @brief Multiplies two binary integers using the Karatsuba multiplication algorithm.
 * @details This function performs efficient multiplication of two binary integers (`x` and `y`) 
 *          using the divide-and-conquer Karatsuba algorithm. It recursively splits the inputs into 
 *          higher and lower parts, computes intermediate products, and combines them to calculate 
 *          the final result. For small input sizes, it falls back to an improved standard 
 *          multiplication (`mul_core_improved`) for efficiency.
 * @param[in] x A double pointer to a `bigint` representing the first operand.
 * @param[in] y A double pointer to a `bigint` representing the second operand.
 * @param[out] z A double pointer to a `bigint` where the result of the multiplication will be stored.
 * @pre Both `x` and `y` must point to valid, initialized `bigint` objects.
 *      `z` must be properly allocated to store the resulting `bigint`.
 * @post The result of the multiplication is stored in the location pointed to by `z`.
 *       Both `x` and `y` are refined back to their original states.
 * @note This function employs memory-efficient techniques for intermediate calculations, 
 *       and uses helper functions such as `right_shift_word`, `reduction`, and `add` 
 *       to manage the binary integer arithmetic.
 */
void mul_core_karatsuba(IN bigint** x, IN bigint** y, OUT bigint** z);





/****************************************/
/*          Squaring operations         */
/****************************************/

/**
 * @brief Squares a single word and stores the result in a bigint structure.
 *
 * This function computes the square of a single word `A` and stores the result
 * in a bigint structure pointed to by `result`.
 * 
 * @param[in] A The single word to be squared.
 * @param[out] result A pointer to the bigint structure to store the squared result.
 *
 * The squaring is done using a method that divides `A` into two halves:
 * `A1` (upper bits) and `A0` (lower bits). The square is calculated as:
 * - \(C[0] = A0 * A0\) (lower bits)
 * - \(C[1] = A1 * A1\) (upper bits)
 * - The cross multiplication \(T = A0 * A1\), left-shifted by `w + 1` bits
 * 
 * These intermediate results are combined to get the full square result.
 */
void squ_single_word(IN word A, OUT bigint** result);

/**
 * @brief Squares a bigint and stores the result in another bigint.
 *
 * This function performs the squaring of a bigint `x` using the method of
 * single-word squaring combined with cross products. The result is stored
 * in the bigint structure pointed to by `result`.
 * 
 * @param[in] x A pointer to the bigint to be squared.
 * @param[out] result A pointer to the bigint structure where the squared result will be stored.
 *
 * The squaring method splits the process into:
 * - Squaring individual words in `x` and shifting accordingly.
 * - Calculating the cross products between words and summing them with left shifts.
 * 
 * The function handles each word in `x` (up to `t` words) by:
 * - Calculating the square of each word `j` and adding the result to `C1`.
 * - Calculating the cross product for word pairs `(j, i)` where `i > j` and accumulating in `C2`.
 * 
 * After processing, `C2` is left-shifted by one bit to account for the doubling effect in cross terms.
 */
void squ_tx(IN bigint** x, OUT bigint** result);

/**
 * @brief Squares a bigint if it is non-zero and stores the result.
 *
 * This function squares the bigint `x` and stores the result in `result`. 
 * If `x` is zero, it directly assigns zero to `result` and sets its sign to non-negative.
 * Otherwise, it calls the `SQUC` function to compute the square of `x`.
 * 
 * @param[in] x A pointer to the bigint to be squared.
 * @param[out] result A pointer to the bigint structure where the squared result will be stored.
 *
 * The function first checks if `x` is zero:
 * - If `x` is zero, `result` is assigned the value of zero with a non-negative sign.
 * - Otherwise, it uses `SQUC` to perform the squaring.
 */
void squaring(IN bigint** x, OUT bigint** result);

/**
 * @brief Computes the square of a binary integer using the Karatsuba squaring algorithm.
 * @details This function efficiently computes the square of a binary integer `x` by utilizing 
 *          the Karatsuba algorithm, which reduces the number of multiplications compared to 
 *          traditional squaring methods. For small input sizes, the function falls back to a 
 *          standard squaring algorithm (`squ_tx`) for performance.
 * @param[in] x A double pointer to a `bigint` representing the binary integer to be squared.
 * @param[out] z A double pointer to a `bigint` where the result of the squaring operation will be stored.
 * @pre `x` must point to a valid, initialized `bigint` object. 
 *      `z` must be properly allocated to store the resulting `bigint`.
 * @post The result of squaring `x` is stored in the location pointed to by `z`.
 *       The input `x` remains unchanged after the operation.
 * @note This function uses memory-efficient recursive computations and helper functions 
 *       such as `left_shift_word`, `add`, and `mul_core_karatsuba` to manage intermediate results.
 */
void squ_karatsuba(IN bigint** x, OUT bigint** z);






/****************************************/
/*          Division operations         */
/****************************************/

/**
 * @brief Performs binary long division on big integers.
 * 
 * This function calculates the quotient (`q`) and remainder (`r`) when dividing bigint `*x` by bigint `*y`.
 * It handles special cases for zero and negative values and manages sign adjustments for the result.
 * 
 * @param[in,out] x Pointer to the dividend bigint. This value is read-only in the function.
 * @param[in] y Pointer to the divisor bigint. This value is read-only in the function.
 * @param[out] q Pointer to the quotient bigint, which will store the result of the division.
 * @param[out] r Pointer to the remainder bigint, which will store the remainder after division.
 * 
 * @details 
 * - If the divisor `*y` is zero, the function triggers an error since division by zero is undefined.
 * - If `*x` is zero, both quotient `*q` and remainder `*r` are set to zero.
 * - If the absolute value of `*x` is smaller than `*y`, `*q` is set to zero and `*r` to `*x`.
 * - If the absolute value of `*x` equals `*y`, `*q` is set to one and `*r` to zero, with sign adjustments if necessary.
 * 
 * - The main division loop shifts bits from `*x` into `*r`, checks if `*r >= *y`, and updates `*q` and `*r` accordingly.
 * - For negative dividends, the remainder `*r` is adjusted to ensure the remainder sign follows division rules.
 * 
 * @note 
 * - The function uses helper functions like `bi_set_zero`, `bi_assign`, `add`, `sub_core`, and `left_shift_bit` to perform operations on bigint structures.
 * - The sign of the quotient `*q` and remainder `*r` is adjusted based on the signs of `*x` and `*y`.
 * - The function assumes bigint pointers `x`, `y`, `q`, and `r` are properly initialized before calling.
 */
void bi_long_div(IN bigint** x, IN bigint** y, OUT bigint** q, OUT bigint** r);

/**
 * @brief Computes the quotient of a two-word dividend divided by a single-word divisor.
 * @details This function performs high-precision division using a two-word dividend 
 *          (`dividend1` as the high word and `dividend0` as the low word) and a single-word divisor. 
 *          The algorithm calculates the quotient (`Q`) and ensures that intermediate remainders (`R`) 
 *          are handled appropriately during bit-by-bit processing.
 * @param[in] dividend1 The higher significant word of the two-word dividend.
 * @param[in] dividend0 The lower significant word of the two-word dividend.
 * @param[in] divisor A single-word divisor.
 * @return The quotient resulting from the division of the two-word dividend by the divisor.
 * @pre `divisor` must be non-zero to avoid division by zero errors.
 * @post The returned quotient is the result of the division, and intermediate calculations ensure correctness.
 * @note This function processes the division bit-by-bit, starting from the most significant bits 
 *       and adjusting the remainder (`R`) and quotient (`Q`) iteratively.
 */
word quotient(IN word dividend1, IN word dividend0, IN word divisor) ;

/**
 * @brief Performs division of two large integers using an optimized algorithm.
 * @details This function calculates the quotient (`Q`) and remainder (`R`) when a large integer `x` is divided by `y`. 
 *          It handles cases where the word lengths of `x` and `y` differ by at most one and adjusts results to ensure
 *          the remainder (`R`) is non-negative. This function also refines results to account for any adjustments due 
 *          to negative remainders.
 * @param[in] x A double pointer to a `bigint` representing the dividend.
 * @param[in] y A double pointer to a `bigint` representing the divisor.
 * @param[out] Q A double pointer to a `bigint` where the quotient will be stored.
 * @param[out] R A double pointer to a `bigint` where the remainder will be stored.
 * @pre Both `x` and `y` must point to valid, initialized `bigint` objects, and `y` must not be zero.
 *      `Q` and `R` must be properly allocated to store the resulting values.
 * @post The quotient and remainder of `x / y` are stored in `Q` and `R`, respectively.
 *       The remainder `R` is guaranteed to be non-negative.
 * @note This function uses helper functions such as `mul_core_tx` for multiplication, 
 *       `sub` for subtraction, and `add` for addition. Memory management and refinement 
 *       (`bi_refine`) are handled to ensure intermediate results are properly cleaned.
 */
void general_div_core(IN bigint** x, IN bigint** y, IN bigint** Q, IN bigint** R);






/*******************************************************/
/*          Modulo exponentiation operations           */
/*******************************************************/

/**
 * @brief Computes modular exponentiation using the Left-to-Right binary method.
 * @details This function calculates \( z = x^y \mod M \) efficiently using a left-to-right 
 *          binary exponentiation algorithm. It processes the bits of `y` from the most significant 
 *          to the least significant bit, squaring the intermediate result and conditionally multiplying 
 *          based on the value of the current bit.
 * @param[in] x A double pointer to a `bigint` representing the base.
 * @param[in] y A double pointer to a `bigint` representing the exponent.
 * @param[out] z A double pointer to a `bigint` where the result will be stored.
 * @param[in] M A double pointer to a `bigint` representing the modulus.
 * @pre Both `x`, `y`, and `M` must point to valid, initialized `bigint` objects. `M` must be non-zero.
 * @post The result \( z = x^y \mod M \) is stored in the location pointed to by `z`.
 * @note This function uses modular reduction during every squaring and multiplication step to 
 *       ensure that intermediate results remain manageable in size.
 */
void L2R(IN bigint** x, IN bigint** y, IN bigint** z, OUT bigint** M);

/**
 * @brief Computes modular exponentiation using the Right-to-Left binary method.
 * @details This function calculates \( z = x^y \mod M \) using the right-to-left binary exponentiation algorithm.
 *          It iterates over the bits of `y` from the least significant to the most significant, updating the
 *          intermediate result and modular reduction efficiently at each step.
 * @param[in] x A double pointer to a `bigint` representing the base.
 * @param[in] y A double pointer to a `bigint` representing the exponent.
 * @param[out] z A double pointer to a `bigint` where the result will be stored.
 * @param[in] M A double pointer to a `bigint` representing the modulus.
 * @pre Both `x`, `y`, and `M` must point to valid, initialized `bigint` objects. `M` must be non-zero.
 * @post The result \( z = x^y \mod M \) is stored in the location pointed to by `z`.
 * @note This function employs modular reduction during each multiplication and squaring step to maintain manageable
 *       intermediate values and optimize performance.
 */
void R2L(IN bigint** x, IN bigint** y, OUT bigint** z, IN bigint** M) ;

/**
 * @brief Performs modular exponentiation using Montgomery multiplication.
 * @details This function computes \( z = x^y \mod M \) using the Montgomery multiplication method.
 *          It iterates over the bits of the exponent `y` from the most significant bit to the least significant bit,
 *          maintaining intermediate modular reductions to ensure efficient computation.
 * @param[in] x A double pointer to a `bigint` representing the base.
 * @param[in] y A double pointer to a `bigint` representing the exponent.
 * @param[out] z A double pointer to a `bigint` where the result will be stored.
 * @param[in] M A double pointer to a `bigint` representing the modulus.
 * @pre Both `x`, `y`, and `M` must point to valid, initialized `bigint` objects. `M` must be non-zero.
 * @post The result \( z = x^y \mod M \) is stored in the location pointed to by `z`.
 * @note This implementation uses Montgomery multiplication for efficient modular reduction at each step.
 */
void exp_mod_montgomery(IN bigint** x, IN bigint** y, OUT bigint** z, IN bigint** M) ;





/************************************************/
/*          Fast reduction operations           */
/************************************************/


/**
 * @brief Performs Barrett reduction to reduce `x` modulo `y`.
 * @details This function computes the result of \( x \mod y \) using the Barrett reduction method. 
 *          Barrett reduction is efficient for modular operations, especially when dealing with repeated modular 
 *          reductions during large integer arithmetic (e.g., in cryptographic algorithms).
 *          It involves approximating the inverse of `y` modulo a power of two and using it to reduce `x` modulo `y`.
 * @param[in] x A double pointer to a `bigint` representing the dividend.
 * @param[in] y A double pointer to a `bigint` representing the divisor.
 * @param[in] z A double pointer to a `bigint` representing the precomputed value used in the reduction.
 * @param[out] result A double pointer to a `bigint` where the result of the reduction will be stored.
 * @pre Both `x` and `y` must point to valid, initialized `bigint` objects. `y` must not be zero.
 * @post The result of \( x \mod y \) is stored in the location pointed to by `result`.
 * @note The function utilizes an approximation method to compute the modular result more efficiently. 
 *       The reduction steps are performed using several intermediate variables to prevent overflow and optimize calculations.
 */
void barret_reduction(IN bigint** x, IN bigint** y, IN bigint** z, OUT bigint** result);


#endif  //arithmetic.h

