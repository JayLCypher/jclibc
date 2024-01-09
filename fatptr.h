//LINT_C_FILE
#pragma once
#ifndef _FAT_PTR_H_
#define _FAT_PTR_H_

#include <stddef.h>

typedef struct _fat_ptr { size_t size; void *p; } fat_p;

#endif // _FAT_PTR_H_
