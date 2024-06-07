// LINT_C_FILE
#include "./jcstrc.h"

static inline bool is_negative(const char c) { return (c == '-'); }
static inline bool is_positive(const char c) { return (c == '+'); }

// Space functions
static inline bool is_tab(const char c) { return (c == '\t'); }
static inline bool is_space(const char c) { return (c == ' '); }

static inline size_t count_space(const char s[static 1]) {
	size_t res = 0;
	while (*s && is_space(*s)) {
		++res;
		++s;
	}
	return res;
}

static inline const char *skip_space(const char s[static 1]) {
	while (*s && is_space(*s)) {
		++s;
	}
	return s;
}

static inline const char *skip_space_r(const char s[static 1],
									   const char *end) {
	while ((end != s) && is_space(*end)) {
		end--;
	}
	return end;
}

// Whitespace, which includes space, tab, null, carriage etc.
static inline bool is_whitespace(const char c) {
	for (size_t i = 0; i < whitespace_sz; ++i) {
		if (c == whitespace[i]) {
			return true;
		}
	}
	return false;
}

static inline unsigned int count_whitespace(const char s[static 1]) {
	unsigned int i = 0;
	while (s[i] && is_whitespace(s[i])) {
		++i;
	}
	return i;
}

static inline const char *skip_whitespace(const char s[static 1]) {
	while (*s && is_whitespace(*s)) {
		++s;
	}
	return s;
}

// Assume that most likely the next ascii character is a normal character and
// not control to short-stroke.
static inline bool is_ascii_binary(const char c) {
	return !(c > '1' || c < '0');
}
static inline bool is_ascii_ternary(const char c) {
	return !(c > '2' || c < '0');
}
static inline bool is_ascii_quaternary(const char c) {
	return !(c > '3' || c < '0');
}
static inline bool is_ascii_quinary(const char c) {
	return !(c > '4' || c < '0');
}
static inline bool is_ascii_senary(const char c) {
	return !(c > '5' || c < '0');
}
static inline bool is_ascii_septenary(const char c) {
	return !(c > '6' || c < '0');
}
static inline bool is_ascii_octal(const char c) {
	return !(c > '7' || c < '0');
}
static inline bool is_ascii_nonary(const char c) {
	return !(c > '8' || c < '0');
}
static inline bool is_ascii_dec(const char c) { return !(c > '9' || c < '0'); }
static inline bool is_ascii_hex(const char c) {
	return !(is_ascii_dec(c) || (c > 'F' || c < 'A') || (c > 'f' || c < 'a'));
}
static inline bool is_ascii_vig(const char c) {
	return !(is_ascii_dec(c) || (c > 'K' || c < 'A') || (c > 'k' || c < 'a'));
}

typedef struct _fixed_string fixed_string;
struct _fixed_string {
	size_t len;
	const char s[sizeof(char *)];
};
static_assert(sizeof(fixed_string) == 16, "Wrong size of fixed string!");

typedef struct _multi_string s;
struct _multi_string {
	size_t count;
	const char *s[];
};

typedef union {
	char ss[sizeof(char *) + sizeof(size_t)];
	struct {
		size_t len;
		union {
			char sf[sizeof(char *)];
			char *s;
		};
	};
} stdstring;
static_assert(sizeof(stdstring) == 16, "stdstring wrong size.");

static string *string_init(string *s, const size_t len, char *cstr) {
	if (s) {
		*s = (string){len, cstr};
	}
	return s;
}

string string_from_cstr(char s[static 1]) {
	size_t s_len = cstr_len(s);
	string jcstr = {.len = s_len, .s = s};
	return jcstr;
}

static string string_from_sv(string_view *sv) {
	string s = {0};
	s.len = sv->count;
	cstr_ncpy(s.len, sv->s, s.s);
	return s;
}

static string string_copy(const string *s) {
	return *string_init(&(string){0}, s->len, s->s);
}

string *string_trim_whitespace_left(string *s) {
	while (*s->s++ && is_space(*s->s)) {
		s->len--;
	}
	return s;
}

string *string_trim_whitespace_right(string *s) {
	while (*s->s + s->len && is_space(*s->s + s->len)) {
		s->len--;
	}
	return s;
}

string *string_trim_whitespace(string *s) {
	return string_trim_whitespace_right(string_trim_whitespace_left(s));
}

// string *string_format(string *s, const char *restrict fmt, va_list *args) {
// 	if (snprintf(s->s, s->len, fmt, args) < 0) {
// 		return nullptr;
// 	}
// 	return s;
// }

// STRING_VIEW STUFF
inline string_view sv_new(const size_t count, const char s[static count]) {
	return (string_view){.count = count, .s = s};
}

// Count spaces on the left of string view and return a new sliced view copy.
string_view sv_trim_whitespace_left(const string_view *sv) {
	size_t count = count_space(sv->s);
	return (string_view){sv->count - count, sv->s + count};
}

string_view sv_trim_whitespace_right(const string_view *sv) {
	return (string_view){
		sv->count - (skip_space_r(sv->s, sv->s + sv->count) - sv->s), sv->s};
}

string_view sv_trim_whitespace(const string_view *sv) {
	string_view s_l = sv_trim_whitespace_left(sv);
	string_view s_r = sv_trim_whitespace_right(sv);
	return (string_view){s_l.s - s_r.s, s_l.s};
}

bool sv_eq(const string_view *sv_a, const string_view *sv_b) {
	if (sv_a->count != sv_b->count) {
		return false;
	}
	const char *a = sv_a->s, *b = sv_b->s;
	for (; *a && *b; ++a, ++b) {
		if (*a != *b) {
			return false;
		}
	}
	return true;
}

void sv_print(const string_view *sv) {
	printf("%.*s\n%zu\n", (int)sv->count, sv->s, sv->count);
}

#define ATO_OFFSET 0xF
#define ato_r(res, c, base)                   \
	do {                                      \
		res = (res * base) + (c & ATO_OFFSET) \
	} while (0)

int atoi(const char s[static 1]) {
	if ((s = skip_space(s)) == nullptr) {
		return 0;
	}
	bool is_negative = (*s == '-');
	if (*(s += is_negative) == '+') {
		++s;
	}
	int result = 0;
	for (; *s && is_ascii_dec(*s); ++s) {
		result = (result * 10) + (*s & ATO_OFFSET);
	}
	return is_negative ? -result : result;
}

unsigned atou(const char s[static 1]) {
	if ((s = skip_space(s)) == nullptr) {
		return 0;
	}
	if (*s == '+' || *s == '-') {
		++s;
	}
	unsigned result = 0;
	for (; *s && is_ascii_dec(*s); ++s) {
		result = (result * 10) + (*s & ATO_OFFSET);
	}
	return result;
}

long atol(const char s[static 1]) {
	if ((s = skip_space(s)) == nullptr) {
		return 0;
	}
	bool is_negative = (*s == '-');
	if (*(s += is_negative) == '+') {
		++s;
	}
	long result = 0;
	for (; *s && is_ascii_dec(*s); ++s) {
		result = (result * 10) + (*s & ATO_OFFSET);
	}
	return is_negative ? -result : result;
}

long long atoll(const char s[static 1]) {
	if ((s = skip_space(s)) == nullptr) {
		return 0;
	}
	bool is_negative = (*s == '-');
	if (*(s += is_negative) == '+') {
		++s;
	}
	long long result = 0;
	for (; *s && is_ascii_dec(*s); s++) {
		result = (result * 10) + (*s & ATO_OFFSET);
	}
	return is_negative ? -result : result;
}

float atof(const char s[static 1]) {
	if ((s = skip_space(s)) == nullptr) {
		return 0.0f;
	}
	bool is_negative = (*s == '-');
	if (*(s += is_negative) == '+') {
		++s;
	}
	size_t pre_dot = 0;
	for (; *s && is_ascii_dec(*s) && *s != '.'; ++s) {
		pre_dot = (pre_dot * 10) + (*s & ATO_OFFSET);
	}
	float result = 0.0f;
	if (*s == '.') {
		size_t i = 0, post_dot = 0;
		for (++s; *s && is_ascii_dec(*s); ++i, ++s) {
			post_dot = (post_dot * 10) + (*s & ATO_OFFSET);
		}
		result = post_dot;
		for (; i > 0; --i) {
			result /= 10;
		}
	}
	result += pre_dot;
	return is_negative ? -result : result;
}

double atod(const char s[static 1]) {
	if ((s = skip_space(s)) == nullptr) {
		return 0.0f;
	}
	bool is_negative = (*s == '-');
	if (*(s += is_negative) == '+') {
		++s;
	}
	size_t pre_dot = 0;
	for (; *s && is_ascii_dec(*s) && *s != '.'; ++s) {
		pre_dot = (pre_dot * 10) + (*s & ATO_OFFSET);
	}
	double result = 0.0f;
	if (*s == '.') {
		size_t i = 0, post_dot = 0;
		for (++s; *s && is_ascii_dec(*s); ++i, ++s) {
			post_dot = (post_dot * 10) + (*s & ATO_OFFSET);
		}
		result = post_dot;
		for (; i > 0; --i) {
			result /= 10;
		}
	}
	result += pre_dot;
	return is_negative ? -result : result;
}

static inline unsigned int check_prefix(const char s[static 2]) {
	if (s[0] == '0') {
		switch (s[1]) {
			case 'x':
#if __STDC__VERSION >= 202311L
			[[fallthrough]]
#endif
			case 'X':
				return 16;
			case 'b':
				return 2;
				default : return 8;
		}
	} else if (is_ascii_dec(s[0])) {
		return 10;
	}
	return 0;
}

int cstr_to_i(const char s[static 2], const char **end_ptr, unsigned base) {
	if ((s = skip_space(s)) == nullptr) {
		return 0;
	}  // Skip whitespace
	if (base == 0) {
		base = check_prefix(s);
		switch (base) {
			case 16:
#if __STDC__VERSION >= 202311L
			[[fallthrough]]
#endif
			case 2:
				s += 2;
				break;
			case 8:
				s++;
				break;
			default:
				break;
		}
		if (base == 0) {
			return 0;
		}
	}
	bool is_negative = (*s == '-');
	if (*(s += is_negative) == '+') {
		s++;
	}  // Bypass - and +
	int result = 0;
	for (; *s && is_ascii_dec(*s); ++s) {
		result = (result * base) + (*s & 0xF);
	}
	if (*(s + 1)) {
		*end_ptr = s;
	}
	return is_negative ? -result : result;
}

double cstr_to_d(const char s[static 2], const char **end_ptr, unsigned base) {
	s = skip_space(s);
	double res = 0.0;
	if (base == 0) {
		base = check_prefix(s);
		switch (base) {
			case 16:
#if __STDC__VERSION >= 202311L
			[[fallthrough]]
#endif
			case 2:
				s += 2;
				break;
			case 8:
				s++;
				break;
			default:
				break;
		}
		if (base == 0) {
			return 0;
		}
	}
	return res;
}
static inline size_t cstr_unrolled_8_byte_len_check(
	const unsigned long long lw_p[static 1]) {
	// Typesafe implicit casts.
	const void *p = lw_p;
	const char *c_p = p;
	if (c_p[0] == '\0') {
		return 0;
	} else if (c_p[1] == '\0') {
		return 1;
	} else if (c_p[2] == '\0') {
		return 2;
	} else if (c_p[3] == '\0') {
		return 3;
	}
	if (sizeof *lw_p > 4) {
		if (c_p[4] == '\0') {
			return 4;
		} else if (c_p[5] == '\0') {
			return 5;
		} else if (c_p[6] == '\0') {
			return 6;
		} else if (c_p[7] == '\0') {
			return 7;
		}
	}
	return 8;  // ERROR;
}
// Get an end pointer to NULL-terminated c style string.
const char *cstr_end(const char s[static 1]) {
	register const char *c_p = s;

	while (*c_p++ && ((sizeof c_p & (sizeof(unsigned long long) - 1)) != 0))
		continue;

	register unsigned long long magic_bits = 0x7efefeffLL;
	register unsigned long long hi_magic = 0x80808080LL;
	register unsigned long long lo_magic = 0x01010101LL;
	return c_p;
}

inline size_t cstr_len(const char s[static 1]) { return cstr_end(s) - s - 1; }

// Copy no more than N characters from s to buf. Make sure buf can hold len.
const char *cstr_ncpy(const size_t n, const char s_in[static n],
					  char buf_out[static n]) {
	assert(s_in && buf_out);
	for (size_t i = 0; i < n; ++i) {
		buf_out[i] = s_in[i];
	}
	return buf_out;
}

int cstr_ncmp(const size_t n, const char s1[static n],
			  const char s2[static n]) {
	for (size_t i = 0; i < n; ++i) {
		if (s1[i] != s2[i]) {
			return s1[i] - s2[i];
		}
	}
	return 0;
}

const char *cstr_chr(const char s[static 1], const char c) {
	while (*s++) {
		if (*s == c) {
			return s;
		}
	}
	return nullptr;
}

const char *cstr_rchr(const char s[static 1], const char c) {
	const char *p = nullptr;
	while (*s++) {
		if (*s == c) {
			p = s;
		}
	}
	return p;
}

const char *cstr_rchrr(const char s_start[static 1], const char *s_end,
					   const char c) {
	while (s_end-- != s_start) {
		if (*s_end == c) {
			return s_end;
		}
	}
	return nullptr;
}

static inline const char *_cstr_str_inner(const char *restrict hay,
										  const char *restrict const needle,
										  const char *const needle_end) {
	for (const char *p = hay, *q = needle; *hay; ++hay) {
		for (; *p == *q; ++p, ++q) {
		}  // Match characters
		if ((*hay == *needle) && ((hay - p - 1) == (needle - needle_end))) {
			return hay;
		}
	}
	return nullptr;
}

const char *cstr_str(const char *restrict hay,
					 const char *restrict const needle) {
	assert(hay && needle);
	for (; *hay != *needle; ++hay) {
	}  // Skip to first occurrance of character match
	const char *needle_end = needle;
	while (*needle_end++) {
	}  // We're going to need the endptr of needle
	return _cstr_str_inner(hay, needle, needle_end);
}

const char *cstr_nstr(const char *restrict hay, const size_t needle_len,
					  const char *restrict const needle) {
	assert(hay && needle);
	for (; *hay != *needle; ++hay) {
	}  // Skip to first occurrance of character match
	return _cstr_str_inner(hay, needle, needle + needle_len);  // No match
}
