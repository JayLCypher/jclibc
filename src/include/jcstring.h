#ifndef _JCSTRING_H
#define _JCSTRING_H

#if __STDC_VERSION__ < 202311L
#define bool _Bool
#define true !0
#define false 0
#endif

int atoi(const char * const restrict aString);
unsigned int atou(const char * const restrict aString);
long int atol(const char * const restrict aString);
long long int atoll(const char * const restrict aString);
float atof(const char * const restrict aString);
double atod(const char * const restrict aString);
double strtod(const char * const aString, char ** const end_ptr);
int strtoi(const char * const aString, const char * const * const end_ptr);

#endif
