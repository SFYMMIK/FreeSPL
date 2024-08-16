#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to duplicate a string
char* string_duplicate(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char* new_str = (char*)malloc(len + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

// Function to convert an integer to a string
char* int_to_string(int num) {
    char buffer[12];  // Enough to hold a 32-bit integer
    snprintf(buffer, sizeof(buffer), "%d", num);
    return string_duplicate(buffer);
}

// Function to concatenate two strings
char* string_concat(const char* str1, const char* str2) {
    if (!str1 || !str2) return NULL;
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1);
    if (result) {
        strcpy(result, str1);
        strcat(result, str2);
    }
    return result;
}

// Function to safely allocate memory
void* safe_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Function to safely reallocate memory
void* safe_realloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    if (!new_ptr) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}
