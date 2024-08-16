#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_OPEN,
    TOKEN_WRITE,
    TOKEN_CFILE,
    TOKEN_DFILE,
    TOKEN_ERRORHANDL,
    TOKEN_LOGHANDL,
    TOKEN_GUI,
    TOKEN_GET,
    TOKEN_POST,
    TOKEN_PUT,
    TOKEN_DELETE,
    TOKEN_FUNC,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_LOOP,
    TOKEN_TRY,
    TOKEN_EXCEPT,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

Token* lexer(const char* source);

#endif
