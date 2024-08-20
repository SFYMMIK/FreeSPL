#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_SIZE 100

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_SYMBOL,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_SIZE];
} Token;

typedef struct {
    int line;
    int column;
    char message[256];
} LexerError;

const char* keywords[] = {"if", "else", "while", "for", "return", "int", "float", "void"};

int isKeyword(const char* str) {
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void reportLexerError(LexerError* error) {
    if (error != NULL) {
        printf("Lexer Error [Line %d, Column %d]: %s\n", error->line, error->column, error->message);
    }
}

void lex(const char* input) {
    const char* p = input;
    Token token;
    LexerError error = {0, 0, ""};
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
            p++;  // Skip whitespace
            continue;
        }

        if (isalpha(*p)) {
            // Identifier or keyword
            int len = 0;
            while (isalnum(*p) && len < MAX_TOKEN_SIZE - 1) {
                token.value[len++] = *p++;
                column++;
            }
            token.value[len] = '\0';
            if (isKeyword(token.value)) {
                token.type = TOKEN_KEYWORD;
                printf("Token: Keyword (%s)\n", token.value);
            } else {
                token.type = TOKEN_IDENTIFIER;
                printf("Token: Identifier (%s)\n", token.value);
            }
        } else if (isdigit(*p)) {
            // Number
            int len = 0;
            while (isdigit(*p) && len < MAX_TOKEN_SIZE - 1) {
                token.value[len++] = *p++;
                column++;
            }
            token.value[len] = '\0';
            token.type = TOKEN_NUMBER;
            printf("Token: Number (%s)\n", token.value);
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '=' || *p == '!' || *p == '<' || *p == '>') {
            // Operator
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
            printf("Token: Operator (%s)\n", token.value);
        } else if (*p == '(' || *p == ')' || *p == '{' || *p == '}' || *p == ';') {
            // Symbols
            token.value[0] = *p++;
            token.value[1] = '\0';
            token.type = TOKEN_SYMBOL;
            printf("Token: Symbol (%s)\n", token.value);
            column++;
        } else {
            // Unknown character
            snprintf(error.message, sizeof(error.message), "Unexpected character '%c'", *p);
            error.line = line;
            error.column = column;
            reportLexerError(&error);
            p++;
            column++;
        }
    }

    // End of file token
    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    printf("Token: EOF\n");
}
