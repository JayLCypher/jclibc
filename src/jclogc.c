#include <stddef.h>
#include <stdio.h>

#include "./include/jcstddef.h"

enum LOG_TYPE {
	LOG_TO_NONEE,
	LOG_TO_STDOUT,
	LOG_TO_STDERR,
	LOG_TO_FILE,
	LOG_TO_MAX
};

enum LOG_LEVEL {
	LOG_ERROR,
	LOG_WARN,
	LOG_DEBUG,
	LOG_INFO,
	LOG_NONE,
	LOG_MAX
};

static const char *log_level_text[LOG_MAX] = {
	"ERROR",
	"WARN",
	"DEBUG",
	"INFO",
	""
};

typedef struct _log jclogc;
struct _log {
	FILE *const stream;
	enum LOG_LEVEL level;
};
static inline const char *jclog_get_level_text(enum LOG_LEVEL level) {
	return log_level_text[level];
}

#define SEVERITY(level) "[" level "]"
#define LOG(severity, str) do { fprintf(stderr, "[%s] %s:%d: %s\n", severity, __FILE__, __LINE__, str); } while (0)

jclogc jclog_init(FILE *const stream, enum LOG_LEVEL level) {
	jclogc l = { stream, level };
	return l;
}

jclogc jclog_create(const char *const filename, enum LOG_LEVEL level) {
	jclogc l = { fopen(filename, "w"), level };
	return l;
}

void jclog_destroy(jclogc *l) {
	if (l->stream) { fclose(l->stream); }
}

void jclog(const jclogc *const l, const char *fmt, ...) {
	if (l->level < LOG_NONE) { fprintf(l->stream, "[%s]: %s:%d: %s\n", log_level_text[l->level], __FILE__, __LINE__, fmt); }
}


