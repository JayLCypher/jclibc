
#ifdef F_MEMORY_DEBUG
#define malloc(n) f_debug_mem_malloc(n, __FILE__, __LINE__)
#define realloc(n, m) f_debug_mem_realloc(n, m, __FILE__, __LINE__)
#define free(n) f_debug_mem_free(n, __FILE__, __LINE__)
#endif // F_MEMORY_DEBUG
