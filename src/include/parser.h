#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct Node {
    Token token;
    struct Node* left;
    struct Node* right;
} Node;

Node* parse(Token* tokens);

#endif
