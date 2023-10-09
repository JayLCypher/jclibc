#pragma once
#ifndef __JCLIBC_HASHMAP_H
#define __JCLIBC_HASHMAP_H

#if __STDC_VERSION__ < 202311L
#include <stdbool.h>
#define nullptr (void *)0

#endif

#include <stddef.h>
#include <stdint.h>

struct hashmap;
typedef struct hashmap map;

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
