#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_STRING,
    TOKEN_IMPORT,
    TOKEN_IMPORT_FROM_C,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
} Token;

#endif
