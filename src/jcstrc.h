// LINT_C_FILE
#pragma once
#ifndef _JCSTRC_H_
#define _JCSTRC_H_

#include "./jcstddef.h"

#define JCLIBC

constexpr char whitespace[] = { ' ', '\n', '\t', '\0', '\v', '\f', '\r' };
constexpr size_t whitespace_sz = sizeof (whitespace);

typedef struct _string_view string_view;
struct _string_view {
	size_t count;
	char *s;
};

void sv_print(string_view *const);

string_view *sv_trim_whitespace_left(string_view *const);
string_view *sv_trim_whitespace_right(string_view *const);
string_view *sv_trim_whitespace(string_view *const);

int atoi(const char[static 1]);
unsigned int atou(const char[static 1]);
long int atol(const char[static 1]);
long long int atoll(const char[static 1]);
float atof(const char[static 1]);
double atod(const char[static 1]);

// cstr_to_X
double cstr_to_d(const char *const, const char *const *const);
int cstr_to_i(const char *const, const char *const *const);

size_t cstr_len(const char *const restrict);
const char *cstr_ncpy(const size_t n, const char[static n], char[static n]);
int cstr_ncmp(const size_t n, const char[static n], const char[static n]);

#endif // _JCSTRC_H_
