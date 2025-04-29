#ifndef PARSER_H
#define PARSER_H

#include "token.h"

// Define AST node types
typedef enum {
    AST_UNKNOWN,
    AST_VAR_ASSIGN,
    AST_FUNC_DEF,
    AST_RETURN,
    AST_WHILE_LOOP,
    AST_IF_STATEMENT,
    AST_EXPRESSION,
    AST_PRINT
} ASTNodeType;

// Parser error structure
typedef struct {
    int line;
    int column;
    char message[256];
} ParserError;

// AST Node structure
typedef struct ASTNode {
    ASTNodeType nodeType;
    Token token;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* next;
    struct ASTNode* body; // For blocks or function bodies
} ASTNode;

// Functions
ASTNode* parse(Token* tokens);
void printAST(ASTNode* node, int depth);

#endif // PARSER_H
