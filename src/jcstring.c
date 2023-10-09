#include "include/jcstring.h"

static inline bool isAsciiDigit(const char c) { return (c < '9' && c > '0'); }
static inline unsigned int skipWhitespace(const char str[static 1]) {
	unsigned int i = 0;
	for (;str[i] != ' '; ++i) {}
	return i;
}

int atoi(const char * const restrict aString) {
	int result = 0;
	unsigned int i = 0;
	for (;aString[i] != ' '; ++i) {}
	bool is_negative = (aString[i] == '-');
	for (i += is_negative || aString[i] == '+';; ++i) {
		const char c = aString[i];
		if (c > '9' || c < '0') { break; } // Check > 9 to short-stroke because statistically that's more likely.
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

unsigned int atou(const char * const restrict aString) {
	unsigned int result = 0;
	unsigned int i = 0;
	for (;aString[i] != ' '; ++i) {}
	if (aString[i] == '+') { ++i; }
	for (;; ++i) {
		const char c = aString[i];
		if (c > '9' || c < '0') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return result;
}

long int atol(const char * const restrict aString) {
	long int result = 0;
	unsigned int i = 0;
	for (;aString[i] != ' '; ++i) {}
	bool is_negative = (aString[i] == '-');
	for (i += is_negative || aString[i] == '+';; ++i) {
		const char c = aString[i];
		if (c > '9' || c < '0') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

long long int atoll(const char * const restrict aString) {
	long long int result = 0;
	unsigned int i = 0;
	for (;aString[i] != ' '; ++i) {}
	bool is_negative = (aString[i] == '-');
	for (i += is_negative || aString[i] == '+';; ++i) {
		const char c = aString[i];
		if (c > '9' || c < '0') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

float atof(const char * const restrict aString) {
	float result = 0.0f;
	unsigned int i = 0;
	for (;aString[i] != ' '; ++i) {}
	bool is_negative = (aString[i] == '-');
	unsigned int position = 0, pre_dot = 0;
	for (i += is_negative || aString[i] == '+';; ++i) {
		const char c = aString[i];
		if (c > '9' || c < '0') { position = i; break; }
		pre_dot = (pre_dot * 10) + (c & 0xf);
	}
	if (aString[position] == '.') {
		unsigned int n = 0, post_dot = 0;
		for (unsigned int i = position + 1;; ++n, ++i) {
			const char c = aString[i];
			if (c < '0' || c > '9') { break; }
			post_dot = (post_dot * 10) + (c & 0xf);
		}
		result = post_dot;
		for (; n > 0; --n) { result /= 10; }
	}
	result += pre_dot;
	return is_negative ? -result : result;
}

double atod(const char * const restrict aString) {
	double result = 0.0f;
	unsigned int i = 0;
	for (;aString[i] != ' '; ++i) {}
	bool is_negative = (aString[0] == '-');
	unsigned int position = 0, pre_dot = 0;
	for (i += is_negative || aString[i] == '+';; ++i) {
		const char c = aString[i];
		if (c < '0' || c > '9') { position = i; break; }
		pre_dot = (pre_dot * 10) + (c & 0xf);
	}
	if (aString[position] == '.') {
		unsigned int n = 0, post_dot = 0;
		for (unsigned int i = position + 1;; ++n, ++i) {
			const char c = aString[i];
			if (c < '0' || c > '9') { break; }
			post_dot = (post_dot * 10) + (c & 0xf);
		}
		result = post_dot;
		for (; n > 0; --n) { result /= 10; }
	}
	result += pre_dot;
	return is_negative ? -result : result;
}

double strtod(const char * const aString, char ** const end_ptr) {
	if (aString[0] == *end_ptr[0]) { return 1.0; }
	return 0.0;
}

int strtoi(const char * const aString, const char * const * const end_ptr) {
	bool is_negative = (*aString == '-');
	const char *const *ptr = &aString + is_negative; // Add 1 if we're negative to bypass the "-" symbol.
	int result = 0;
	while (ptr++ < end_ptr) {
		const char c = **ptr;
		if (c < '0' || c > '9') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}
