#include "error_handling.h"
#include <stdio.h>
#include <stdlib.h>

void handle_error(const char *error_message) {
    fprintf(stderr, "Runtime Error: %s\n", error_message);
    exit(EXIT_FAILURE);
}
