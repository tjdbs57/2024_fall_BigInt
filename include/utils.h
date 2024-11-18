#ifndef UTILS_H
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
 * @brief Refines a bigint by removing leading zeros and adjusting its length.
 *
 * This function modifies the given bigint by removing any leading zeros,
 * updating its word length accordingly, and ensuring that the sign is set
 * correctly if the bigint becomes zero.
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
void right_shift_bit(INOUT bigint* x, IN int shift);


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
void left_shift_bit(INOUT bigint* x, IN int shift);

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


/**
 * @brief Check if the given big integer is zero.
 * 
 * This function checks whether a given big integer (represented by the `bigint` structure)
 * is zero by iterating through its words and checking each bit. If any bit is non-zero,
 * the function returns 0, indicating that the number is not zero. If all bits are zero,
 * the function returns 1, indicating that the number is zero.
 * 
 * @param x A pointer to a `bigint` structure representing the big integer to check.
 *          The `bigint` structure contains an array of words (`x->a[]`) and the length of the number in words (`x->wordlen`).
 * 
 * @return 1 if the big integer is zero, otherwise 0 if it is non-zero.
 */
int is_zero(IN bigint* x);

/**
 * @brief Performs a left shift operation on a `bigint` by a specified number of words.
 * @details This function shifts the words in a `bigint` object `x` to the left by `shift_words` positions, 
 *          effectively appending zeros to the lower-order positions. The function reallocates memory 
 *          to accommodate the new size and ensures the integrity of the `bigint` structure.
 * @param[in, out] x A double pointer to the `bigint` to be shifted. 
 *                   The structure will be updated to reflect the shift.
 * @param[in] shift_words The number of word positions to shift. Must be a non-negative integer.
 * @pre `x` must point to a valid, initialized `bigint` object.
 *      `shift_words` must be non-negative.
 * @post The `bigint` pointed to by `x` is updated:
 *       - Its words are shifted to the left by `shift_words` positions.
 *       - Newly created lower-order positions are initialized to zero.
 *       - The `wordlen` of the `bigint` is increased by `shift_words`.
 * @note 
 * - If `shift_words` is negative, an error message is printed to `stderr`, and the function returns without performing any operation.
 * - Memory for the `bigint` is reallocated using `realloc`. If allocation fails, the program exits with an error.
 * - The function assumes that the `bigint` structure has attributes `a` (an array of words) and `wordlen` (length of the array).
 * @warning This function modifies the memory of `x` directly and may reallocate it. 
 *          Ensure no other references are holding the original pointer before calling this function.
 */
void left_shift_word(INOUT bigint** x, IN int shift_words);

/**
 * @brief Swaps the values of two `bigint` pointers.
 * @details Exchanges the pointers of two `bigint` objects `x` and `y`. 
 *          This effectively swaps the two `bigint` structures without copying their contents.
 * @param[in, out] x A double pointer to the first `bigint` object.
 * @param[in, out] y A double pointer to the second `bigint` object.
 * @pre `x` and `y` must be valid, initialized double pointers to `bigint` objects.
 * @post The values of `*x` and `*y` are swapped. The `bigint` objects they point to are unchanged, 
 *       but the pointers themselves are exchanged.
 * @note This function operates only on the pointers and does not modify the actual contents of the `bigint` objects.
 */
void swap_bigint(INOUT bigint** x, INOUT bigint** y);

/**
 * @brief Refines the word length of a `bigint` by removing a specified number of words.
 * @details This function adjusts the word length of a `bigint` object `x` by reducing it by `num_words`. 
 *          It reallocates memory if necessary and ensures the integrity of the `bigint` structure.
 *          If the result represents zero (single word with value `0`), the sign is reset to `NON_NEGATIVE`.
 * @param[in] x A pointer to the `bigint` object to be refined.
 * @param[in] num_words The number of words to remove from the end of the `bigint`.
 * @pre `x` must be a valid pointer to an initialized `bigint` object.
 *      `num_words` should not exceed the current word length of `x`.
 * @post The `bigint`'s word length is reduced by `num_words`, and its memory is resized accordingly.
 *       If the resulting `bigint` represents zero, its sign is reset to `NON_NEGATIVE`.
 * @note
 * - If `x` is `NULL`, the function prints an error message (`INVALID_DATA`) and returns without performing any operation.
 * - The function assumes that the `bigint` structure has attributes `a` (an array of words), `wordlen` (length of the array), 
 *   and `sign` (the sign of the integer).
 * - Memory reallocation is handled using `realloc`. Ensure proper error handling for memory operations if extended.
 * @warning 
 * - If `num_words` is greater than or equal to `x->wordlen`, the behavior may be undefined.
 * - Always ensure that the `bigint` object `x` is valid before calling this function.
 */
void bi_refine_word(IN bigint* x, IN int num_words) ;

/**
 * @brief Ensures that the word length of a `bigint` is even.
 * @details This function checks if the word length of the given `bigint` object `x` is odd. 
 *          If it is, the function increases the word length by one, reallocates memory to 
 *          accommodate the new size, and initializes the newly added word to zero.
 * @param[in, out] x A pointer to the `bigint` object to be adjusted. 
 *                   The structure is modified to ensure an even word length.
 * @pre `x` must be a valid pointer to an initialized `bigint` object.
 * @post If the original word length of `x` is odd:
 *       - The word length is increased by one.
 *       - Memory is reallocated to match the new size.
 *       - The new higher-order word is initialized to zero.
 * @note
 * - If the word length of `x` is already even, no changes are made.
 * - The function assumes that the `bigint` structure has attributes `a` (an array of words) and `wordlen` (length of the array).
 * - Memory reallocation is handled using `realloc`. Ensure error handling for memory failures.
 * @warning 
 * - If memory allocation fails during `realloc`, the program prints an error message (`MEM_ALLOCATION_FAIL`) and exits.
 * - This function modifies the memory directly, so ensure there are no conflicting references to the same `bigint`.
 */
void makeEven(INOUT bigint* x) ;

/**
 * @brief Matches the word lengths of two `bigint` objects by resizing them to the larger of their current lengths.
 * @details This function adjusts the word lengths of two `bigint` objects `x` and `y` so that both have the same word length. 
 *          If either `x` or `y` has a smaller word length than the maximum of the two, it reallocates memory and initializes 
 *          the newly added words to zero.
 * @param[in, out] x A pointer to the first `bigint` object. 
 *                   Its word length and memory may be modified to match the other `bigint`.
 * @param[in, out] y A pointer to the second `bigint` object. 
 *                   Its word length and memory may be modified to match the other `bigint`.
 * @pre Both `x` and `y` must point to valid, initialized `bigint` objects.
 * @post 
 * - The word lengths of `x` and `y` are equal and match the larger of their original word lengths.
 * - Memory is reallocated for either `x` or `y` (or both) if necessary.
 * - Any newly added words are initialized to zero.
 * @note
 * - The function assumes that the `bigint` structure has attributes `a` (an array of words) and `wordlen` (length of the array).
 * - Memory reallocation is performed using `realloc`. Proper error handling is in place to terminate the program if allocation fails.
 * @warning 
 * - If memory allocation fails during `realloc`, the program prints an error message (`MEM_ALLOCATION_FAIL`) and exits.
 * - Ensure no conflicting references to the same `bigint` objects are active when calling this function.
 */
void match_wordlen(INOUT bigint* x, INOUT bigint* y);

/**
 * @brief Resets all words in a `bigint` to zero.
 * @details This function iterates through all words in the given `bigint` object `x` and sets each word to zero. 
 *          It does not alter the word length of the `bigint`, only the values of the words.
 * @param[in, out] x A pointer to the `bigint` object to be reset.
 * @pre `x` must be a valid pointer to an initialized `bigint` object.
 * @post All words in the `bigint` pointed to by `x` are set to zero, but the word length remains unchanged.
 * @note This function assumes that the `bigint` structure has an attribute `a` (an array of words) and `wordlen` (the number of words).
 */
void bi_reset(INOUT bigint* x);

#endif  //utils.h
