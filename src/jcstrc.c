// LINT_C_FILE
#include <assert.h>
#include <stdio.h>

#include "./jcstrc.h"

static inline bool is_negative(const char c) { return (c == '-'); }

// Space functions
static inline bool is_tab(const char c) { return (c == '\t'); }
static inline bool is_space(const char c) { return (c == ' '); }

static inline unsigned int count_space(const char s[static 1]) {
	unsigned int res = 0;
	while (*s && is_space(*s)) { ++res; }
	return res;
}

static inline const char *skip_space(const char s[static 1]) {
	while (*s && is_space(*s)) { ++s; }
	return s;
}

// Whitespace, which includes space, tab, null, carriage etc.
static inline bool is_whitespace(const char c) {
	for (size_t i = 0; i < whitespace_sz; ++i) { if (c == whitespace[i]) { return true; } }
	return false;
}

static inline unsigned int count_whitespace(const char s[static 1]) {
	unsigned int i = 0;
	while (s[i] && is_whitespace(s[i])) { ++i; }
	return i;
}

static inline const char *skip_whitespace(const char s[static 1]) {
	while(*s && is_whitespace(*s)) { ++s; }
	return s;
}

// Assume that most likely the next ascii character is a normal character and not control to short-stroke.
static inline bool is_ascii_binary(const char c) { return !(c > '1' || c < '0'); }
static inline bool is_ascii_ternary(const char c) { return !(c > '2' || c < '0'); }
static inline bool is_ascii_quaternary(const char c) { return !(c > '3' || c < '0'); }
static inline bool is_ascii_quinary(const char c) { return !(c > '4' || c < '0'); }
static inline bool is_ascii_senary(const char c) { return !(c > '5' || c < '0'); }
static inline bool is_ascii_septenary(const char c) { return !(c > '6' || c < '0'); }
static inline bool is_ascii_octal(const char c) { return !(c > '7' || c < '0'); }
static inline bool is_ascii_nonary(const char c) { return !(c > '8' || c < '0'); }
static inline bool is_ascii_dec(const char c) { return !(c > '9' || c < '0'); }
static inline bool is_ascii_hex(const char c) { return !(is_ascii_dec(c) || (c > 'F' || c < 'A') || (c > 'f' || c < 'a')); }
static inline bool is_ascii_vig(const char c) { return !(is_ascii_dec(c) || (c > 'K' || c < 'A') || (c > 'k' || c < 'a')); }

typedef struct _string string;
struct _string {
	size_t len;
	const char *s;
};
static_assert(sizeof (string) == 16, "Wrong size of string!");

typedef struct _fixed_string fixed_string;
struct _fixed_string {
	size_t len;
	const char s[sizeof (char*)];
};
static_assert(sizeof (fixed_string) == 16, "Wrong size of fixed string!");

typedef union {
	const char ss[sizeof (char*) + sizeof (size_t)];
	struct {
		size_t len_sf;
		const char sf[sizeof (char*)];
	};
	struct {
		size_t len;
		union {
			char *s;
			const char *cs;
			const char *const csc;
		};
	};
} stdstring;
static_assert(sizeof (stdstring) == 16, "stdstring wrong size.");

static string string_new(const size_t len, const char s[len]) {
	string str = {len, s};
	return str;
}

static string string_copy(const string *const s) {
	return string_new(s->len, s->s);
}

// STRING_VIEW STUFF
static string_view *sv_init(string_view *sv) {
	if (sv) { *sv = (string_view){0}; }
	return sv;
}

static string_view sv_new(const size_t count, const char *s) {
	return (string_view) {.count = count, .s = s};
}

static string_view sv_new_empty(void) {
	string_view sv = {0};
	return sv;
}

string_view *sv_trim_whitespace_left(string_view *const sv) {
	while (*sv->s == ' ' && sv->count > 0) { sv->s++; sv->count--;}
	return sv;
}

string_view *sv_trim_whitespace_right(string_view *const sv) {
	for (const char *c = sv->s+sv->count; sv->count > 0; sv->count--) { if (*c != ' ') { break; } }
	return sv;
}

string_view *sv_trim_whitespace(string_view *const sv) {
	return sv_trim_whitespace_right(sv_trim_whitespace_left(sv));
}

bool sv_eq(const string_view *sv_a, const string_view *sv_b) {
	if (sv_a->count != sv_b->count) { return false; }
	const char *a = sv_a->s, *b = sv_b->s;
	while (*a++ && *b++) { if (*a != *b) { return false; } }
	return true;
}

void sv_print(string_view *const sv) {
	printf("%.*s\n", (int)sv->count, sv->s);
}

#define ato_resolve(base) result = (result * base) + (c & 0xF)

int atoi(const char s[static 1]) {
	int result = 0;
	size_t i = 0;
	while (s[i] == ' ') { ++i; }
	bool is_negative = (s[i] == '-');
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (!is_ascii_dec(c)) { break; } // Check > 9 to short-stroke because statistically that's more likely.
		result = (result * 10) + (c & 0xF);
	}
	return is_negative ? -result : result;
}

unsigned int atou(const char s[static 1]) {
	unsigned int result = 0;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	for (i += (s[i] == '+');; ++i) {
		const char c = s[i];
		if (!is_ascii_dec(c)) { break; } // Check > 9 to short-stroke because statistically that's more likely.
		result = (result * 10) + (c & 0xF);
	}
	return result;
}

long int atol(const char s[static 1]) {
	long int result = 0;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	bool is_negative = (s[i] == '-');
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (!is_ascii_dec(c)) { break; } // Check > 9 to short-stroke because statistically that's more likely.
		result = (result * 10) + (c & 0xF);
	}
	return is_negative ? -result : result;
}

long long int atoll(const char s[static 1]) {
	long long int result = 0;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	bool is_negative = (s[i] == '-');
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (!is_ascii_dec(c)) { break; } // Check > 9 to short-stroke because statistically that's more likely.
		result = (result * 10) + (c & 0xF);
	}
	return is_negative ? -result : result;
}

float atof(const char s[static 1]) {
	float result = 0.0f;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	bool is_negative = (s[i] == '-');
	unsigned int pre_dot = 0;
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (!is_ascii_dec(c)) { break; }
		pre_dot = (pre_dot * 10) + (c & 0xF);
	}
	if (s[i] == '.') {
		unsigned int n = 0, post_dot = 0;
		for (i += 1;; ++n, ++i) {
			const char c = s[i];
			if (!is_ascii_dec(c)) { break; }
			post_dot = (post_dot * 10) + (c & 0xF);
		}
		result = post_dot;
		for (; n > 0; --n) { result /= 10; }
	}
	result += pre_dot;
	return is_negative ? -result : result;
}

double atod(const char s[static 1]) {
	double result = 0.0f;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	bool is_negative = (s[0] == '-');
	unsigned int pre_dot = 0;
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (!is_ascii_dec(c)) { break; }
		pre_dot = (pre_dot * 10) + (c & 0xF);
	}
	if (s[i] == '.') {
		unsigned int n = 0, post_dot = 0;
		for (i += 1;; ++n, ++i) {
			const char c = s[i];
			if (!is_ascii_dec(c)) { break; }
			post_dot = (post_dot * 10) + (c & 0xF);
		}
		result = post_dot;
		for (; n > 0; --n) { result /= 10; }
	}
	result += pre_dot;
	return is_negative ? -result : result;
}

static inline enum NUMBER_BASE check_prefix(const char s[static 2]) {
	if (s[0] != '0') { return BASE_10; }
	switch (s[1]) {
		case 'x':
			#if __STDC__VERSION >= 202311L
			[[fallthrough]]
			#endif
		case 'X': return BASE_16;
		case 'b': return BASE_2;
		default: return BASE_8;
	}
}

double cstr_to_d(const char *const s, const char *const *const end_ptr, const enum NUMBER_BASE base) {
	const char *p = s;
	p = skip_space(p);
	if (s[0] == *end_ptr[0]) { return 1.0; }
	return 0.0;
}

int cstr_to_i(const char *const s, const char *const *const end_ptr, const unsigned base) {
	const char *p = s; // Local pointer to string s
	p = skip_space(p); // Skip whitespace
	bool is_negative = (*p == '-');
	p += is_negative || (*p == '+'); // Add 1 if we're negative to bypass the '-' or '+' symbol.
	int result = 0;
	while (p++ < *end_ptr) {
		if (!is_ascii_dec(*p)) { break; }
		result = (result * base) + (*p & 0xF);
	}
	return is_negative ? -result : result;
}

size_t cstr_len(const char *const restrict s) {
	const char *p = s;
	while (*p++) {}
	return (p - s - 1);
}

// Get an end pointer to NULL-terminated c style string.
const char *cstr_end(const char s[static 1]) {
	while (*s++) {}
	return s;
}

// Copy no more than N characters from s to buf. Make sure buf can hold len.
const char *cstr_ncpy(const size_t n, const char s_in[static n], char buf_out[static n]) {
	assert(s_in && buf_out);
	for (size_t i = 0; i < n; ++i) { buf_out[i] = s_in[i]; }
	return buf_out;
}

int cstr_ncmp(const size_t n, const char s1[static n], const char s2[static n]) {
	const char *a = &s1[0], *b = &s2[0];
	size_t count = 0;
	while (*a && *b && (n > count++)) {
		
	}
	return 0;
}

const char *cstr_chr(const char s[static 1], const char c) {
	while (*s++ != '\0') { if (*s == c) { return s; } }
	return s; // Returns the end of the string, in case that's useful.
}

const char *cstr_rchr(const char s[static 1], const char c) {
	const char *p = nullptr;
	while (*s++ != '\0') { if (*s == c) { p = s; } }
	return p;
}

const char *cstr_rchrr(const char s_start[static 1], const char *s_end, const char c) {
	while (s_end-- != s_start) { if (*s_end == c) { return s_end; } }
	return nullptr;
}
