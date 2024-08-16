#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Token create_token(TokenType type, const char* value) {
    Token token;
    token.type = type;
    token.value = strdup(value);
    return token;
}

Token* lexer(const char* source) {
    size_t length = strlen(source);
    Token* tokens = malloc(sizeof(Token) * length);
    int index = 0;

    for (int i = 0; i < length; i++) {
        if (isalpha(source[i])) {
            char buffer[128];
            int j = 0;
            while (isalnum(source[i]) || source[i] == '_') {
                buffer[j++] = source[i++];
            }
            buffer[j] = '\0';
            i--;
            tokens[index++] = create_token(TOKEN_IDENTIFIER, buffer);
        } else if (source[i] == '+') {
            tokens[index++] = create_token(TOKEN_PLUS, "+");
        } else if (isspace(source[i])) {
            continue;
        } else {
            tokens[index++] = create_token(TOKEN_EOF, "EOF");
        }
    }

    return tokens;
}
