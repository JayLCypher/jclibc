
#include <jclibc/stddef.h>
#include <jclibc/stdint.h>
#include <jclibc/stdargs.h>

#define TEST_ALL

#ifdef TEST_ALL
#define TEST_ARGS
#define TEST_ATO
#define TEST_STRING
#define TEST_STRING_VIEW
#define TEST_STRING_LEN
#endif

static int test_args(int *const argc, char **argv[*argc]) {
	int argc_len = *argc;
	for (int i = 0; i < argc_len; ++i) {
		if (argc_len - i != *argc) { return -1; }
		printf("Argument [%d]: %s | Argc: %d\n", i, std_args_shr(argc, argv), *argc);
	}
	return 0;
}

static int test_ato(void) {
	printf("Testing atoi: ");
	do {
		int k = atoi(" 9420kekw");
		printf("%d\n", k);
		if (k != 9420) { return -1; }
	} while (0);
	printf("Testing atou: ");
	do {
		unsigned k = atou("+1590437432kekek");
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
		if (k > -4.223238f || k < -4.223240f) { return -1; }
	} while (0);
	printf("Testing atod: ");
	do {
		double k = atod("-565995.223239238dw");
		printf("atod: %f\n", k);
		if (k > -565995.223239230 || k < -565995.22323924) { return -1; }
	} while (0);
	printf("Testing cstr_to_i:\n\t");
	do {
		const char p[] = "10 200000000000000000000000000000 30 -40 junk";
		const char *p_ptr = &p[0], *end = nullptr;
		int k = 0;
		for (size_t i = 0; i < 20; ++i) {
			k = cstr_to_i(p_ptr, &end, 10);
			printf("Pass %zu: k = %d\n\t", i, k);
			if (p_ptr == end) { break; }
			p_ptr = end;
		}
	} while (0);
	return 0;
}

static int test_string(void) {
	const char s[] = "Hello world bbbbbbb";
	const size_t len = cstr_len(s);
	printf("%s is length %zu and my len without null is %zu\n", s, sizeof s, len);
	if (len != sizeof s - 1) { return -1; }

	string_view sv = {sizeof s, s};
	//string jcstr1 = string_from_sv(&sv);
	// String format
	//string jcstr = string_from_cstr("hello mom!");

	return 0;
}

#include <string.h>
static int test_string_len(void) {
	const char kek[100000] = {[0 ... 99998] = 'c'};
	struct timespec t1 = {0}, t2 = {0};
	clock_gettime(0, &t1);
	size_t a = strlen(kek);
	clock_gettime(0, &t2);
	long double t = (t2.tv_nsec - t1.tv_nsec);
	printf("Library strlen got %zu in %.0Lf nsecs\n", a, t);
	clock_gettime(0, &t1);
	size_t b = cstr_len(kek);
	clock_gettime(0, &t2);
	t = (t2.tv_nsec - t1.tv_nsec);
	printf("cstr_len got %zu in %.0Lf nsecs\n", b, t);
	return (a == b);
}

static int test_string_view(void) {
	string_view sv = { 10, "   hello  " };
	sv_print(&sv);
	string_view sv2 = sv_trim_whitespace(&sv);
	sv_print(&sv2);
	return cstr_ncmp(sv2.count, sv2.s, "hello");
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

	#ifdef TEST_STRING_LEN
	assert(test_string_len() == 0);
	#endif

	printf("All tests succeeeded!\n");

	return 0;
}
