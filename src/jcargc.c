//LINT_C_FILE

#include "./jcargc.h"
#include "./jcstddef.h"

const char *pop_argv(int *const argc, char **argv[*argc]) {
	if (*argc <= 0) { return nullptr; }
	const char *const ret = *argv[0];
	*argv += 1;
	*argc -= 1;
	return ret;
}
