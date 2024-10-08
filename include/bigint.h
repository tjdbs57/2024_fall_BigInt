#include "config.h"

<<<<<<< Updated upstream
=======

/* Return Maximum of x1 and x2. */
#define MAXIMUM(x1, x2) (x1 > x2 ? x1 : x2)

/* Return Minimum of x1 and x2. */
#define MINIMUM(x1, x2) (x1 < x2 ? x1 : x2)

#define WORD_BITLEN 32

// Conditional compilation based on the word size for data type flexibility
#if WORD_BITLEN == 8
typedef u8 word;
#define ONE (u8)1
#elif WORD_BITLEN == 32
typedef u32 word;
#define ONE (u32)1
#else
typedef u64 word;
#define ONE (u64)1
#endif

#define Invalid_Bigint "Invalid Bigint"
>>>>>>> Stashed changes
