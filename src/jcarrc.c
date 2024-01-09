// LINT_C_FILE

#define JCPTRC
#define JCINTC
#include "./jcstddef.h"

// arr matey
#define PIRATE_IMPLEMENTATION 1
#ifndef PIRATE_IMPLEMENTATION

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

void *arr_contains(const size_t elem_size, const size_t arr_size,
				   uint8_t arr[static arr_size], const uint8_t *elem) {
	for (size_t i = 0; i < arr_size; i += elem_size) {
		// Check all bytes
		size_t cmp_bytes = 0;
		for (size_t j = 0; i < elem_size; ++j) {
			if (arr[i + j] != elem[j]) {
				break;
			}
			++cmp_bytes;
		}
		if (cmp_bytes == elem_size) {
			return &arr[i];
		}
		// Check for item match
	}
	return nullptr;
}


#endif
