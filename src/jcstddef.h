// LINT_C_FILE
/* This is a standard library implementation for self-use purposes. */
#pragma once
#ifndef _JCSTDDEF_H_
#define _JCSTDDEF_H_


/*
 * * * * * * * * * * * *
 * DECLARATION SECTION *
 * * * * * * * * * * * *
*/
#ifdef JCASSERTC
#ifdef NDEBUG
#define assert(expr) ((void)0)
#else
#define assert(expr) _Assert(expr)
#define static_assert(expr, message) _Static_assert(expr, message)
#endif
#else
#include <assert.h>
#endif

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#if __STDC_VERSION__ < 199901L
#define bool int
#define false 0
#define true !false
#else
#ifndef __bool_true_false_are_defined
#define bool _Bool
#define false 0
#define true !false
#define __bool_true_false_are_defined
#endif
#endif

static_assert(false == 0);
static_assert((5 == 5) == true);
static_assert(!!(5 == 5) == true);

#if __STDC_VERSION__ < 202311L
#define nullptr (void *)0 // nullptr keyword coming in C23, for older versions use a macro.
#define constexpr static const
static_assert(!nullptr);
#endif

#ifdef JCPTRC
typedef unsigned long long size_t;
typedef unsigned long long uintptr_t;
typedef signed long long intptr_t;
typedef signed long long ptrdiff_t;
static_assert(sizeof (size_t) == sizeof (void *));
static_assert(sizeof (uintptr_t) == sizeof (void *));
static_assert(sizeof (intptr_t) == sizeof (void *));
static_assert(sizeof (ptrdiff_t) == sizeof (void *));
#endif

#ifdef JCINTC
// Unsigned
typedef unsigned char uint8_t;
static_assert(sizeof (uint8_t) == 1 << 0);
typedef unsigned short uint16_t;
static_assert(sizeof (uint16_t) == 1 << 1);
typedef unsigned long uint32_t;
static_assert(sizeof (uint32_t) == 1 << 2);
typedef unsigned long long uint64_t;
static_assert(sizeof (uint64_t) == 1 << 3);

// Signed
typedef signed char int8_t;
static_assert(sizeof (int8_t) == 1 << 0);
typedef signed short int16_t;
static_assert(sizeof (int16_t) == 1 << 1);
typedef signed long int32_t;
static_assert(sizeof (int32_t) == 1 << 2);
typedef signed long long int64_t;
static_assert(sizeof (int64_t) == 1 << 3);
#endif


#ifdef JCFLOATC
typedef float f32_t;
static_assert(sizeof (f32_t) == 1 << 2);
typedef double f64_t;
static_assert(sizeof (f64_t) == 1 << 3);
#if defined(__x86_64__)
typedef long double f128_t;
static_assert(sizeof (f128_t) == 1 << 4);
#endif
#if defined(__i386__)
typedef long double f64_t;
static_assert(sizeof (f64_t) == 1 << 3);
#endif
#endif

/*
 * * * * * * * * * * * * * *
 * IMPLEMENTATION SECTIONS *
 * * * * * * * * * * * * * *
*/

#endif // _JCSTDDEF_H_

/* LICENSE INFORMATION */
/* TBD */

