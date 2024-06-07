
#include <jclibc/stdarray.h>
#include <jclibc/stdmem.h>

bool std_array_contains(
	size_t __array_nmemb,
	const unsigned char __array[static const __array_nmemb],
	size_t __element_nmemb,
	const unsigned char __element[static const __element_nmemb]
)
{
	for (size_t i = 0; i < __array_nmemb; ++i) {
		if (std_mem_cmp(&__array[i], __element, __element_nmemb) == 0) { return true; }
	}
	return false;
}

