#include "include/jcstring.h"

int atoi(const char * const restrict aString) {
	int result = 0;
	bool is_negative = (aString[0] == '-');
	for (unsigned int i = 0 + is_negative;; ++i) {
		const char c = aString[i];
		if (c < '0' || c > '9') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

unsigned int atou(const char * const restrict aString) {
	unsigned int result = 0;
	for (unsigned int i = 0;; ++i) {
		const char c = aString[i];
		if (c < '0' || c > '9') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return result;
}

long int atol(const char * const restrict aString) {
	long int result = 0;
	bool is_negative = (aString[0] == '-');
	for (unsigned int i = 0 + is_negative;; ++i) {
		const char c = aString[i];
		if (c < '0' || c > '9') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

long long int atoll(const char * const restrict aString) {
	long long int result = 0;
	bool is_negative = (aString[0] == '-');
	for (unsigned int i = 0 + is_negative;; ++i) {
		const char c = aString[i];
		if (c < '0' || c > '9') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}

float atof(const char * const restrict aString) {
	float result = 0.0f;
	bool is_negative = (aString[0] == '-');
	unsigned int position = 0, pre_dot = 0;
	for (unsigned int i = 0 + is_negative;; ++i) {
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

double atod(const char * const restrict aString) {
	double result = 0.0f;
	bool is_negative = (aString[0] == '-');
	unsigned int position = 0, pre_dot = 0;
	for (unsigned int i = 0 + is_negative;; ++i) {
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