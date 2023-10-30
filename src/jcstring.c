// LINT_C_FILE
#include <assert.h>
#include <stdio.h>
#include "include/jcstring.h"

static inline bool jc_is_space(const char c) { return (c == ' ') ? true : false; }

static inline bool jc_is_whitespace(const char c) {
	for (size_t i = 0; i < whitespace_sz; ++i) {
		if (c == whitespace[i]) { return true; }
	}
	return false;
}

// Assume that most likely the next ascii character is a normal character and not control to short-stroke.
static inline bool jc_is_ascii_digit(const char c) { return (c < '9' && c > '0'); }

static inline unsigned int jc_count_whitespace(const char str[static 1]) {
	unsigned int i = 0;
	while (str[i] != ' ' && str[i] != '\0') { ++i; }
	return i;
}
static inline const char *const skipWhitespace(const char str[static 1]) {
	while(jc_is_space(*str)) { str++; }
	return str;
}

// Copy no more than N characters from s to buf. Make sure buf can hold len.
const char *strncpy(char buf[static 1], const size_t len, const char s[len]) {
	assert(buf && s);
	for (size_t i = 0; i < len; ++i) { buf[i] = s[i]; }
	return buf;
}

typedef struct string string;
struct string {
	const size_t len;
	const char *s;
};

static string string_new(const size_t len, const char s[len]) {
	string str = {.len = len, .s = strncpy(str.s, len, s) };
	return str;
}

static string_view *sv_init(string_view *sv) {
	if (sv) { *sv = (string_view){0}; }
	return sv;
}

static string_view sv_new(void) {
	string_view sv = {0};
	return sv;
}

string_view *const sv_trim_whitespace_left(string_view *const sv) {
	while (*sv->s == ' ' && sv->count > 0) { sv->s++; sv->count--;}
	return sv;
}

string_view *const sv_trim_whitespace_right(string_view *const sv) {
	for (char *c = sv->s+sv->count; sv->count > 0; sv->count--) { if (*c != ' ') { break; } }
	return sv;
}

string_view *const sv_trim_whitespace(string_view *const sv) {
	return sv_trim_whitespace_right(sv_trim_whitespace_left(sv));
}



int atoi(const char s[static const restrict 1]) {
	int result = 0;
	size_t i = 0;
	while (s[i] == ' ') { ++i; }
	bool is_negative = (s[i] == '-');
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (c > '9' || c < '0') { break; } // Check > 9 to short-stroke because statistically that's more likely.
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

unsigned int atou(const char * const restrict s) {
	unsigned int result = 0;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	for (i += (s[i] == '+');; ++i) {
		const char c = s[i];
		if (c > '9' || c < '0') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return result;
}

long int atol(const char * const restrict s) {
	long int result = 0;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	bool is_negative = (s[i] == '-');
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (c > '9' || c < '0') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

long long int atoll(const char * const restrict s) {
	long long int result = 0;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	bool is_negative = (s[i] == '-');
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (c > '9' || c < '0') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

float atof(const char * const restrict s) {
	float result = 0.0f;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	bool is_negative = (s[i] == '-');
	unsigned int pre_dot = 0;
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (c > '9' || c < '0') { break; }
		pre_dot = (pre_dot * 10) + (c & 0xf);
	}
	if (s[i] == '.') {
		unsigned int n = 0, post_dot = 0;
		for (i += 1;; ++n, ++i) {
			const char c = s[i];
			if (c > '9' || c < '0') { break; }
			post_dot = (post_dot * 10) + (c & 0xf);
		}
		result = post_dot;
		for (; n > 0; --n) { result /= 10; }
	}
	result += pre_dot;
	return is_negative ? -result : result;
}

double atod(const char * const restrict s) {
	double result = 0.0f;
	size_t i = 0;
	for (; s[i] == ' '; ++i) {}
	bool is_negative = (s[0] == '-');
	unsigned int pre_dot = 0;
	for (i += is_negative || s[i] == '+';; ++i) {
		const char c = s[i];
		if (c < '0' || c > '9') { break; }
		pre_dot = (pre_dot * 10) + (c & 0xf);
	}
	if (s[i] == '.') {
		unsigned int n = 0, post_dot = 0;
		for (i += 1;; ++n, ++i) {
			const char c = s[i];
			if (c < '0' || c > '9') { break; }
			post_dot = (post_dot * 10) + (c & 0xf);
		}
		result = post_dot;
		for (; n > 0; --n) { result /= 10; }
	}
	result += pre_dot;
	return is_negative ? -result : result;
}

double strtod(const char * const s, char ** const end_ptr) {
	if (s[0] == *end_ptr[0]) { return 1.0; }
	return 0.0;
}

int strtoi(const char * const s, const char * const * const end_ptr) {
	bool is_negative = (*s == '-');
	const char *const *ptr = &s + is_negative; // Add 1 if we're negative to bypass the "-" symbol.
	int result = 0;
	while (ptr++ < end_ptr) {
		const char c = **ptr;
		if (c < '0' || c > '9') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

size_t strlen(const char *const restrict s) {
	const char *p = s;
	while (*p++) {}
	return (p - s - 1);
}


