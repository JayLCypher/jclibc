#ifndef _JCSTRING_H
#define _JCSTRING_H

#if __STDC_VERSION__ < 202311L
#define bool _Bool
#define true !0
#define false 0
#endif

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
	bool is_negative (aString[0] == '-');
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

}

int strtoi(const char * const aString, char ** const end_ptr) {
	bool is_negative = (*aString == '-');
	const char **ptr = &aString + is_negative;
	int result = 0;
	while (ptr++ < end_ptr) {
		const char c = **ptr;
		if (c < '0' || c > '9') { break; }
		result = (result * 10) + (c & 0xf);
	}
	return is_negative ? -result : result;
}
#endif
