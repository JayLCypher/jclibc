#pragma once
#ifndef __STDMACRO_H
#define __STDMACRO_H

// Dereference a pointer to type T if it exists, else set to 0 ptr
#define DEREF(T, ptr) ptr ? *(T*)ptr : (void*)0

// Use sizeof operators to find the size of a LOCAL array. 
#define ARRAY_SIZE(arr) sizeof (arr) / sizeof (*arr)

#endif
