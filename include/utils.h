#ifndef BIGINT_H
#include "bigint.h"

#define MEM_ALLOCATION_FAIL                                                    \
    fprintf(stderr, "Error: Memory allocation failed at %s:%d\n", __FILE__, __LINE__); 

#define SET_STRING_FAIL                                                     \
    fprintf(stderr, "Error: Invalid input for string or base at %s:%d\n", __FILE__, __LINE__);


#define SET_ARRAY_FAIL                                          \
    fprintf(stderr, "Error: Set array failed at %s:%d\n", __FILE__, __LINE__);


#define INVAILD_DATA                               \
    fprintf(stderr, "Error: Invalid data at %s:%d\n", __FILE__, __LINE__); 


    
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
void bi_new(IN bigint** x, IN int wordlen);

/**
 * @brief Initializes an array with a given length to a default value (0x00000000).
 *
 * This function sets all elements of the input array to 0. It is typically
 * used to clear or reset an array before performing other operations like
 * big integer addition or multiplication.
 *
 * @param[in] array The pointer to the array to be initialized.
 * @param[in] length The number of elements (length) of the array to initialize.
 *
 * @return void This function does not return any value.
 *
 * @note Ensure that the array has enough allocated memory to hold `length` elements.
 * @warning This function assumes the array is writable and properly allocated.
 */
void array_init(IN word* array, IN int length) ;

/**
 * @brief Frees the memory occupied by a bigint structure.
 *
 * This function releases the memory allocated for the bigint structure and sets the
 * pointer to NULL to avoid dangling references.
 *
 * @param[in,out] x Pointer to the bigint structure to be deleted.
 * @return void
 */
void bi_delete(INOUT bigint** x);

/**
 * @brief Displays the hexadecimal representation of a bigint.
 *
 * This function prints the value of the bigint in hexadecimal format. 
 * It also handles the sign of the bigint and checks for invalid inputs.
 *
 * @param[in] x Pointer to the bigint to be displayed. 
 *              If NULL, an error message is printed.
 */
void bi_show_hex(IN bigint* x) ;

/**
 * @brief Initializes a bigint structure using an array of words and sets its sign.
 * 
 * @param[out] x      Pointer to the bigint structure to be initialized.
 * @param[in] sign   The sign of the bigint (NEGATIVE or NON_NEGATIVE).
 * @param[in] a      Array of words to set the value of the bigint.
 * @param[in] wordlen Length of the word array.
 * @return int Returns 0 on success, non-zero on failure.
 */
int bi_set_by_array(OUT bigint** x, IN int sign, IN word* a, IN int wordlen); 

/**
 * @brief Initializes a bigint structure from a character string representing a number.
 * 
 * @param[out] x     Pointer to the bigint structure to be initialized (output).
 * @param[in]  sign   The sign of the bigint (NEGATIVE or NON_NEGATIVE) (input).
 * @param[in]  str    The string representation of the number to be converted (input).
 * @param[in]  base   The base of the number system (e.g., 10 for decimal) (input).
 * @return int Returns 0 on success, non-zero on failure.
 */
int bi_set_by_string(OUT bigint** x, IN int sign, IN char* str, IN int base); 

/**
 * @brief Refines the bigint structure by removing leading zeros.
 *
 * This function updates the word length of the bigint and reallocates memory
 * if necessary. It also sets the sign to NON_NEGATIVE if the bigint becomes zero.
 *
 * @param[in,out] x Pointer to the bigint to be refined (input/output).
 */
void bi_refine(INOUT bigint* x);

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
void bi_assign(OUT bigint** dest, IN bigint* src);

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
void bi_gen_rand(OUT bigint** x, IN int sign, IN int wordlen);

/**
 * @brief Fills an array with random bytes.
 *
 * This function populates the specified array with random bytes using the rand function.
 *
 * @param[out] dst Pointer to the array to be filled with random values.
 * @param[in] wordlen The number of words to fill.
 */
void array_rand(OUT word* dst, IN int wordlen);

/**
 * @brief Sets a bigint to the value of one.
 *
 * This function initializes a new bigint structure and sets its value to one.
 * The sign of the bigint is set to NON_NEGATIVE.
 *
 * @param[out] x Pointer to the bigint structure to be initialized and set to one.
 */
void bi_set_one(OUT bigint** x);

/**
 * @brief Sets a bigint to the value of zero.
 *
 * This function initializes a new bigint structure and sets its value to zero.
 * The sign of the bigint is set to NON_NEGATIVE.
 *
 * @param[out] x Pointer to the bigint structure to be initialized and set to zero.
 */
void bi_set_zero(OUT bigint** x);

/**
 * @brief Compare the absolute values of two bigint numbers.
 * 
 * This function compares the magnitudes of two bigint numbers without considering their signs.
 * 
 * @param[in] x Pointer to the first bigint number (input).
 * @param[in] y Pointer to the second bigint number (input).
 * @return 1 if |x| > |y|, -1 if |x| < |y|, or 0 if |x| == |y|.
 */
int compareABS(IN bigint* x, IN bigint* y);

/**
 * @brief Compare two bigint numbers, taking into account their signs.
 *
 * This function compares two bigint numbers, considering both their magnitudes and signs.
 * 
 * @param[in] x Pointer to the first bigint number (input).
 * @param[in] y Pointer to the second bigint number (input).
 * @return 1 if x > y, -1 if x < y, or 0 if x == y.
 */
int compare(IN bigint* x, IN bigint* y);


/**
 * @brief Calculate the bit length of a given bigint.
 *
 * This function calculates the bit length of the absolute value of a bigint.
 * It accounts for the number of leading zeros in the last word and adds an 
 * extra bit for the sign if the bigint is negative.
 *
 * @param[in] x A pointer to the bigint structure (input).
 * @return The bit length of the bigint. If there are no words, returns 0.
 *         If the bigint is negative, an extra bit for the sign is included.
 */
int get_bit_length(IN bigint* x);


/**
 * @brief Get the value of the j-th bit from a bigint.
 *
 * This function retrieves the value of the specified bit (j) from the given 
 * bigint. The indexing is 0-based, where the least significant bit is at index 0.
 *
 * @param[in] x Pointer to the bigint structure from which the bit will be retrieved (input).
 * @param[in] j The index of the bit to retrieve (0-based) (input).
 * @return Returns 1 if the bit is set, 0 if the bit is not set, or -1 if 
 *         the input is invalid (e.g., NULL pointer, out of bounds).
 */
int get_jth_bit(IN bigint* x, IN word j);


/**
 * @brief Perform a right shift operation on a bigint.
 *
 * This function shifts the bits of the given bigint to the right by a specified number
 * of positions. It reallocates memory for the underlying array to accommodate the shift
 * and updates the word length accordingly. If the shift results in fewer than one word,
 * the function ensures that at least one word remains.
 *
 * @param[in,out] x Pointer to the bigint to be shifted (input/output).
 * @param[in] shift The number of bits to shift to the right (input).
 *
 * @note The function handles memory allocation and will exit the program with an error
 *       message if memory allocation fails. It also removes any leading zero words 
 *       after the shift operation.
 */
void right_shift(INOUT bigint* x, IN int shift);


/**
 * @brief Perform a left shift operation on a bigint.
 *
 * This function shifts the bits of the given bigint to the left by a specified number
 * of positions. It reallocates memory for the underlying array to accommodate the shift
 * and updates the word length accordingly.
 *
 * @param[inout] x Pointer to the bigint to be shifted.
 * @param[in] shift The number of bits to shift to the left.
 *
 * @note The function handles memory allocation and will exit the program with an error
 *       message if memory allocation fails. It also removes any leading zero words 
 *       after the shift operation.
 */
void left_shift(INOUT bigint* x, IN int shift);

/**
 * @brief Perform a reduction operation on a bigint.
 *
 * This function reduces the given bigint A by shifting its bits to the right by
 * a specified number of positions r. The result is stored in the provided
 * bigint result. If r is greater than or equal to the total bit length of A,
 * the result will be zero. The sign of the result will match the sign of A.
 *
 * @param[in] x Pointer to the bigint to be reduced.
 * @param[in] r The number of bits to shift to the right.
 * @param[out] result Pointer to the bigint where the result will be stored.
 *
 * @note Memory for the result bigint is allocated inside this function. 
 *       The caller is responsible for freeing the memory used by result.
 */
void reduction(IN bigint* x, IN int r, OUT bigint* result);

#endif
