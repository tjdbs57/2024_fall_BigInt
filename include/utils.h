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