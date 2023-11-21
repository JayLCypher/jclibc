
#include "./jcmemc.h"
#include "./jcstddef.h"

// Scan N bytes of memory for first instance of C.
const void *mem_chr(const void *const m, const char c, const size_t n) {
	const char *p = m;
	for (size_t i = 0; i < n; ++i) { if (p[i] == c) { return &p[i]; } }
	return nullptr;
}

// Scan N bytes of memory back to front for last instance of C.
const void *mem_rchr(const void *const m, const char c, const size_t n) {
	const char *p = m;
	for (size_t i = n; i > 0; --i) { if (p[i] == c) { return &p[i]; } }
	return nullptr;
}

// Scans N bytes of memory back to front using pointer range for C.
const void *mem_rchrr(const void *const m_start, const void *const m_end, const char c) {
	const char *p = m_end;
	while (p-- != m_start) { if (*p == c) { return p; } }
	return nullptr;
}

// Assume user knows that C exist in memory range, optimizes search for first instance of C.
const void *mem_rawchr(const void *const m, const char c) {
	const char *p = m;
	while (*p++) { if (*p == c) { return p; } }
	return nullptr;
}


