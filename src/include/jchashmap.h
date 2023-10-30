#pragma once
#ifndef __JCLIBC_HASHMAP_H
#define __JCLIBC_HASHMAP_H

#include <stddef.h>
#include <stdint.h>

#if __STDC_VERSION__ < 202311L
#include "./jcstddef.h"
#endif

typedef struct hashmap map;
struct hashmap {
	size_t count;
	size_t max;
	unsigned int (*hash_func)();
	void **buckets;
};

map map_create();
void map_destroy(map * const m);

void map_clear(map * const m);
size_t map_count(const map * const m);
void map_grow(map * const m, size_t power);

const void *map_get(map * const m, const void *item);
const void *map_set(map * const m, const void *item);
const void *map_delete(map * const m, const void *item);
const void *map_probe(map * const m, uint64_t position);


#endif
