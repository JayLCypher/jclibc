// LINT_C_FILE
#pragma once
#ifndef JCSTRC_H
#define JCSTRC_H

#include <assert.h>

#define JCPTRC
#define JCINTC
#include "./jcstddef.h"

/*
 * * * * * * * * * * * *
 * DECLARATION SECTION *
 * * * * * * * * * * * *
 */

static const char whitespace[] = {' ', '\n', '\r', '\t', '\0', '\v', '\f'};
static constexpr size_t whitespace_sz = sizeof (whitespace) / sizeof(whitespace[0]);

typedef struct string_t string;
struct string_t {
	size_t len;
	char *s;
};
static_assert(sizeof (string) == 16, "Wrong size of string!");

string string_from_cstr(const char[static 1]);
string *string_trim_whitespace_left(string *string);
string *string_trim_whitespace_right(string *s);
string *string_trim_whitespace(string *s);
string *string_format(string *s, const char *fmt, va_list *args);

typedef struct string_view_t string_view;
struct string_view_t {
	const size_t count;
	const char *s;
};
static_assert(sizeof (string_view) == 16, "Wrong size of string view!");

#define SV_fmt "%.*s"
#define SV_arg(sv) (int){(sv).count}, (sv).s

string_view sv_new(size_t count, const char[static count]);
void sv_print(const string_view *);

string_view sv_trim_whitespace_left(const string_view *);
string_view sv_trim_whitespace_right(const string_view *);
string_view sv_trim_whitespace(const string_view *);

bool sv_eq(const string_view *, const string_view *);

int32_t atoi(const char[static 1]);
uint32_t atou(const char[static 1]);
int32_t atol(const char[static 1]);
int64_t atoll(const char[static 1]);
float atof(const char[static 1]);
double atod(const char[static 1]);

// cstr_to_X
double cstr_to_d(const char[static 2], const char **, unsigned);
int cstr_to_i(const char[static 2], const char **, unsigned);

size_t cstr_len(const char[static 1]);
const char *cstr_end(const char[static 1]);
const char *cstr_ncpy(size_t n, const char[static n], char[static n]);
int cstr_ncmp(size_t n, const char[static n], const char[static n]);
const char *cstr_chr(const char[static 1], char);
const char *cstr_rchr(const char[static 1], char);
const char *cstr_rchrr(const char[static 1], const char *, char);

#endif	// JCSTRC_H
