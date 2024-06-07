#pragma once
#ifndef __STDMEM_H
#define __STDMEM_H

#define __need_size_t
#include <jclibc/stddef.h>

#ifdef __need_mem_funcptr
typedef void *(*std_alloc_func)(size_t);
typedef void *(*std_realloc_func)(const void*, size_t);
typedef void *(*std_reallocarr_func)(const void*, int, size_t);
typedef void (*std_free_func)(const void *);
#endif

void *std_mem_alloc(size_t);
void *std_mem_realloc(const void *, size_t);
void *std_mem_realloc_array(const void *, int, size_t);
void std_free(const void *);

void std_mem_set(void *, size_t);
int std_mem_cmp(const void *, const void *, size_t);
int std_mem_cmp_deep(const void *, const void *, size_t);

const void *std_mem_chr   (const void *, char, size_t);
const void *std_mem_rchr  (const void *, char, size_t);
const void *std_mem_rchrr (const void *, char, const void *);
const void *std_mem_rawchr(const void *, char);

#endif /* __STDMEM_H */
