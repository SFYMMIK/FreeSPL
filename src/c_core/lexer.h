#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "error_handling.h"

Token* lex(const char* input, int* token_count);

#endif // LEXER_H
