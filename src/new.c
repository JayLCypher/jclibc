
#define JCPTR
#include "jcstddef.h"
#include "jcmemc.h"

typedef void *(*allocator)(const size_t);
typedef void *(*reallocator)(const void *, const size_t);
typedef void *(*reallocator_array)(const void *, const int, const size_t);
typedef void (*deallocator)(const void *);

#ifndef JCALLOCATOR
#define JCALLOCATOR
#define JC_ALLOC malloc
#define JC_REALLOC realloc
#define JC_REALLOCARRAY reallocarray
#define JC_FREE free
#endif

static constexpr allocator alloca = JC_ALLOC;
static constexpr reallocator realloca = JC_REALLOC;
static constexpr reallocator_array realloca_arr = JC_REALLOCARRAY;
static constexpr deallocator freea = JC_FREE;

void *new(const size_t size) {
	return alloca(size);
}

void delete(const void *ptr) {
	free(ptr);
}
