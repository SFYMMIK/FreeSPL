#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_SYMBOL,
    TOKEN_NUMBER,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
} Token;

#endif // TOKEN_H
