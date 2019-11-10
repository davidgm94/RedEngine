#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#define MEASURE_PERFORMANCE 1

#ifndef __cplusplus
#define bool	_Bool
#define false	0
#define true	1
#endif

    typedef signed char        i8;
    typedef short              i16;
    typedef int                i32;
    typedef long long          i64;
    typedef unsigned char      u8;
    typedef unsigned short     u16;
    typedef unsigned int       u32;
    typedef unsigned long long u64;

	//typedef u64 size_t;
    typedef i64 intptr;
    typedef u64 uintptr;
    typedef unsigned int uint;
	
//#define INT8_MIN         (-127i8 - 1)
//#define INT16_MIN        (-32767i16 - 1)
//#define INT32_MIN        (-2147483647 - 1)
//#define INT64_MIN        (-9223372036854775807 - 1)
//#define INT8_MAX         127i8
//#define INT16_MAX        32767i16
//#define INT32_MAX        2147483647
//#define INT64_MAX        9223372036854775807
//#define UINT8_MAX        0xff
//#define UINT16_MAX       0xffff
//#define UINT32_MAX       0xffffffff
//#define UINT64_MAX       0xffffffffffffffff

//#define INTPTR_MIN   INT64_MIN
//#define INTPTR_MAX   INT64_MAX
//#define UINTPTR_MAX  UINT64_MAX

//#define INTMAX_MIN       INT64_MIN
//#define INTMAX_MAX       INT64_MAX
//#define UINTMAX_MAX      UINT64_MAX

//#define PTRDIFF_MIN      INTPTR_MIN
//#define PTRDIFF_MAX      INTPTR_MAX

#ifndef SIZE_MAX
#define SIZE_MAX     UINTPTR_MAX
#endif

//#define SIG_ATOMIC_MIN   INT32_MIN
//#define SIG_ATOMIC_MAX   INT32_MAX

//#define WCHAR_MIN        0x0000
//#define WCHAR_MAX        0xffff

//#define WINT_MIN         0x0000
//#define WINT_MAX         0xffff

//#define DBL_DECIMAL_DIG  17                      // # of decimal digits of rounding precision
//#define DBL_DIG          15                      // # of decimal digits of precision
//#define DBL_EPSILON      2.2204460492503131e-016 // smallest such that 1.0+DBL_EPSILON != 1.0
//#define DBL_HAS_SUBNORM  1                       // type does support subnormal numbers
//#define DBL_MANT_DIG     53                      // # of bits in mantissa
//#define DBL_MAX          1.7976931348623158e+308 // max value
//#define DBL_MAX_10_EXP   308                     // max decimal exponent
//#define DBL_MAX_EXP      1024                    // max binary exponent
//#define DBL_MIN          2.2250738585072014e-308 // min positive value
//#define DBL_MIN_10_EXP   (-307)                  // min decimal exponent
//#define DBL_MIN_EXP      (-1021)                 // min binary exponent

//#define FLT_DECIMAL_DIG  9                       // # of decimal digits of rounding precision
//#define FLT_DIG          6                       // # of decimal digits of precision
//#define FLT_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0
//#define FLT_HAS_SUBNORM  1                       // type does support subnormal numbers
//#define FLT_GUARD        0
//#define FLT_MANT_DIG     24                      // # of bits in mantissa
//#define FLT_MAX          3.402823466e+38F        // max value
//#define FLT_MAX_10_EXP   38                      // max decimal exponent
//#define FLT_MAX_EXP      128                     // max binary exponent
//#define FLT_MIN          1.175494351e-38F        // min normalized positive value
//#define FLT_MIN_10_EXP   (-37)                   // min decimal exponent
//#define FLT_MIN_EXP      (-125)                  // min binary exponent
//#define FLT_NORMALIZE    0
//#define FLT_RADIX        2                       // exponent radix
//#define FLT_TRUE_MIN     1.401298464e-45F        // min positive value

    /**********************************************/

    // Memory
#define BYTE_SIZE 1Ui64
#define KILOBYTE (1024 * BYTE_SIZE)
#define MEGABYTE (1024 * KILOBYTE)
#define GIGABYTE (1024 * MEGABYTE)

// ******************
#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))
// ******************

#ifndef __cplusplus
#define nullptr NULL
#endif
#define null nullptr

#ifdef __cplusplus
}
#endif
