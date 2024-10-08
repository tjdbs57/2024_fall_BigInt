#include "config.h"

void bi_new(bigint** x, u32 wordlen);
void bi_delete(bigint** x);

void bi_assign(bigint** y, bigint* x);
void bi_gen_rand(bigint**x, int sign, int wordlen);
void array_rand(word* dst, int wordlen);

/**
 * @brief Displays the hexadecimal representation of a bigint.
 *
 * This function prints the value of the bigint in hexadecimal format. 
 * It also handles the sign of the bigint and checks for invalid inputs.
 *
 * @param[in] x Pointer to the bigint to be displayed. 
 *              If NULL, an error message is printed.
 */
void bi_show_hex(bigint* x); 

/**
 * @brief Gets the j-th bit of a bigint.
 *
 * This function retrieves the value of the j-th bit in the bigint.
 *
 * @param[in] x Pointer to the bigint from which the bit is retrieved.
 * @param[in] j The index of the bit to retrieve (0-based).
 * @return 0 if the bit is 0, 1 if the bit is 1, and -1 if the input is invalid.
 */
int get_jth_bit(bigint* x, unsigned int j);

/**
 * @brief Retrieves the sign bit of a bigint.
 *
 * This function checks if the provided bigint pointer is valid
 * and returns its sign. A return value of 0 indicates a 
 * non-negative number, while a return value of 1 indicates a 
 * negative number. If the input is NULL, an error message 
 * is printed, and -1 is returned to indicate an error.
 *
 * @param[in] x Pointer to the bigint from which to retrieve the sign.
 * @return int The sign of the bigint (0 for non-negative, 1 for negative).
 *             Returns -1 if the input is NULL.
 */
int get_sign_bit(bigint* x);

/**
 * @brief Flips the sign bit of a bigint.
 *
 * This function checks if the provided bigint pointer is valid.
 * If valid, it toggles the sign of the bigint. A sign of 0 
 * becomes 1 (negative), and a sign of 1 becomes 0 (non-negative).
 * If the input is NULL, an error message is printed, and the
 * function does not perform any operation.
 *
 * @param[in,out] x Pointer to the bigint whose sign bit will be flipped.
 */
void flip_sign_bit(bigint* x);

/**
 * @brief Returns the number of words in a bigint structure.
 *
 * This function retrieves the word length of the provided bigint structure.
 *
 * @param a A pointer to the bigint structure. This must not be NULL.
 *
 * @return The word length of the bigint structure. If the input is NULL,
 *         an error code of -1 is returned.
 */
int get_word_length(bigint* a);

/**
 * @brief Calculate the bit length of a bigint.
 * 
 * This function computes the number of bits required to represent a
 * given bigint. It takes into account the sign of the bigint and
 * counts leading zeros to accurately determine the bit length.
 *
 * @param a Pointer to the bigint structure whose bit length is to be calculated.
 *          If a is NULL, an error message will be printed, and -1 will be returned.
 *
 * @return The bit length of the bigint as an integer.
 *         If the bigint is zero, the return value is 1.
 *         If a is NULL, the return value is -1 indicating an error.
 */
int get_bit_length(bigint* a);

/**
 * @brief Performs a left bitwise shift on a bigint structure by a specified number of bits.
 * 
 * This function shifts the bigint `a` to the left by `shift` bits. It handles both word-level 
 * and bit-level shifts. If necessary, it allocates more memory to accommodate the larger result. 
 * It also ensures that unnecessary leading zeros are removed from the bigint after the shift.
 * 
 * @param a Pointer to the bigint structure that will be shifted.
 *          The structure is modified in place.
 * @param shift The number of bits to shift left.
 *              Must be a positive integer greater than zero.
 * 
 * @note If the bigint `a` is NULL or `shift` is less than or equal to 0, the function returns 
 *       without making any changes.
 * 
 * @warning This function allocates new memory for the bigint's internal array.
 *          Make sure to handle memory management carefully to avoid memory leaks.
 * 
 * @note After the shift, the function automatically reduces the word length (`wordlen`) if there 
 *       are leading words with zero values.
 * 
 * @warning If memory allocation for the new array fails, the program will terminate with an error message.
 */
void left_shift(bigint* a, int l);

/**
 * @brief Performs a right bit shift on a bigint.
 * 
 * Shifts the given bigint structure to the right by the specified number of bits.
 * 
 * @param a Pointer to the bigint structure to be shifted. If NULL, no operation is performed.
 * @param shift The number of bits to shift to the right. No operation is performed if less than or equal to 0.
 */
void right_shift(bigint* a, int shift);

void reduction(bigint* A, int r, bigint* result);