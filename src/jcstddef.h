// LINT_C_FILE
/* This is a standard library implementation for self-use purposes. */
#pragma once
#ifndef _JCSTDDEF_H_
#define _JCSTDDEF_H_

#include <assert.h>

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

#if __STDC_VERSION__ < 202311L
#define nullptr (void *)0 // nullptr keyword coming in C23, for older versions use a macro.
#define constexpr static const
#endif

static_assert(!nullptr);

#ifdef JCLIBC
typedef unsigned size_t;
typedef unsigned uintptr_t;
typedef signed intptr_t;
typedef signed ptrdiff_t;
#endif

#endif // _JCSTDDEF_H_

/* LICENSE INFORMATION */
/* TBD */

