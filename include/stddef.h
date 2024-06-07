#pragma once
#ifndef __STDDEF_H
#define __STDDEF_H


#if __STDC_VERSION__ <= 201710L
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#if __STDC_VERSION__ > 199901L
	typedef _Bool bool;
#else
#ifndef __bool_true_false_are_defined
	enum bool { false = 0, true = !false };
#define __bool_true_false_are_defined
#endif
#endif

#ifndef __nullptr_is_defined
typedef void*const nullptr_t;
#define nullptr (nullptr_t)0
#define __nullptr_is_defined
#endif

#else
constexpr int EXIT_SUCCESS = 0;
constexpr int EXIT_FAILURE = 1;

#endif

#ifdef __need_size_t
typedef __SIZE_TYPE__ size_t;
static_assert(sizeof (size_t) == sizeof (void *));
#undef __need_size_t
#endif

#ifdef __need_ptrdiff_t
typedef __PTRDIFF_TYPE__ ptrdiff_t;
static_assert(sizeof (ptrdiff_t) == sizeof (void *));
#undef __need_ptrdiff_t
#endif

typedef unsigned long long uintptr_t;
typedef signed long long intptr_t;
static_assert(sizeof (uintptr_t) == sizeof (void *));
static_assert(sizeof (intptr_t) == sizeof (void *));



#endif /* __STDDEF_H */

/* LICENSE INFORMATION */
/* TBD */

