#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.c"  // Include lexer for token types

typedef struct ASTNode {
    Token token;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* next;
} ASTNode;

typedef struct {
    int line;
    int column;
    char message[256];
} ParserError;

void reportParserError(ParserError* error) {
    if (error != NULL) {
        printf("Parser Error [Line %d, Column %d]: %s\n", error->line, error->column, error->message);
    }
}

ASTNode* createNode(Token token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    return node;
}

ASTNode* parseExpression(Token** tokens, ParserError* error);
ASTNode* parseStatement(Token** tokens, ParserError* error);
ASTNode* parseBlock(Token** tokens, ParserError* error);

ASTNode* parse(Token* tokens) {
    ParserError error = {0, 0, ""};
    return parseBlock(&tokens, &error);
}

ASTNode* parseBlock(Token** tokens, ParserError* error) {
    ASTNode* head = NULL;
    ASTNode* current = NULL;

    while ((*tokens)->type != TOKEN_EOF && (*tokens)->type != TOKEN_SYMBOL && strcmp((*tokens)->value, "}") != 0) {
        ASTNode* stmt = parseStatement(tokens, error);
        if (stmt != NULL) {
            if (head == NULL) {
                head = stmt;
            } else {
                current->next = stmt;
            }
            current = stmt;
        }
    }

    if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "}") == 0) {
        (*tokens)++;
    } else {
        snprintf(error->message, sizeof(error->message), "Expected '}' at end of block");
        reportParserError(error);
    }

    return head;
}

ASTNode* parseStatement(Token** tokens, ParserError* error) {
    Token token = **tokens;

    if (token.type == TOKEN_IDENTIFIER) {
        // Parse assignment or function call
        (*tokens)++;
        if ((*tokens)->type == TOKEN_OPERATOR && strcmp((*tokens)->value, "=") == 0) {
            // Assignment
            (*tokens)++;
            ASTNode* expr = parseExpression(tokens, error);
            ASTNode* assignNode = createNode(token);
            assignNode->right = expr;
            return assignNode;
        } else {
            snprintf(error->message, sizeof(error->message), "Expected '=' after identifier");
            reportParserError(error);
            return NULL;
        }
    } else if (token.type == TOKEN_KEYWORD) {
        if (strcmp(token.value, "if") == 0) {
            // Parse if statement
            (*tokens)++;
            ASTNode* cond = parseExpression(tokens, error);
            ASTNode* thenBranch = parseBlock(tokens, error);
            ASTNode* ifNode = createNode(token);
            ifNode->left = cond;
            ifNode->right = thenBranch;

            // Check for else branch
            if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "else") == 0) {
                (*tokens)++;
                ASTNode* elseBranch = parseBlock(tokens, error);
                ifNode->next = elseBranch;
            }

            return ifNode;
        } else if (strcmp(token.value, "while") == 0) {
            // Parse while loop
            (*tokens)++;
            ASTNode* cond = parseExpression(tokens, error);
            ASTNode* body = parseBlock(tokens, error);
            ASTNode* whileNode = createNode(token);
            whileNode->left = cond;
            whileNode->right = body;
            return whileNode;
        }
    }

    snprintf(error->message, sizeof(error->message), "Unexpected token '%s'", token.value);
    reportParserError(error);
    return NULL;  // Error case
}

ASTNode* parseExpression(Token** tokens, ParserError* error) {
    ASTNode* node = createNode(**tokens);
    (*tokens)++;
    return node;
}

void printAST(ASTNode* node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s\n", node->token.value);

    printAST(node->left, depth + 1);
    printAST(node->right, depth + 1);
    printAST(node->next, depth);
}
