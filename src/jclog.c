#include <__stddef_size_t.h>
#include <stdio.h>

enum LOG_TYPE {
	LOG_TO_NONEE,
	LOG_TO_STDOUT,
	LOG_TO_STDERR,
	LOG_TO_FILE,
	LOG_TO_MAX
};

enum LOG_LEVEL {
	LOG_ERROR,
	LOG_DEBUG,
	LOG_INFO,
	LOG_NONE,
	LOG_MAX
};

static const char *log_level[LOG_MAX] = {
	"ERROR",
	"DEBUG",
	"INFO",
	""
};

#define SEVERITY(level) "[" level "]"
#define LOG(severity, str) do { fprintf(stderr, "[%s] %s:%d: %s\n", severity, __FILE__, __LINE__, str); } while(0)

typedef struct logger jclogger;
struct logger {
	FILE *const stream;
	enum LOG_LEVEL level;
};

jclogger jc_log_create(jclogger *const lg, const char *const file_path) {
	jclogger l;
	if (file_path) {
		
	}
	return l;
}

void jc_log(const jclogger *const lg, const char *str) {
	if (lg->level < LOG_NONE) { fprintf(lg->stream, "[%s]: %s:%d: %s\n", log_level[lg->level], __FILE__, __LINE__, str); }
}