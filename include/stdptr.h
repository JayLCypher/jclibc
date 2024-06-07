#pragma once
#ifndef __STDPTR_H
#define __STDPTR_H

#include <jclibc/stddef.h>

typedef struct __fatptr fatptr_t;
struct __fatptr {
	size_t __size;
	void *__ptr;
};



#endif /* __STDPTR_H */
