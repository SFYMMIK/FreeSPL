#include "lexer.h"
#include "error_handling.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_SIZE 100
#define MAX_TOKENS 1024

const char* keywords[] = {"if", "else", "while", "for", "return", "int", "float", "void"};

int isKeyword(const char* str) {
    for (int i = 0; i < (int)(sizeof(keywords)/sizeof(keywords[0])); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

Token* lex(const char* input, int* token_count) {
    static Token tokens[MAX_TOKENS];
    int count = 0;

    const char* p = input;
    int line = 1;
    int column = 1;

    while (*p != '\0') {
        if (isspace(*p)) {
            if (*p == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            p++;
            continue;
        }

        Token token;

        if (isalpha(*p)) {
            int len = 0;
            while (isalnum(*p) && len < MAX_TOKEN_SIZE - 1) {
                token.value[len++] = *p++;
                column++;
            }
            token.value[len] = '\0';
            if (isKeyword(token.value)) {
                token.type = TOKEN_KEYWORD;
            } else {
                token.type = TOKEN_IDENTIFIER;
            }
        } else if (isdigit(*p)) {
            int len = 0;
            while (isdigit(*p) && len < MAX_TOKEN_SIZE - 1) {
                token.value[len++] = *p++;
                column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_NUMBER;
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=' || *p == '!' || *p == '<' || *p == '>') {
            int len = 0;
            while ((*p == '=' || *p == '!' || *p == '<' || *p == '>') && len < MAX_TOKEN_SIZE - 1) {
                token.value[len++] = *p++;
                column++;
            }
            if (len == 0) {
                token.value[len++] = *p++;
                column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_OPERATOR;
        } else if (*p == '(' || *p == ')' || *p == '{' || *p == '}' || *p == ';' || *p == ',') {
            token.value[0] = *p++;
            token.value[1] = '\0';
            token.type = TOKEN_SYMBOL;
            column++;
        } else {
            LexerError error;
            snprintf(error.message, sizeof(error.message), "Unexpected character '%c'", *p);
            error.line = line;
            error.column = column;
            reportLexerError(&error);
            p++;
            column++;
            continue;
        }

        tokens[count++] = token;
    }

    // Add EOF token
    Token eof_token;
    eof_token.type = TOKEN_EOF;
    strcpy(eof_token.value, "EOF");
    tokens[count++] = eof_token;

    *token_count = count;
    return tokens;
}
