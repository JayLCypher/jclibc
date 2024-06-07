
#define __need_size_t
#include <jclibc/stddef.h>
#include <jclibc/stdmem.h>

static const void *mem_chr_internal_naive(const void *const m, const char c, const size_t n) {
	const char *p = m;
	for (size_t i = 0; i < n; ++i) { if (p[i] == c) { return &p[i]; } }
	return nullptr;
}

// Scan N bytes of memory back to front for last instance of C.
static const void *mem_rchr_internal_naive(const void *const m, const char c, const size_t n) {
	const char *p = m;
	for (size_t i = n; i > 0; --i) { if (p[i] == c) { return &p[i]; } }
	return nullptr;
}

// Scans N bytes of memory back to front using pointer range for C.
static const void *mem_rchrr_internal_naive(const void *const m_start, const char c, const void *const m_end) {
	const char *p = m_end;
	while (p-- != m_start) { if (*p == c) { return p; } }
	return nullptr;
}

// Assume user knows that C exist in memory range, optimizes search for first instance of C.
static const void *mem_rawchr_internal_naive(const void *m, const char c) {
	const char *p = m;
	while (*p++) { if (*p == c) { return p; } }
	return nullptr;
}

static const void *mem_rawchr_internal_opt1(const void *m, const char c) {
	const unsigned long long *p = m;
	while (*p++) {
		if ( ( ( (*p) >> 56) & 0xFF) == c ) { return (unsigned char *)(p + (sizeof (unsigned char) * 7)); }
		if ( ( ( (*p) >> 48) & 0xFF) == c ) { return (unsigned char *)(p + (sizeof (unsigned char) * 6)); }
		if ( ( ( (*p) >> 40) & 0xFF) == c ) { return (unsigned char *)(p + (sizeof (unsigned char) * 5)); }
		if ( ( ( (*p) >> 32) & 0xFF) == c ) { return (unsigned char *)(p + (sizeof (unsigned char) * 4)); }
		if ( ( ( (*p) >> 24) & 0xFF) == c ) { return (unsigned char *)(p + (sizeof (unsigned char) * 3)); }
		if ( ( ( (*p) >> 16) & 0xFF) == c ) { return (unsigned char *)(p + (sizeof (unsigned char) * 2)); }
		if ( ( ( (*p) >> 8)  & 0xFF) == c ) { return (unsigned char *)(p + (sizeof (unsigned char) * 1)); }
		if ( ( ( (*p) )      & 0xFF) == c ) { return (unsigned char *)(p); }
	}
	return nullptr;
}

/* LIBRARY EXPOSED FUNCTIONS */

// Scan N bytes of memory for first instance of C.
const void *mem_chr(const void *const m, const char c, const size_t n) {
	return mem_chr_internal_naive(m, c, n);
}

// Scan N bytes of memory back to front for last instance of C.
const void *mem_rchr(const void *const m, const char c, const size_t n) {
	return mem_rchr_internal_naive(m, c, n);
}

// Scans N bytes of memory back to front using pointer range for C.
const void *mem_rchrr(const void *m_start, const char c, const void *m_end) {
	return mem_rchrr_internal_naive(m_start, c, m_end);
}

// Assume user knows that C exist in memory range, optimizes search for first instance of C.
const void *mem_rawchr(const void *const m, const char c) {
	return mem_rawchr_internal_naive(m, c);
}

#ifdef F_MEMORY_DEBUG
#define malloc(n) f_debug_mem_malloc(n, __FILE__, __LINE__)
#define realloc(n, m) f_debug_mem_realloc(n, m, __FILE__, __LINE__)
#define free(n) f_debug_mem_free(n, __FILE__, __LINE__)
#endif // F_MEMORY_DEBUG
