
#include "./include/jchashmap.h"

// Stolen from: https://theartincode.stanis.me/008-djb2/
// Written by Daniel J. Bernstein (DJB) in 1991
static unsigned int djb2_hash(const unsigned char *const str) {
	unsigned int hash = 5381;
	for (unsigned char c = *str++; *str; c = *str++) {
		hash = ((hash << 5) + hash) + c; // Hash * 33 + c;
	}
	return hash;
}

static unsigned int sbdm_hash(const unsigned char *const str) {
	unsigned int hash = 0;
	for (unsigned char c = *str++; *str; c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

static map *map_init(map * const m) {
	if (m) { *m = (map){0}; }
	return m;
}

map map_new() {
	return *map_init(&(map){0});
}


int main(void) {
	const char file_path[] = "hashmap_test.txt";
	return 0;
}
