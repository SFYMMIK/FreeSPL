#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Function to duplicate a string
char* string_duplicate(const char* str);

// Function to convert an integer to a string
char* int_to_string(int num);

// Function to concatenate two strings
char* string_concat(const char* str1, const char* str2);

// Function to safely allocate memory
void* safe_malloc(size_t size);

// Function to safely reallocate memory
void* safe_realloc(void* ptr, size_t size);

#endif
