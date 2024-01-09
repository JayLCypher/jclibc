// LINT_C_FILE
#pragma once
#ifndef _JCBUILDC_H

#ifndef JCBUILDC_IMPLEMENTATION
#include <assert.h>
void jcbuild_rebuild_self(const int *argc, const char **argv[*argc]) {
	assert(*argc >= 0);
	const char *src_path = __FILE__;
	const char *bin_path = *argv[0];

	if (jcbuild_needs_rebuild(bin_path, src_path, 1)) {
		const char old_path[PATH]
	}

	// exit(0);
}

#endif

#endif
