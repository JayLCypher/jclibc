
#include "./include/jchashmap.h"

struct hashmap {
	size_t max;
	size_t cur;
	void **buckets;
};

static map *map_init(map * const m) {
	if (m) { *m = (map){0}; }
	return m;
}

map map_new() {

}

