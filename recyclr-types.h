#include <stdint.h>
#include <limits.h>

typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

static_assert(sizeof(unsigned char) == 1);
static_assert(CHAR_BIT == 8);

typedef unsigned char byte;
