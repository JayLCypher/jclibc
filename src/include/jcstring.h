// LINT_C_FILE
#pragma once
#ifndef _JCSTRING_H
#define _JCSTRING_H

#include <stddef.h>

#if __STDC_VERSION__ < 202311L
#include <stdbool.h>
#include "./jcstddef.h"
#endif

constexpr char whitespace[] = { ' ', '\n', '\t', '\0', '\v', '\f', '\r' };
constexpr size_t whitespace_sz = sizeof(whitespace);

struct string_view {
	size_t count;
	char *s;
};
typedef struct string_view string_view;

string_view *const sv_trim_whitespace_left(string_view *const);
string_view *const sv_trim_whitespace_right(string_view *const);
string_view *const sv_trim_whitespace(string_view *const);

int atoi(const char[restrict static 1]);
unsigned int atou(const char *const restrict);
long int atol(const char *const restrict);
long long int atoll(const char *const restrict);
float atof(const char *const restrict);
double atod(const char *const restrict);
double strtod(const char *const, char ** const);
int strtoi(const char *const, const char *const *const);

size_t strlen(const char *const restrict);

#endif
