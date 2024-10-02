#include "config.h"

void bi_new(bigint** x, int wordlen);
void bi_delete(bigint** x);

/**
 * Function: bi_set_by_array
 * Description: Initializes a bigint structure using an array of words and sets its sign.
 * @param x      Pointer to the bigint structure to be initialized.
 * @param sign   The sign of the bigint (NEGATIVE or NON_NEGATIVE).
 * @param a      Array of words to set the value of the bigint.
 * @param wordlen Length of the word array.
 * @return void
 */
void bi_set_by_array(bigint** x, int sign, word* a, int wordlen);

/**
 * Function: bi_set_by_string
 * Description: Initializes a bigint structure from a character string representing a number.
 * @param x     Pointer to the bigint structure to be initialized.
 * @param sign   The sign of the bigint (NEGATIVE or NON_NEGATIVE).
 * @param str    The string representation of the number to be converted.
 * @return void
 */
void bi_set_by_string(bigint** x, int sign, char* str, int base);

/**
 * @brief Refines the bigint structure by removing leading zeros.
 *
 * This function updates the word length of the bigint and reallocates memory
 * if necessary. It also sets the sign to NON_NEGATIVE if the bigint becomes zero.
 *
 * @param[in,out] x Pointer to the bigint to be refined.
 */
void bi_refine(bigint* x);

/**
 * @brief Assigns the value of one bigint to another.
 *
 * This function deletes the existing destination bigint, 
 * creates a new bigint of the same word length as the source, 
 * and copies the sign, word length, and value from the source bigint.
 *
 * @param[out] dest Pointer to the destination bigint to be assigned to.
 * @param[in] src Pointer to the source bigint to copy from.
 */
void bi_assign(bigint** y, bigint* x);

/**
 * @brief Generates a random bigint with a specified sign and word length.
 *
 * This function creates a new bigint, sets its sign, generates random values
 * for its word array, and then refines it to remove any leading zeros.
 *
 * @param[out] x Pointer to the bigint to be generated.
 * @param[in] sign The sign of the bigint (NEGATIVE or NON_NEGATIVE).
 * @param[in] wordlen The desired length of the word array.
 */
void bi_gen_rand(bigint**x, int sign, int wordlen);

/**
 * @brief Fills an array with random bytes.
 *
 * This function populates the specified array with random bytes using the rand function.
 *
 * @param[out] dst Pointer to the array to be filled with random values.
 * @param[in] wordlen The number of words to fill.
 */
void array_rand(word* dst, int wordlen);

/**
 * @brief Sets a bigint to the value of one.
 *
 * This function initializes a new bigint structure and sets its value to one.
 * The sign of the bigint is set to NON_NEGATIVE.
 *
 * @param[out] x Pointer to the bigint structure to be initialized and set to one.
 */
void bi_set_one(bigint** x);

/**
 * @brief Sets a bigint to the value of zero.
 *
 * This function initializes a new bigint structure and sets its value to zero.
 * The sign of the bigint is set to NON_NEGATIVE.
 *
 * @param[out] x Pointer to the bigint structure to be initialized and set to zero.
 */
void bi_set_zero(bigint** x);




