// LINT_C_FILE
#include <limits.h>
#include <stdio.h>
#include <assert.h>

// JCLIBC INCLUDES
#include "include/jcstring.h"


int main(void) {
	// Tests all of the functions in the standard library using their test suites.

	{
		int k = atoi("420kekw");
		printf("atoi: %d\n", k);
		assert(k == 420);
	}
	{
		unsigned int k = atou("1590437432kekek");
		printf("atou: %u\n", k);
		assert(k == 1590437432);
	}
	{
		long int k = atol("-420kek");
		printf("atol: %ld\n", k);
		assert(k == -420);
	}
	{
		long long int k = atoll("-11245718420kek");
		printf("atoll: %lld\n", k);
		assert(k == -11245718420);
	}
	{
		float k = atof("-4.223239238f");
		printf("atof: %9.6f\n", (double)k);
		assert(k < -4.223239230 && k > -4.22323924);
	}
	{
		double k = atod("-565995.223239238dw");
		printf("atod: %f\n", k);
		assert(k < -565995.223239230 && k > -565995.22323924);
	}
	{
		const char s[] = "Hello world bbbbbbb";
		const size_t len = strlen(s);
		printf("%s is length %lu\n", s, len);
		assert(len == 16);
	}
	{
		string_view sv = { 10, "   hello  " };
		printf("%.*s\n", sv.count & INT_MAX, sv.s);
		sv_trim_whitespace(&sv);
		printf("%.*s\n", sv.count & INT_MAX, sv.s);
	}

		return 0;
}
