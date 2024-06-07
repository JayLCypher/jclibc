#pragma once
#ifndef __STDINT_H
#define __STDINT_H

/* SIGNED */
#ifdef __need_signed
typedef signed_char s8;
typedef signed short s16;
typedef signed s32;
typedef signed long long s64;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int32_t;
typedef signed long long int64_t;

#if __need_max
#define __MAX_TYPE__
constexpr int INT_MAX = 0x7FFFFFFF;
#unedf __need_max
#endif

#if __need_min
#define __MIN_TYPE__
constexpr int INT_MIN = ~0x7FFFFFFF + 1;
#undef __need_min
#endif

#undef __need_signed
#endif

/* UNSIGNED */
#ifdef __need_unsigned
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned u32;
typedef unsigned long long u64;

#undef __need_unsigned
#endif

/* FLOATS */
#ifdef __need_float
typedef float f32;
typedef double f64;
#if defined(__x86_64__)
typedef long double f128;
#elif defined(__i386__)
typedef long double f64_t;
#endif

#undef __need_float
#endif /* __need_float */

#endif /* __STDINT_H */
