#include <stdio.h>
#include <assert.h>

// JCLIBC INCLUDES
#include "include/jcstring.h"


int main(void) {
	// Tests all of the functions in the standard library using their test suites.

	{
		int k = atoi("420kekw");
		printf("atoi: %d\n", k);
	}
	{
		unsigned int k = atou("1590437432kekek");
		printf("atou: %u\n", k);
	}
	{
		long int k = atol("-420kek");
		printf("atol: %ld\n", k);
	}
	{
		long long int k = atoll("-11245718420kek");
		printf("atoll: %lld\n", k);
	}
	{
		float k = atof("-4.223239238f");
		printf("atof: %9.6f\n", (double)k);
	}
	{
		double k = atod("-565995.223239238dw");
		printf("atod: %f\n", k);
	}

		return 0;
}