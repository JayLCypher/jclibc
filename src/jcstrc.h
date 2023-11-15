// LINT_C_FILE
#pragma once
#ifndef _JCSTRC_H_
#define _JCSTRC_H_

//#define JCLIBC
#include "./jcstddef.h"

/*
 * * * * * * * * * * * *
 * DECLARATION SECTION *
 * * * * * * * * * * * *
*/

enum NUMBER_BASE {
	BASE_2,
	BASE_8,
	BASE_10,
	BASE_16,
	BASE_COUNT
};

constexpr char whitespace[] = { ' ', '\n', '\t', '\0', '\v', '\f', '\r' };
constexpr size_t whitespace_sz = sizeof (whitespace) / sizeof (whitespace[0]);

typedef struct _string_view string_view;
struct _string_view {
	size_t count;
	const char *s;
};

void sv_print(string_view *const);

string_view *sv_trim_whitespace_left(string_view *const);
string_view *sv_trim_whitespace_right(string_view *const);
string_view *sv_trim_whitespace(string_view *const);

bool sv_eq(const string_view *, const string_view *);

int atoi(const char[static 1]);
unsigned int atou(const char[static 1]);
long int atol(const char[static 1]);
long long int atoll(const char[static 1]);
float atof(const char[static 1]);
double atod(const char[static 1]);

// cstr_to_X
double cstr_to_d(const char *const, const char *const *const, const enum NUMBER_BASE);
int cstr_to_i(const char *const, const char *const *const, const enum NUMBER_BASE);

size_t cstr_len(const char *const restrict);
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
