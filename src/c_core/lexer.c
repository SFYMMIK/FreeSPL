#include "lexer.h"
#include "error_handling.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_SIZE 100
#define MAX_TOKENS 1024

const char* keywords[] = {
    "if", "else", "while", "for", "return", "int", "float", "void",
    "func", "print", "input", "break", "loop"
};

int isKeyword(const char* str) {
    for (int i = 0; i < (int)(sizeof(keywords) / sizeof(keywords[0])); i++) {
        if (strcmp(str, keywords[i]) == 0) return 1;
    }
    return 0;
}

Token* lex(const char* input, int* token_count) {
    static Token tokens[MAX_TOKENS];
    int count = 0;
    const char* p = input;

    while (*p != '\0') {
        if (*p == '/' && *(p+1) == '/') {
            while (*p && *p != '\n') p++;
            continue;
        }

        if (isspace(*p)) {
            p++; continue;
        }

        Token token;

        if (*p == '"') {
            p++; int len = 0;
            while (*p && *p != '"' && len < MAX_TOKEN_SIZE - 1)
                token.value[len++] = *p++;
            token.value[len] = '\0';
            token.type = TOKEN_STRING;
            if (*p == '"') p++;
            tokens[count++] = token;
            continue;
        }

        if (isalpha(*p)) {
            int len = 0;
            while (isalnum(*p) && len < MAX_TOKEN_SIZE - 1)
                token.value[len++] = *p++;
            token.value[len] = '\0';
            token.type = isKeyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        } else if (isdigit(*p)) {
            int len = 0;
            while (isdigit(*p) && len < MAX_TOKEN_SIZE - 1)
                token.value[len++] = *p++;
            token.value[len] = '\0';
            token.type = TOKEN_NUMBER;
        } else if (strchr("+-*/=!<>", *p)) {
            token.value[0] = *p++;
            token.value[1] = '\0';
            token.type = TOKEN_OPERATOR;
        } else if (strchr("(){};,", *p)) {
            token.value[0] = *p++;
            token.value[1] = '\0';
            token.type = TOKEN_SYMBOL;
        } else {
            p++;
            continue;
        }

        tokens[count++] = token;
    }

    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strcpy(eof_token.value, "EOF");
    tokens[count++] = eof_token;

    *token_count = count;
    return tokens;
}
