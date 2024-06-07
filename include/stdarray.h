#pragma once
#ifndef __STDARRAY_H
#define __STDARRAY_H

#define __need_size_t
#include <jclibc/stddef.h>

bool std_array_contains(
	size_t __array_nmemb,
	const unsigned char __array[static const __array_nmemb],
	size_t __element_nmemb,
	const unsigned char __element[static const __element_nmemb]
);

#endif
