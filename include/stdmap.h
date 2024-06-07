#pragma once
#ifndef __JCLIBC_HASHMAP_H
#define __JCLIBC_HASHMAP_H

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#if __STDC_VERSION__ < 202311L
#include "./jcstddef.h"
#endif

typedef unsigned (hash_func)(const void*, const void*);
typedef unsigned (cmp_func)(const void*, const void*);

typedef struct _jchash_item hash_item;
struct _jchash_item {
	void *key;
	void *item;
};

typedef struct _jchashmapc map;
struct _jchashmapc {
	size_t count;
	size_t capactity;
	hash_item **items;
	hash_func *hash;
	cmp_func *cmp;
};
static_assert(sizeof (map) == 40);

map map_new();
void map_delete(map *m);

void map_clear(map *m);
size_t map_count(const map *m);

const void *map_get(map *m, const void *item);
const void *map_set(map *m, const void *item);
const void *map_delete(map *m, const void *item);
const void *map_probe(map *m, uint64_t position);

// HASH FUNCS

// Stolen from: https://theartincode.stanis.me/008-djb2/
// Written by Daniel J. Bernstein (DJB) in 1991
static unsigned djb2_hash(const unsigned char *str) {
	unsigned int hash = 5381;
	for (unsigned char c = *str++; *str; c = *str++) {
		hash = ((hash << 5) + hash) + c; // Hash * 33 + c;
	}
	return hash;
}

static unsigned sbdm_hash(const unsigned char *str) {
	unsigned int hash = 0;
	for (unsigned char c = *str++; *str; c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

#endif
