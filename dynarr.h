//LINT_C_FILE
#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

/* 
 * Requires a typedeffed dynamic array.
 * The array must be of type:
 * typedef struct da {
 *    size_t count;
 *    size_t capacity;
 *    T *items;
 * };
 * Macro provided as convenience
 * */

#define DA_NEW(T) typedef struct _da_##T { size_t count; size_t capacity; T *items; } da_##T
#define DA_ALLOC(da) do { if ((da)->capacity == 0) { (da)->capacity = DA_INIT_CAP; } realloc((da)->items, (da)->capacity * sizeof ((da)->items[0])); }
#define DA_FREE(da) free((da).items)

// This idea of a dynamic array macro is taken from Tsoding / Rexim.
// Very based idea with minimal changes.
#define DA_INIT_CAP 256

#ifdef DA_MACRO_IMPLEMENTATION
#define da_append(da, n, new_items) do { \
	assert((n) > 0); \
	if ((da)->capacity <= (da)->count + n) { \
		if ((da)->capacity == 0) { (da)->capacity = DA_INIT_CAP; } \
		while ((da)->capacity < (da)->count + n) { (da)->capacity *= 2; } \
		(da)->items = reallocarray((da)->items, (da)->capacity, sizeof ((da)->items[0])); \
	} \
	memcpy(&(da)->items[(da)->count], (new_items), n * sizeof ((da)->items[0])); \
	(da)->count += n; \
} while (0)
#endif // DA_MACRO_IMPLEMENTATION

#ifdef DA_FUNCTION_IMPLEMENTATION
#include <assert.h>
#include <stdlib.h>
#include <string.h>
void da_append(void *restrict da_ptr, const size_t n, const size_t item_size, void *restrict new_items) {
    assert(n > 0 && item_size > 0 && "Sizes must be above 0\n");
    struct {size_t count; size_t capacity; void *items; } *da = da_ptr;
    if (da->capacity <= da->count + n) {
        if (da->capacity == 0) { da->capacity = DA_INIT_CAP; }
        while (da->capacity < da->count + n) { da->capacity *= 2; }
        da->items = realloc(da->items, da->capacity * item_size);
    }
    memcpy((unsigned char *)da->items + (da->count * item_size), new_items, n * item_size);
    da->count += n;
}

void da_put(void *restrict da_ptr, const size_t idx, const size_t item_size, void *restrict item) {
	assert(item_size > 0 && "Size must be greater than 0\n");
	struct {size_t count; size_t capacity; void *items; } *da = da_ptr;
	if (da->capacity <= da->count + 1) {
		da->capacity = da->capacity == 0 ? DA_INIT_CAP : da->capacity * 2;
		da->items = realloc(da->items, da->capacity * item_size);
	}
	memcpy((unsigned char *)da->items + (idx * item_size), item, item_size);
	da->count++;
}
#endif // DA_FUNCTION_IMPLEMENTATION

#endif // _DYNAMIC_ARRAY_H_
