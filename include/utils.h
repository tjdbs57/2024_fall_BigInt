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
 * This function prints the contents of a bigint structure in hexadecimal format.
 * It checks for null pointers and handles error messages accordingly.
 *
 * @param x A pointer to the bigint structure to be displayed. 
 *          It must not be NULL and its internal array must be allocated.
 * 
 * @note If the bigint is negative, it will display a '-' sign before the hexadecimal values.
 *       If the bigint is invalid (e.g., uninitialized or empty), an error message will be printed.
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
 * @brief Calculate the bit length of a given bigint.
 *
 * This function calculates the bit length of the absolute value of a bigint.
 * It accounts for the number of leading zeros in the last word and adds an 
 * extra bit for the sign if the bigint is negative.
 *
 * @param num A pointer to the bigint structure.
 * @return The bit length of the bigint. If there are no words, returns 0.
 *         If the bigint is negative, an extra bit for the sign is included.
 */
int get_bit_length(bigint* num);


/**
 * @brief Retrieves the word length of the bigint.
 *
 * This function returns the number of words that represent the given bigint.
 *
 * @param x Pointer to the bigint whose word length is to be retrieved. It must not be NULL.
 * @return The word length of the bigint.
 *
 * @note The function checks if the bigint pointer is valid before accessing its word length.
 */
int get_word_length(bigint* x);

/**
 * @brief Get the value of the j-th bit from a bigint.
 *
 * This function retrieves the value of the specified bit (j) from the given 
 * bigint. The indexing is 0-based, where the least significant bit is at index 0.
 *
 * @param x Pointer to the bigint structure from which the bit will be retrieved.
 * @param j The index of the bit to retrieve (0-based).
 * @return Returns 1 if the bit is set, 0 if the bit is not set, or -1 if 
 *         the input is invalid (e.g., NULL pointer, out of bounds).
 */
int get_jth_bit(bigint* x, u32 j);


/**
 * @brief Retrieves the sign of the bigint.
 *
 * This function returns the sign of the given bigint. The sign indicates whether the
 * bigint is positive or negative. A value of 0 indicates a non-negative bigint, while
 * a value of -1 indicates a negative bigint.
 *
 * @param x Pointer to the bigint from which the sign is to be retrieved. It must not be NULL.
 *
 * @return The sign of the bigint. Returns 0 for non-negative and -1 for negative.
 *
 * @note The function checks if the bigint pointer is valid before accessing the sign field.
 */
int get_sign_bit(bigint* x);


/**
 * @brief Flips the sign of the bigint.
 *
 * This function toggles the sign of the given bigint. If the current sign is
 * negative, it changes it to non-negative (0). If the current sign is non-negative,
 * it changes it to negative (-1).
 *
 * @param x Pointer to the bigint whose sign is to be flipped. It must not be NULL.
 *
 * @note The function checks if the bigint pointer is valid before attempting to flip the sign.
 */
void flip_sign_bit(bigint* x);


#define CHECK_MEM_ALLOCATION(x)                                      \
    if ((x) == NULL) {                                               \
        printf("Memory allocation failed.\n");                       \
        exit(1);                                                     \
    }

#define CHECK_SET_STRING(str, base)                                  \
    if ((str) == NULL || (base) < 2 || (base) > 16) {              \
        printf("Error: Invalid input for string or base!\n");        \
        exit(1);                                                     \
    }

#define CHECK_SET_ARRAY(a, wordlen)                                  \
    if ((a) == NULL || (wordlen) <= 0) {                             \
        printf("set array fail!\n");                                 \
        exit(1);                                                     \
    }


/**
 * @brief Perform a right shift operation on a bigint.
 *
 * This function shifts the bits of the given bigint to the right by a specified number
 * of positions. It reallocates memory for the underlying array to accommodate the shift
 * and updates the word length accordingly. If the shift results in fewer than one word,
 * the function ensures that at least one word remains.
 *
 * @param a Pointer to the bigint to be shifted.
 * @param shift The number of bits to shift to the right.
 *
 * @note The function handles memory allocation and will exit the program with an error
 *       message if memory allocation fails. It also removes any leading zero words 
 *       after the shift operation.
 */
void right_shift(bigint* a, int shift);


/**
 * @brief Perform a left shift operation on a bigint.
 *
 * This function shifts the bits of the given bigint to the left by a specified number
 * of positions. It reallocates memory for the underlying array to accommodate the shift
 * and updates the word length accordingly.
 *
 * @param a Pointer to the bigint to be shifted.
 * @param shift The number of bits to shift to the left.
 *
 * @note The function handles memory allocation and will exit the program with an error
 *       message if memory allocation fails. It also removes any leading zero words 
 *       after the shift operation.
 */
void left_shift(bigint* a, int shift);

/**
 * @brief Perform a reduction operation on a bigint.
 *
 * This function reduces the given bigint A by shifting its bits to the right by
 * a specified number of positions r. The result is stored in the provided
 * bigint result. If r is greater than or equal to the total bit length of A,
 * the result will be zero. The sign of the result will match the sign of A.
 *
 * @param A Pointer to the bigint to be reduced.
 * @param r The number of bits to shift to the right.
 * @param result Pointer to the bigint where the result will be stored.
 *
 * @note Memory for the result bigint is allocated inside this function. 
 *       The caller is responsible for freeing the memory used by result.
 */
void reduction(bigint* A, int r, bigint* result);