#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef enum {
    AST_UNKNOWN,
    AST_VAR_ASSIGN,
    AST_FUNC_DEF,
    AST_RETURN,
    AST_WHILE_LOOP,
    AST_IF_STATEMENT,
    AST_EXPRESSION,
    AST_PRINT,
    AST_INPUT,
    AST_LOOP,     // <-- added
    AST_BREAK,     // <-- added
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType nodeType;
    Token       token;   // stores the “main” token for this node (operator, keyword, etc.)
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* body;   // for blocks (func, if, while)
    struct ASTNode* next;   // next statement in the same block
} ASTNode;

typedef struct {
    int line;
    int column;
    char message[128];
} ParserError;

// Entry point: parse a top‐level block and return the head of a statement list
ASTNode* parse(Token* tokens);

// Utility to print an AST (unchanged from before)
void printAST(ASTNode* node, int depth);

// (These are now implemented in parser.c — you don’t need to call them from outside)
ASTNode* parseBlock(Token** tokens, ParserError* error);
ASTNode* parseStatement(Token** tokens, ParserError* error);
ASTNode* parseExpression(Token** tokens, ParserError* error);

#endif // PARSER_H