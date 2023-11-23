//LINT_C_FILE
#pragma once
#ifndef _JCSTRC_H_
#define _JCSTRC_H_

#include <assert.h>

//#define JCLIBC
#include "./jcstddef.h"

/*
 * * * * * * * * * * * *
 * DECLARATION SECTION *
 * * * * * * * * * * * *
*/

constexpr char whitespace[] = { ' ', '\n', '\t', '\0', '\v', '\f', '\r' };
constexpr size_t whitespace_sz = sizeof (whitespace) / sizeof (whitespace[0]);

typedef struct _string string;
struct _string {
	size_t len;
	char *s;
};
static_assert(sizeof (string) == 16, "Wrong size of string!");

string *string_trim_whitespace_left(string *string);
string *string_trim_whitespace_right(string *s);
string *string_trim_whitespace(string *s);

typedef struct _string_view string_view;
struct _string_view {
	const size_t count;
	const char *s;
};

void sv_print(const string_view *);

string_view sv_trim_whitespace_left(const string_view *);
string_view sv_trim_whitespace_right(const string_view *);
string_view sv_trim_whitespace(const string_view *);

bool sv_eq(const string_view *, const string_view *);

int atoi(const char[static 1]);
unsigned int atou(const char[static 1]);
long int atol(const char[static 1]);
long long int atoll(const char[static 1]);
float atof(const char[static 1]);
double atod(const char[static 1]);

// cstr_to_X
double cstr_to_d(const char[static 2], const char **, const unsigned);
int cstr_to_i(const char[static 2], const char **, const unsigned);

size_t cstr_len(const char[static 1]);
const char *cstr_end(const char[static 1]);
const char *cstr_ncpy(const size_t n, const char[static n], char[static n]);
int cstr_ncmp(const size_t n, const char[static n], const char[static n]);
const char *cstr_chr(const char[static 1], const char);
const char *cstr_rchr(const char[static 1], const char);
const char *cstr_rchrr(const char[static 1], const char *, const char);

/*
 * * * * * * * * * * * * * *
 * IMPLEMENTATION SECTIONS *
 * * * * * * * * * * * * * *
*/
#ifdef JC_STRING_VIEW_IMPLEMENTATION

#endif

#endif // _JCSTRC_H_
