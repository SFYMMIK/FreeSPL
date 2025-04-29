#include "error_handling.h"
#include <stdio.h>

void reportLexerError(LexerError* error) {
    if (error != NULL) {
        printf("Lexer Error [Line %d, Column %d]: %s\n", error->line, error->column, error->message);
    }
}
