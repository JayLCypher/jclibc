// LINT_C_FILE
#pragma once
#ifndef _JCMEMC_H_
#define _JCMEMC_H_

const void *mem_chr(const void *const, const char, const size_t);
const void *mem_rchr(const void *const, const char, const size_t);
const void *mem_rchrr(const void *const, const void *const, const char);
const void *mem_rawchr(const void *const, const char);

#endif /* end of include guard: _JCMEMC_H_ */
