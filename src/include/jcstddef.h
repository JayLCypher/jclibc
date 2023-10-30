// LINT_C_FILE
/* This is a standard library implementation for self-use purposes. */

#ifndef _JCSTDDEF_H
#define _JCSTDDEF_H

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

// nullptr keyword coming in C23, for older versions use a macro.
#if __STDC_VERSION__ < 202311L
	#define nullptr (void *)0
	#define bool _Bool
	#define true !0
	#define false 0
	#define constexpr static const
#endif


typedef unsigned int uint; // Feature from D lang

#endif // _JCSTDDEF_H

/* LICENSE INFORMATION */
/* TBD */

