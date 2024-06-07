#include <stdargs.h>

const char *std_args_shr(int *const argc, const char *const *argv[static const *argc]) {
	if (*argc <= 0) { return nullptr; }
	const char *const ret = *argv[0];
	*argv += 1;
	*argc -= 1;
	return ret;
}
