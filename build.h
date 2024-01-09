// LINT_C_FILE
#ifndef BUILD_H_
#define BUILD_H_
#include "config.h"

#ifndef COMPILER
#error "Please bootstrap with -DCOMPILER='compiler' or add #define COMPILER 'compiler' to config.h"
#endif

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>

#if __STDC_VERSION__ < 202311L
#define nullptr (void*)0
#include <stdbool.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#    define WIN32_LEAN_AND_MEAN
#    define _WINUSER_
#    define _WINGDI_
#    define _IMM_
#    define _WINCON_
#    include <windows.h>
#    include <direct.h>
#    include <shellapi.h>

#define LINE_END "\r\n"
#else
#    include <limits.h>
#    include <sys/types.h>
#    include <sys/wait.h>
#    include <sys/stat.h>
#    include <unistd.h>
#    include <fcntl.h>

#define LINE_END "\n"
#endif

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define ASSERT_UNREACHABLE assert(0 && "unreachable at "__FILE__ ":" STRINGIZE(__LINE__))

#define return_defer(value) do { result = (value); goto defer; } while (0)

#define ARRAY_LEN(array) (sizeof (array) / sizeof (array[0]))

#define DA_INIT_CAP 256
#define DA_NEW(T, label) typedef struct _da_##label { size_t count; size_t capacity; T *items; } da_##label
#define DA_FREE(da) free((da)->items)
#define da_append(da, n, new_items) do { \
	assert((n) > 0); \
	if ((da)->capacity <= (da)->count + n) { \
		if ((da)->capacity == 0) { (da)->capacity = DA_INIT_CAP; } \
		while ((da)->capacity < (da)->count + (n)) { (da)->capacity *= 2; } \
		(da)->items = reallocarray((da)->items, (da)->capacity, sizeof ((da)->items[0])); \
	} \
	memcpy(&(da)->items[(da)->count], (new_items), (n) * sizeof ((da)->items[0])); \
	(da)->count += (n); \
} while (0)

const char *pop_argv(int *const argc, char **argv[static *argc]);

DA_NEW(const char *, fs_paths); // file_paths dynamic array

typedef enum {
	FT_REGULAR,
	FT_DIRECTORY,
	FT_SYMLINK,
	FT_OTHER,
	FT_COUNT
} file_type;

file_type ft_get(const char path[static PATH_MAX]);

// File system manipulation
bool fs_mkdir_if_not_exists(const char path[static PATH_MAX]);
bool fs_copy_file(const char src_path[static PATH_MAX], const char dst_path[static PATH_MAX]);
bool fs_copy_dir_recursive(const char src_path[static PATH_MAX], const char dst_path[static PATH_MAX]);
bool fs_read_entire_dir(const char *parent, da_fs_paths *children);
bool fs_write_entire_file(const char path[static PATH_MAX], size_t size, const void *data);
bool fs_rename(const char old_path[static PATH_MAX], const char new_path[static PATH_MAX]);
int fs_file_exists(const char file_path[static PATH_MAX]);
int fs_is_path1_modified_after_path2(const char path1[static PATH_MAX], const char path2[static PATH_MAX]);

DA_NEW(char, sb); // string_builder dynamic array
bool fs_read_entire_file_to_sb(const char path[static PATH_MAX], da_sb *sb);
#define sb_append_buf(sb, buf, size) da_append((sb), (size), (buf))
#define sb_append_cstr(sb, cstr, len) da_append((sb), (len), (cstr))
#define sb_append_sz(sb) da_append((sb), 1, "")
#define sb_free(sb) DA_FREE((sb))

// String View Class
typedef struct _string_view_ string_view;
struct _string_view_ {
	size_t len;
	const char *data;
};

const char *sv_tmp_to_cstr(string_view *sv);

string_view sv_from_parts(const size_t len, const char s[len]);
string_view sv_from_cstr(const char s[static 1]);
string_view sv_trim(string_view sv);
string_view sv_chop_by_delim(string_view *sv, const char delim);
bool sv_eq(const string_view a, const string_view b);

#ifndef SV_Fmt
#define SV_Fmt "%.*s"
#endif // SV_Fmt
#ifndef SV_Arg
#define SV_Arg(sv) (int)(sv).count, (sv).data
#endif // SV_Arg
// USAGE:
//   String_View name = ...;
//   printf("Name: "SV_Fmt"\n", SV_Arg(name));

// Multiprocessing
// Process handle
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
typedef HANDLE proc_desc;
#define INVALID_PROC INVALID_HANDLE_VALUE
#else // NOT windows.
typedef int proc_desc;
#define INVALID_PROC (-1)
#endif // _WINDOWS

DA_NEW(proc_desc, pd);
bool proc_wait(proc_desc pd);
bool procs_wait(da_pd *procs);

typedef enum _log_level {
	LOG_NONE,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_COUNT
} log_level;

void nob_log(log_level level, const char fmt[static 1], ...);

typedef struct _build_t build;
struct _build_t {
	size_t count;
	size_t capacity;
	const char **items;
};

void build_render(build cmd, da_sb *render);
#define build_append(build, ...) \
	da_append((build), (sizeof ((const char*[]){__VA_ARGS__}) / sizeof (const char *)), ((const char *[]){__VA_ARGS__}))
#define build_free(build) DA_FREE((build))
#define build_clean(build) do { memset((build)->items, 0, (build)->count * sizeof ((build)->items[0])); (build)->count = 0; } while (0)

void build_init(build *);
//void build_append(build *, const char *[]);

int build_needs_rebuild(const char output_path[static PATH_MAX], const size_t input_paths_count, const char *input_paths[input_paths_count]);
int build_needs_rebuild1(const char output_path[static PATH_MAX], const char input_path[static PATH_MAX]);

proc_desc build_run_async(const build b);
bool build_run_sync(const build b);

// Temporaries
#ifndef BUILD_TEMP_CAPACITY
#define BUILD_TEMP_CAPACITY (8*1024*1024)
#endif // BUILD_TEMP_CAPACITY
char *temp_strdup(const char *cstr);
void *temp_alloc(size_t size);
char *temp_sprintf(const char *format, ...);
void temp_reset(void);
size_t temp_save(void);
void temp_rewind(size_t checkpoint);

#ifndef REBUILD_SELF
	#if defined(_MSC_VER)
		#define OUT "/out:"
	#else
		#define OUT "-o"
	#endif
	#define REBUILD_SELF(binary_path, src_path) COMPILER, src_path, OUT, binary_path
#endif

#define GO_REBUILD_SELF(argc, argv) \
do { \
	assert(argc >= 1); \
	const char source_path[] = __FILE__; \
	const char *binary_path = argv[0]; \
	if (build_needs_rebuild(binary_path, 1, (const char *[]){source_path})) { \
		da_sb sb = {0}; \
		sb_append_cstr(&sb, binary_path, strlen(binary_path)); \
		sb_append_cstr(&sb, ".old", 5); \
		sb_append_sz(&sb); \
		if (fs_rename(binary_path, sb.items) == 0) { exit(EXIT_FAILURE); } \
		build cmd = {0}; \
		build_append(&cmd, REBUILD_SELF(binary_path, source_path)); \
		if (build_run_sync(cmd) == false) { \
			build_free(&cmd); \
			fs_rename(sb.items, binary_path); \
			exit(EXIT_FAILURE); \
		} \
		build_clean(&cmd); \
		da_append(&cmd, argc, argv); \
		if (build_run_sync(cmd) == false) { \
			build_free(&cmd); \
			exit(EXIT_FAILURE); \
		} \
		exit(EXIT_SUCCESS); \
	} \
} while (0)

// minirent.h HEADER BEGIN ////////////////////////////////////////
// Copyright 2021 Alexey Kutepov <reximkut@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// ============================================================
//
// minirent — 0.0.1 — A subset of dirent interface for Windows.
//
// https://github.com/tsoding/minirent
//
// ============================================================
//
// ChangeLog (https://semver.org/ is implied)
//
//    0.0.2 Automatically include dirent.h on non-Windows
//          platforms
//    0.0.1 First Official Release

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

struct dirent { char d_name[MAX_PATH+1]; };

typedef struct DIR DIR;

DIR *opendir(const char *dirpath);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
#else // LINUX
#include <dirent.h>
#endif // _WIN32+64
// minirent.h HEADER END ////////////////////////////////////////

// minirent.h SOURCE BEGIN ////////////////////////////////////////
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
struct DIR {
	HANDLE hFind;
	WIN32_FIND_DATA data;
	struct dirent *dirent;
};

DIR *opendir(const char dirpath[static PATH_MAX]) {
	assert(dirpath);

	char buffer[MAX_PATH];
	snprintf(buffer, MAX_PATH, "%s\\*", dirpath);

	DIR *dir = (DIR*)calloc(1, sizeof(DIR));

	dir->hFind = FindFirstFile(buffer, &dir->data);
	if (dir->hFind == INVALID_HANDLE_VALUE) {
		// TODO: opendir should set errno accordingly on FindFirstFile fail
		// https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
		errno = ENOSYS;
		if (dir) { free(dir); }
		return nullptr;
	}
	return dir;
}

struct dirent *readdir(DIR *dirp) {
	assert(dirp);

	if (dirp->dirent == NULL) { dirp->dirent = (struct dirent*)calloc(1, sizeof(struct dirent)); }
	else {
		if(!FindNextFile(dirp->hFind, &dirp->data)) {
			if (GetLastError() != ERROR_NO_MORE_FILES) {
				// TODO: readdir should set errno accordingly on FindNextFile fail
				// https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
				errno = ENOSYS;
			}
			return nullptr;
		}
	}

	memset(dirp->dirent->d_name, 0, sizeof(dirp->dirent->d_name));
	strncpy(dirp->dirent->d_name, dirp->data.cFileName, sizeof(dirp->dirent->d_name) - 1);
	return dirp->dirent;
}

int closedir(DIR *dirp) {
	assert(dirp);

	if(!FindClose(dirp->hFind)) {
		// TODO: closedir should set errno accordingly on FindClose fail
		// https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
		errno = ENOSYS;
		return -1;
	}

	if (dirp->dirent) { free(dirp->dirent); }
	free(dirp);
	return 0;
}
#endif // _WIN32 + 64
// minirent.h SOURCE END ////////////////////////////////////////

#endif	// BUILD_H_

#pragma once
#ifdef BUILD_IMPLEMENTATION

static size_t build_temp_size = 0;
static char build_temp[BUILD_TEMP_CAPACITY] = {0};

const char *pop_argv(int *const argc, char **argv[static *argc]) {
	if (*argc <= 0) { return nullptr; }
	const char *const ret = *argv[0];
	(*argv) += 1;
	(*argc) -= 1;
	return ret;
}


bool fs_mkdir_if_not_exists(const char path[static PATH_MAX]) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	int result = mkdir(path);
#else
	int result = mkdir(path, 0755);
#endif
	if (result < 0) {
		if (errno == EEXIST) {
			nob_log(LOG_INFO, "directory `%s` already exists", path);
			return true;
		}
		nob_log(LOG_ERROR, "could not create directory `%s`: %s", path, strerror(errno));
		return false;
	}
	nob_log(LOG_INFO, "created directory `%s`", path);
	return true;
}

bool fs_copy_file(const char src_path[static PATH_MAX], const char dst_path[static PATH_MAX]) {
	nob_log(LOG_INFO, "copying %s -> %s", src_path, dst_path);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	if (!CopyFile(src_path, dst_path, FALSE)) {
		nob_log(ERROR, "Could not copy file: %lu", GetLastError());
		return false;
	}
	return true;
#else
	size_t buf_size = 32*1024;
	char *buf = realloc(NULL, buf_size);
	assert(buf != NULL && "Buy more RAM lol!!");
	bool result = true;

	const int src_fd = open(src_path, O_RDONLY);
	if (src_fd < 0) {
		nob_log(LOG_ERROR, "Could not open file %s: %s", src_path, strerror(errno));
		return_defer(false);
	}

	struct stat src_stat = {0};
	if (fstat(src_fd, &src_stat) < 0) {
		nob_log(LOG_ERROR, "Could not get mode of file %s: %s", src_path, strerror(errno));
		return_defer(false);
	}

	const int dst_fd = open(dst_path, O_CREAT | O_TRUNC | O_WRONLY, src_stat.st_mode);
	if (dst_fd < 0) {
		nob_log(LOG_ERROR, "Could not create file %s: %s", dst_path, strerror(errno));
		return_defer(false);
	}

	for (;;) {
		ssize_t n = read(src_fd, buf, buf_size);
		if (n == 0) break;
		if (n < 0) {
			nob_log(LOG_ERROR, "Could not read from file %s: %s", src_path, strerror(errno));
			return_defer(false);
		}
		char *buf2 = buf;
		while (n > 0) {
			const ssize_t m = write(dst_fd, buf2, n);
			if (m < 0) {
				nob_log(LOG_ERROR, "Could not write to file %s: %s", dst_path, strerror(errno));
				return_defer(false);
			}
			n    -= m;
			buf2 += m;
		}
	}

defer:
	free(buf);
	close(src_fd);
	close(dst_fd);
	return result;
#endif
}

// Init function where different configuration stuff can be predefined.
// Supports COMPILER
void build_init(build *cmd) {
	build_append(cmd, COMPILER);
}

void build_render(build cmd, da_sb *render) {
	for (size_t i = 0; i < cmd.count; ++i) {
		const char *arg = cmd.items[i];
		const size_t arg_len = strlen(arg);
		if (arg == NULL) break;
		if (i > 0) sb_append_cstr(render, " ", 1);
		if (!strchr(arg, ' ')) { sb_append_cstr(render, arg, arg_len); }
		else {
			da_append(render, sizeof (char), &(char){'\''});
			sb_append_cstr(render, arg, arg_len);
			da_append(render, sizeof (char), &(char){'\''});
		}
	}
}

proc_desc build_run_async(build cmd) {
	if (cmd.count < 1) {
		nob_log(LOG_ERROR, "Could not run empty command");
		return INVALID_PROC;
	}

	da_sb sb = {0};
	build_render(cmd, &sb);
	sb_append_sz(&sb);
	nob_log(LOG_INFO, "CMD: %s", sb.items);
	sb_free(&sb);
	memset(&sb, 0, sizeof (sb));

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	// https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output
	STARTUPINFO siStartInfo;
	ZeroMemory(&siStartInfo, sizeof(siStartInfo));
	siStartInfo.cb = sizeof(STARTUPINFO);

	// NOTE: theoretically setting NULL to std handles should not be a problem
	// https://docs.microsoft.com/en-us/windows/console/getstdhandle?redirectedfrom=MSDN#attachdetach-behavior
	siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	siStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	// NOTE: checking for errors in GetStdHandle
	if (siStartInfo.hStdError == INVALID_HANDLE_VALUE
		|| siStartInfo.hStdOutput == INVALID_HANDLE_VALUE
		|| siStartInfo.hStdInput == INVALID_HANDLE_VALUE) {
		nob_log(LOG_ERROR, "Error in GetStdHandle with extended info: %lu\n", GetLastError());
	}

	PROCESS_INFORMATION piProcInfo;
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// TODO: use a more reliable rendering of the command instead of cmd_render
	// cmd_render is for logging primarily
	build_render(cmd, &sb);
	sb_append_null(&sb);
	BOOL bSuccess = CreateProcessA(NULL, sb.items, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo);
	sb_free(&sb);

	if (!bSuccess) {
		nob_log(LOG_ERROR, "Could not create child process: %lu", GetLastError());
		return NOB_INVALID_PROC;
	}

	CloseHandle(piProcInfo.hThread);

	return piProcInfo.hProcess;
#else
	const pid_t cpid = fork();
	if (cpid < 0) {
		nob_log(LOG_ERROR, "Could not fork child process: %s", strerror(errno));
		return INVALID_PROC;
	}

	if (cpid == 0) {
		// NOTE: This leaks a bit of memory in the child process.
		// But do we actually care? It's a one off leak anyway...
		build cmd_null = {0};
		da_append(&cmd_null, cmd.count, cmd.items);
		build_append(&cmd_null, NULL);

		if (execvp(cmd.items[0], (char * const*) cmd_null.items) < 0) {
			nob_log(LOG_ERROR, "Could not exec child process: %s", strerror(errno));
			exit(EXIT_FAILURE);
		}
		ASSERT_UNREACHABLE;
	}

	return cpid;
#endif
}

bool procs_wait(da_pd *procs) {
	bool success = true;
	for (size_t i = 0; i < procs->count; ++i) {
		success = proc_wait(procs->items[i]) && success;
	}
	return success;
}

bool proc_wait(proc_desc proc) {
	if (proc == INVALID_PROC) { return false; }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	const DWORD result = WaitForSingleObject(
		proc,    // HANDLE hHandle,
		INFINITE // DWORD  dwMilliseconds
	);

	if (result == WAIT_FAILED) {
		nob_log(LOG_ERROR, "could not wait on child process: %lu", GetLastError());
		return false;
	}

	const DWORD exit_status;
	if (!GetExitCodeProcess(proc, &exit_status)) {
		nob_log(LOG_ERROR, "could not get process exit code: %lu", GetLastError());
		return false;
	}

	if (exit_status != 0) {
		nob_log(LOG_ERROR, "command exited with exit code %lu", exit_status);
		return false;
	}

	CloseHandle(proc);

#else
	for (;;) {
		int wstatus = 0;
		if (waitpid(proc, &wstatus, 0) < 0) {
			nob_log(LOG_ERROR, "could not wait on command (pid %d): %s", proc, strerror(errno));
			return false;
		}

		if (WIFEXITED(wstatus)) {
			const int exit_status = WEXITSTATUS(wstatus);
			if (exit_status != 0) {
				nob_log(LOG_ERROR, "command exited with exit code %d", exit_status);
				return false;
			}

			break;
		}

		if (WIFSIGNALED(wstatus)) {
			nob_log(LOG_ERROR, "command process was terminated by %s", strsignal(WTERMSIG(wstatus)));
			return false;
		}
	}

#endif
	return true;
}

bool build_run_sync(build cmd) {
	proc_desc p = build_run_async(cmd);
	if (p == INVALID_PROC) { return false; }
	return proc_wait(p);
}

void nob_log(log_level level, const char fmt[static 1], ...) {
	switch (level) {
		case LOG_INFO:
			fprintf(stderr, "[INFO] ");
			break;
		case LOG_WARN:
			fprintf(stderr, "[WARNING] ");
			break;
		case LOG_ERROR:
			fprintf(stderr, "[ERROR] ");
			break;
		default: ASSERT_UNREACHABLE;
	}

	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

bool fs_read_entire_dir(const char parent_path[static PATH_MAX], da_fs_paths *children) {
	bool result = true;
	DIR *dir = opendir(parent_path);
	if (dir == nullptr) {
		nob_log(LOG_ERROR, "Could not open directory %s: %s", parent_path, strerror(errno));
		return_defer(false);
	}

	errno = 0;
	struct dirent *ent = readdir(dir);
	while (ent != nullptr) {
		da_append(children, 1, temp_strdup(ent->d_name));
		ent = readdir(dir);
	}

	if (errno != 0) {
		nob_log(LOG_ERROR, "Could not read directory %s: %s", parent_path, strerror(errno));
		return_defer(false);
	}

defer:
	if (dir) { closedir(dir); }
	return result;
}

bool fs_write_entire_file(const char path[static PATH_MAX], size_t size, const void *data) {
	bool result = true;

	FILE *const f = fopen(path, "wb");
	if (f == nullptr) {
		nob_log(LOG_ERROR, "Could not open file %s for writing: %s\n", path, strerror(errno));
		return_defer(false);
	}

	//           len
	//           v
	// aaaaaaaaaa
	//     ^
	//     data
	const char *buf = data;
	while (size > 0) {
		size_t n = fwrite(buf, 1, size, f);
		if (ferror(f)) {
			nob_log(LOG_ERROR, "Could not write into file %s: %s\n", path, strerror(errno));
			return_defer(false);
		}
		size -= n;
		buf  += n;
	}

defer:
	if (f) { fclose(f); }
	return result;
}

file_type fs_get_file_type(const char path[static PATH_MAX]) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	const DWORD attr = GetFileAttributesA(path);
	if (attr == INVALID_FILE_ATTRIBUTES) {
		nob_log(NOB_ERROR, "Could not get file attributes of %s: %lu", path, GetLastError());
		return -1;
	}
	if (attr & FILE_ATTRIBUTE_DIRECTORY) return FT_DIRECTORY;
	// TODO(tsoding): detect symlinks on Windows (whatever that means on Windows anyway)
	return FT_REGULAR;
#else // _WIN32
	struct stat statbuf;
	if (stat(path, &statbuf) < 0) {
		nob_log(LOG_ERROR, "Could not get stat of %s: %s", path, strerror(errno));
		return -1;
	}

	switch (statbuf.st_mode & S_IFMT) {
		case S_IFDIR:  return FT_DIRECTORY;
		case S_IFREG:  return FT_REGULAR;
		case S_IFLNK:  return FT_SYMLINK;
		default:       return FT_OTHER;
	}
#endif // _WIN32
}

bool fs_copy_directory_recursively(const char src_path[static PATH_MAX], const char dst_path[static PATH_MAX]) {
	bool result = true;
	da_fs_paths children = {0};
	da_sb src_sb = {0};
	da_sb dst_sb = {0};
	const size_t temp_checkpoint = temp_save();

	const file_type type = fs_get_file_type(src_path);
	if (type < 0) { return false; }

	size_t src_path_len = strlen(src_path);
	size_t dst_path_len = strlen(dst_path);
	switch (type) {
		case FT_DIRECTORY: {
			if (!fs_mkdir_if_not_exists(dst_path)) return_defer(false);
			if (!fs_read_entire_dir(src_path, &children)) return_defer(false);

			for (size_t i = 0; i < children.count; ++i) {
				if (strcmp(children.items[i], ".") == 0) continue;
				if (strcmp(children.items[i], "..") == 0) continue;

				const size_t item_len = strlen(children.items[i]);
				src_sb.count = 0;
				sb_append_cstr(&src_sb, src_path, src_path_len);
				sb_append_cstr(&src_sb, "/", 1);
				sb_append_cstr(&src_sb, children.items[i], item_len);
				sb_append_sz(&src_sb);

				dst_sb.count = 0;
				sb_append_cstr(&dst_sb, dst_path, dst_path_len);
				sb_append_cstr(&dst_sb, "/", 1);
				sb_append_cstr(&dst_sb, children.items[i], item_len);
				sb_append_sz(&dst_sb);

				if (!fs_copy_directory_recursively(src_sb.items, dst_sb.items)) { return_defer(false); }
			}
		} break;

		case FT_REGULAR: {
			if (fs_copy_file(src_path, dst_path) == 0) { return_defer(false); }
		} break;

		case FT_SYMLINK: {
			nob_log(LOG_WARN, "TODO(tsoding): Copying symlinks is not supported yet");
		} break;

		case FT_OTHER: {
			nob_log(LOG_ERROR, "Unsupported type of file %s", src_path);
			return_defer(false);
		} break;

		default: ASSERT_UNREACHABLE;
	}

defer:
	temp_rewind(temp_checkpoint);
	sb_free(&src_sb);
	sb_free(&dst_sb);
	DA_FREE(&children);
	return result;
}

char *temp_strdup(const char *cstr) {
	const size_t n = strlen(cstr);
	char *result = temp_alloc(n + 1);
	assert(result != nullptr && "Increase BUILD_TEMP_CAPACITY");
	memcpy(result, cstr, n);
	result[n] = '\0';
	return result;
}

void *temp_alloc(const size_t size) {
	if (build_temp_size + size > BUILD_TEMP_CAPACITY) { return nullptr; }
	void *result = &build_temp[build_temp_size];
	build_temp_size += size;
	return result;
}

char *temp_sprintf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	int n = vsnprintf(nullptr, 0, format, args);
	va_end(args);

	assert(n >= 0);
	char *result = temp_alloc(n + 1);
	assert(result != nullptr && "Extend the size of the temporary allocator");
	// TODO(tsoding): use proper arenas for the temporary allocator;
	va_start(args, format);
	vsnprintf(result, n + 1, format, args);
	va_end(args);

	return result;
}

void temp_reset(void) { build_temp_size = 0; }
size_t temp_save(void) { return build_temp_size; }
void temp_rewind(const size_t checkpoint) { build_temp_size = checkpoint; }
const char *temp_sv_to_cstr(string_view sv) {
	char *result = temp_alloc(sv.len + 1);
	assert(result != nullptr && "Extend the size of the temporary allocator");
	memcpy(result, sv.data, sv.len);
	result[sv.len] = '\0';
	return result;
}

int build_needs_rebuild(const char output_path[static PATH_MAX], const size_t input_paths_count, const char *input_paths[input_paths_count]) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	BOOL bSuccess = false;

	HANDLE output_path_fd = CreateFile(output_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (output_path_fd == INVALID_HANDLE_VALUE) {
		// NOTE: if output does not exist it 100% must be rebuilt
		if (GetLastError() == ERROR_FILE_NOT_FOUND) return 1;
		nob_log(LOG_ERROR, "Could not open file %s: %lu", output_path, GetLastError());
		return -1;
	}
	FILETIME output_path_time;
	bSuccess = GetFileTime(output_path_fd, NULL, NULL, &output_path_time);
	CloseHandle(output_path_fd);
	if (bSuccess == false) {
		nob_log(LOG_ERROR, "Could not get time of %s: %lu", output_path, GetLastError());
		return -1;
	}

	for (size_t i = 0; i < input_paths_count; ++i) {
		const char *input_path = input_paths[i];
		HANDLE input_path_fd = CreateFile(input_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
		if (input_path_fd == INVALID_HANDLE_VALUE) {
			// NOTE: non-existing input is an error cause it is needed for building in the first place
			nob_log(LOG_ERROR, "Could not open file %s: %lu", input_path, GetLastError());
			return -1;
		}
		FILETIME input_path_time;
		bSuccess = GetFileTime(input_path_fd, NULL, NULL, &input_path_time);
		CloseHandle(input_path_fd);
		if (bSuccess == false) {
			nob_log(LOG_ERROR, "Could not get time of %s: %lu", input_path, GetLastError());
			return -1;
		}

		// NOTE: if even a single input_path is fresher than output_path that's 100% rebuild
		if (CompareFileTime(&input_path_time, &output_path_time) == 1) return 1;
	}
#else
	struct stat statbuf = {0};

	if (stat(output_path, &statbuf) < 0) {
		// NOTE: if output does not exist it 100% must be rebuilt
		if (errno == ENOENT) return 1;
		nob_log(LOG_ERROR, "could not stat %s: %s", output_path, strerror(errno));
		return -1;
	}
	const int output_path_time = statbuf.st_mtime;

	for (size_t i = 0; i < input_paths_count; ++i) {
		const char *input_path = input_paths[i];
		if (stat(input_path, &statbuf) < 0) {
			// NOTE: non-existing input is an error cause it is needed for building in the first place
			nob_log(LOG_ERROR, "could not stat %s: %s", input_path, strerror(errno));
			return -1;
		}
		int input_path_time = statbuf.st_mtime;
		// NOTE: if even a single input_path is fresher than output_path that's 100% rebuild
		if (input_path_time > output_path_time) return 1;
	}

#endif
	return 0;
}

int build_needs_rebuild1(const char output_path[static PATH_MAX], const char input_path[static PATH_MAX]) {
	return build_needs_rebuild(output_path, 1, &input_path);
}

bool fs_rename(const char old_path[static PATH_MAX], const char new_path[static PATH_MAX]) {
	nob_log(LOG_INFO, "renaming %s -> %s", old_path, new_path);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	if (!MoveFileEx(old_path, new_path, MOVEFILE_REPLACE_EXISTING)) {
		nob_log(LOG_ERROR, "could not rename %s to %s: %lu", old_path, new_path, GetLastError());
		return false;
	}
#else
	if (rename(old_path, new_path) < 0) {
		nob_log(LOG_ERROR, "could not rename %s to %s: %s", old_path, new_path, strerror(errno));
		return false;
	}
#endif // _WIN32+64
	return true;
}

bool fs_read_entire_file(const char path[static PATH_MAX], da_sb *sb) {
	bool result = true;
	const size_t buf_size = 32*1024;
	char *buf = realloc(NULL, buf_size);
	assert(buf != NULL && "Buy more RAM lool!!");
	FILE *const f = fopen(path, "rb");
	if (f == nullptr) {
		nob_log(LOG_ERROR, "Could not open %s for reading: %s", path, strerror(errno));
		return_defer(false);
	}

	size_t n = fread(buf, 1, buf_size, f);
	while (n > 0) {
		sb_append_buf(sb, buf, n);
		n = fread(buf, 1, buf_size, f);
	}
	if (ferror(f)) {
		nob_log(LOG_ERROR, "Could not read %s: %s\n", path, strerror(errno));
		return_defer(false);
	}

defer:
	free(buf);
	if (f) { fclose(f); }
	return result;
}

string_view sv_from_parts(size_t count, const char data[count]) {
	string_view sv = { .len = count, .data = data };
	return sv;
}

string_view sv_chop_by_delim(string_view *sv, const char delim) {
	size_t i = 0;
	for (i; i < sv->len && sv->data[i] != delim; ++i) {}

	string_view result = sv_from_parts(i, sv->data);
	bool off_by_one = (i < sv->len);
	sv->len -= i + off_by_one;
	sv->data  += i + off_by_one;
	return result;
}

string_view sv_trim_left(string_view sv) {
	size_t i = 0;
	for (i; i < sv.len && isspace(sv.data[i]); ++i) {}
	return sv_from_parts(sv.len - i, sv.data + i);
}

string_view sv_trim_right(string_view sv) {
	size_t i = 0;
	for (i; i < sv.len && isspace(sv.data[sv.len - 1 - i]); ++i) {}
	return sv_from_parts(sv.len - i, sv.data);
}

string_view sv_trim(string_view sv) {
	return sv_trim_right(sv_trim_left(sv));
}

string_view sv_from_cstr(const char *cstr) {
	return sv_from_parts(strlen(cstr), cstr);
}

bool sv_eq(const string_view a, const string_view b) {
	if (a.len != b.len) { return false; }
	return memcmp(a.data, b.data, a.len) == 0;
}

// RETURNS:
//  0 - file does not exists
//  1 - file exists
// -1 - error while checking if file exists. The error is logged
int fs_file_exists(const char *file_path) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	// TODO: distinguish between "does not exists" and other errors
	const DWORD dwAttrib = GetFileAttributesA(file_path);
	return dwAttrib != INVALID_FILE_ATTRIBUTES;
#else
	struct stat statbuf;
	if (stat(file_path, &statbuf) < 0) {
		if (errno == ENOENT) return 0;
		nob_log(LOG_ERROR, "Could not check if file %s exists: %s", file_path, strerror(errno));
		return -1;
	}
	return 1;
#endif
}

#endif // BUILD_IMPLEMENTATION


// Defines for platforms if needed.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
// define something for Windows (32-bit and 64-bit, this part is common)
#ifdef _WIN64
// define something for Windows (64-bit only)
#else
// define something for Windows (32-bit only)
#endif
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS, tvOS, or watchOS Simulator
#elif TARGET_OS_MACCATALYST
// Mac's Catalyst (ports iOS API into Mac, like UIKit).
#elif TARGET_OS_IPHONE
// iOS, tvOS, or watchOS device
#elif TARGET_OS_MAC
// Other kinds of Apple platforms
#else
#error "Unknown Apple platform"
#endif
#elif __ANDROID__
// Below __linux__ check should be enough to handle Android,
// but something may be unique to Android.
#elif __linux__
// linux
#elif __unix__	// all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif
