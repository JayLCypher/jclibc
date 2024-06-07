//LINT_C_FILE
#include <stdlib.h>
#include "./jchashmapc.h"



// HASH_MAP
static map map_grow(map *m) {
	m->buckets = realloc(m->buckets, sizeof (hash_item) * m->capactity * m->capactity);
}

static map *map_init(map *m) {
	if (m) { *m = (map){0}; }
	return m;
}

map map_new() { return *map_init(&(map){0}); }

void map_insert(map *m, hash_item item) {
	if (m->count + 1 >= m->capactity) { map_grow(m); }
}



int main(void) {
	const char file_path[] = "hashmap_test.txt";

	return 0;
}
