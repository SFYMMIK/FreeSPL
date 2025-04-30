#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_SYMBOL,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    TOKEN_IMPORT,
    TOKEN_IMPORT_FROM_C,
    TOKEN_STRING
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
} Token;

#endif
