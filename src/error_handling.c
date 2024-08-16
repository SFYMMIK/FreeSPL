#include "include/error_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void error_handl(const char* error_type, const char* error_message, const char* fix_suggestion) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s.errorFile", error_type);
    FILE* file = fopen(filename, "w");
    if (file) {
        time_t now = time(NULL);
        char* timestamp = ctime(&now);
        timestamp[strlen(timestamp) - 1] = '\0';
        fprintf(file, "[%s] Error: %s\nSuggested Fix: %s\n", timestamp, error_message, fix_suggestion);
        fclose(file);
    }
}
