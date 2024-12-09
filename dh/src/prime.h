/**
 * @file prime.c
 * @brief Implementation of functions for handling prime numbers using the Miller-Rabin test.
 */
#ifndef PRIME_H  
#define PRIME_H

#include "bigint.h"
#include "arithmetic.h"
#include "utils.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


/**
 * @brief Generates a random bigint within a given range.
 *
 * Generates a random bigint value between a given lower bound and an upper bound (exclusive).
 * 
 * @param b Pointer to store the resulting random bigint.
 * @param lower_bound String representation of the lower bound in hexadecimal format.
 * @param upper_bound Upper bound as a bigint.
 */
void bi_set_random_in_range(bigint **b, char *lower_bound, bigint *upper_bound);

/**
 * @brief Miller-Rabin primality test for a single base.
 *
 * Performs a single round of the Miller-Rabin test for a candidate prime number.
 * 
 * @param n Candidate number to test for primality.
 * @param b Random base for the test.
 * @param s The number of divisions by 2 in n-1 (used in the test).
 * @param t The odd factor of n-1.
 * @return 1 if \(n\) is likely prime; 0 if \(n\) is composite.
 */
int miller_rabin_test(bigint *n, bigint *b, int s, bigint *t);

/**
 * @brief Checks if a bigint is even.
 *
 * Determines whether the given bigint is an even number by inspecting the least significant bit.
 * 
 * @param x Pointer to the bigint to check.
 * @return 1 if the number is even; 0 otherwise.
 */
int is_even(const bigint *x);

/**
 * @brief Determines whether a bigint is prime using the Miller-Rabin test.
 *
 * Uses the Miller-Rabin primality test to determine if a given bigint is prime.
 * 
 * @param n Candidate number to test.
 * @param k Number of Miller-Rabin iterations to perform.
 * @return 1 if \(n\) is likely prime; 0 if \(n\) is composite.
 */
int is_prime(bigint *n, int k);

/**
 * @brief Generates a large prime number.
 *
 * Continuously generates random candidates and tests them for primality until a prime is found.
 * 
 * @param prime Pointer to store the resulting prime bigint.
 * @param bit_length Desired bit length of the prime number.
 * @param k Number of Miller-Rabin iterations for primality testing.
 */
void generate_large_prime(bigint **prime, int bit_length, int k);

#endif 