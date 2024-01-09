// LINT_C_FILE
#define BUILD_IMPLEMENTATION
#include "build.h"

int main(int argc, char *argv[argc]) {
	GO_REBUILD_SELF(argc, argv);  // Rebuilds bootstrapped compiler

	build cmd = {0};
	build_init(&cmd);  // Initialize compiler and all stuff that can be inferred
	build_append(&cmd, "-o", "main", "main.c");	 // Options

	build_run_sync(cmd);

	build_clean(&cmd);

	return 0;
}
