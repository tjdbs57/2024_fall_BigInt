#include "config.h"

/**
 * @brief Allocates memory for a new bigint structure.
 *
 * This function creates a new bigint structure and initializes it with the specified
 * word length. The pointer to the newly created bigint is stored in the provided
 * pointer to a pointer (x).
 *
 * @param[out] x      Pointer to the bigint structure to be initialized.
 * @param[in]  wordlen The number of words to allocate for the bigint.
 * @return void
 */
void bi_new(bigint** x, int wordlen);

/**
 * @brief Frees the memory occupied by a bigint structure.
 *
 * This function releases the memory allocated for the bigint structure and sets the
 * pointer to NULL to avoid dangling references.
 *
 * @param[in,out] x Pointer to the bigint structure to be deleted.
 * @return void
 */
void bi_delete(bigint** x);

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
 * @brief Initializes a bigint structure using an array of words and sets its sign.
 * 
 * @param x      Pointer to the bigint structure to be initialized.
 * @param sign   The sign of the bigint (NEGATIVE or NON_NEGATIVE).
 * @param a      Array of words to set the value of the bigint.
 * @param wordlen Length of the word array.
 * @return int
 */
int bi_set_by_array(bigint** x, int sign, word* a, int wordlen);

/**
 * @brief Initializes a bigint structure from a character string representing a number.
 * 
 * @param x     Pointer to the bigint structure to be initialized.
 * @param sign   The sign of the bigint (NEGATIVE or NON_NEGATIVE).
 * @param str    The string representation of the number to be converted.
 * @param base   The base of the number system (e.g., 10 for decimal).
 * @return int
 */
int bi_set_by_string(bigint** x, int sign, char* str, int base);

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

/**
 * @brief Compare the absolute values of two bigint numbers.
 * 
 * This function compares the magnitudes of two bigint numbers without considering their signs.
 * 
 * @param x Pointer to the first bigint number.
 * @param y Pointer to the second bigint number.
 * @return 1 if |x| > |y|, -1 if |x| < |y|, or 0 if |x| == |y|.
 */
int compareABS(bigint* x, bigint* y);

/**
 * @brief Compare two bigint numbers, taking into account their signs.
 *
 * This function compares two bigint numbers, considering both their magnitudes and signs.
 * 
 * @param x Pointer to the first bigint number.
 * @param y Pointer to the second bigint number.
 * @return 1 if x > y, -1 if x < y, or 0 if x == y.
 */
int compare(bigint* x, bigint* y);

/**
 * @brief Gets the j-th bit of a bigint.
 *
 * This function retrieves the value of the j-th bit in the bigint.
 *
 * @param[in] x Pointer to the bigint from which the bit is retrieved.
 * @param[in] j The index of the bit to retrieve (0-based).
 * @return 0 if the bit is 0, 1 if the bit is 1, and -1 if the input is invalid.
 */
int get_jth_bit(bigint* x, u32 j);

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

#define CHECK_MEM_ALLOCATION(x)                                      \
    if ((x) == NULL) {                                               \
        printf("Memory allocation failed.\n");                       \
        exit(1);                                                     \
    }

#define CHECK_SET_STRING(str, base)                                   \
    if ((str) == NULL || (base) < 2 || (base) > 16) {              \
        printf("Error: Invalid input for string or base!\n");      \
        exit(1);                                                    \
    }

#define CHECK_SET_ARRAY(a, wordlen)                               \
    if ((a) == NULL || (wordlen) <= 0) {                          \
        printf("set array fail!\n");                               \
        exit(1);                                                  \
    }