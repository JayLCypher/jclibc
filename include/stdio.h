
#define __need_size_t
#include <jclibc/stddef.h>

#define __FILE_TYPE__      \
struct {                   \
	int IO_MODE;           \
	int BUF_MODE;          \
	unsigned char *buffer; \
};

typedef __FILE_TYPE__ std_file_stream;
typedef __SIZE_TYPE__ std_file_pos;

#define __STDIO
#define __STDOUT
#define __STDERR

static constexpr int EOF = -1;

#define SEEK_SET
#define SEEK_CUR
#define SEEK_END

/* OS SPECIFICS */
#define FILEHANDLE_MAX
#define FILENAME_MAX

#define BUFSIZE 65535

static constexpr file *const stdio  = &STDIO;
static constexpr file *const stdout = &STDOUT;
static constexpr file *const stderr = &STDERR;

enum FILE_MODE_INDEX {
	FILE_READ          = 'r',
	FILE_WRITE         = 'w',
	FILE_APPEND        = 'a',
	FILE_EX            = '+',
	FILE_BINARY        = 'b',
	FILE_NEW           = 'x',
	FILE_DEFAULT_PERMS = 'u',
};

extern std_file_stream *std_file_open(
	const char file_name[restrict const],
	const char mode[static restrict const 5]
);

extern std_error std_file_open_s(
	std_file_stream *restrict stream,
	const char file_name[restrict const],
	const char mode[static restrict const 5]
);

extern std_file_stream *std_file_reopen(
	const char file_name[restrict const],
	const char mode[static restrict const 5],
	std_file_stream *restrict stream
);

extern std_error std_file_reopen_s(
	std_file_stream *restrict new_stream,
	const char file_name[restrict const],
	const char mode[static restrict const 5],
	std_file_stream *restrict old_stream
);

extern int std_file_close(std_file_stream * stream);

extern int std_file_flush(std_file_stream * stream);
extern size_t std_file_read(void *restrict buffer, size_t size, size_t count, std_file_stream *restrict stream)
extern size_t std_file_write(const void *restrict buffer, size_t size, size_t count, std_file_stream *restrict stream);

extern long std_file_tell(std_file_stream * stream);
extern int std_file_wide(std_file_stream * stream, int mode);

static constexpr unsigned char __std_stream_buffer[BUFSIZE] = {0};
/* Sets the internal buffer for stream operation. */
extern void std_set_buf(std_file_stream *restrict stream, unsigned char *restrict buffer);
extern int std_set_buf_v(std_file_stream *restrict stream, unsigned char *restrict buffer, int mode, size_t size);

enum STD_BUFFER_MODE {
	STD_IOFBF,
	STD_IOLBF,
	STD_IONBF,
};
