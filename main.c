// LINT_C_FILE
#include <stdio.h>
#include <assert.h>

// JCLIBC INCLUDES
#include "./src/jcstrc.h"
#include "./src/jcargc.h"

#define TEST_ALL

#ifdef TEST_ALL
#define TEST_ARGS
#define TEST_ATO
#define TEST_STRING
#define TEST_STRING_VIEW
#endif

static int test_args(int *const argc, char **argv[*argc]) {
	int argc_len = *argc;
	for (int i = 0; i < argc_len; ++i) {
		if (argc_len - i != *argc) { return -1; }
		printf("Argument [%d]: %s | Argc: %d\n", i, pop_argv(argc, argv), *argc);
	}
	return 0;
}

static int test_ato(void) {
	printf("Testing atoi: ");
	do {
		int k = atoi("9420kekw");
		printf("%d\n", k);
		if (k != 9420) { return -1; }
	} while (0);
	printf("Testing atou: ");
	do {
		unsigned int k = atou("1590437432kekek");
		printf("atou: %u\n", k);
		if (k != 1590437432) { return -1; }
	} while (0);
	printf("Testing atol: ");
	do {
		long int k = atol("-420kek");
		printf("atol: %ld\n", k);
		if (k != -420) { return -1; }
	} while (0);
	printf("Testing atoll: ");
	do {
		long long int k = atoll("-11245718420kek");
		printf("atoll: %lld\n", k);
		if (k != -11245718420) { return -1; }
	} while (0);
	printf("Testing atof: ");
	do {
		float k = atof("-4.223239238f");
		printf("atof: %9.6f\n", (double)k);
		if (k > -4.223238f && k < -4.223240f) { return -1; }
	} while (0);
	printf("Testing atod: ");
	do {
		double k = atod("-565995.223239238dw");
		printf("atod: %f\n", k);
		if (k > -565995.223239230 && k < -565995.22323924) { return -1; }
	} while (0);
	return 0;
}

static int test_string(void) {
	const char s[] = "Hello world bbbbbbb";
	const size_t len = cstr_len(s);
	printf("%s is length %zu and my len without null is %zu\n", s, sizeof s, len);
	if (len != sizeof s - 1) { return -1; }
	return 0;
}

static int test_string_view(void) {
	string_view sv = { 10, "   hello  " };
	sv_print(&sv);
	sv_trim_whitespace(&sv);
	if (cstr_ncmp(sv.count, sv.s, "hello") != 0) { return -1; }
	sv_print(&sv);
	return 0;
}

int main(int argc, char *argv[argc]) {
	printf("Running test suites!\n");
	#ifdef TEST_ARGS
	assert(test_args(&argc, &argv) == 0);
	#endif
	// Tests all of the functions in the standard library using their test suites.
	#ifdef TEST_ATO
	assert(test_ato() == 0);
	#endif

	// String
	#ifdef TEST_STRING
	assert(test_string() == 0);
	#endif

	// String_View_Test
	#ifdef TEST_STRING_VIEW
	assert(test_string_view() == 0);
	#endif

	printf("All tests succeeeded!\n");

	return 0;
}
