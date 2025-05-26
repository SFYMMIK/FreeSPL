#include "parser.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* parseBlock(Token** tokens, ParserError* error);

ASTNode* createNode(ASTNodeType nodeType, Token token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->nodeType = nodeType;
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    node->body = NULL;
    return node;
}

void reportParserError(ParserError* error) {
    if (error && strlen(error->message) > 0) {
        printf("Parser Error [Line %d, Column %d]: %s\n", error->line, error->column, error->message);
    }
}

ASTNode* parseExpression(Token** tokens, ParserError* error) {
    if ((*tokens)->type == TOKEN_STRING || (*tokens)->type == TOKEN_NUMBER || (*tokens)->type == TOKEN_IDENTIFIER) {
        ASTNode* left = createNode(AST_EXPRESSION, **tokens);
        (*tokens)++;

        // Obsługa operatorów: np. x + 1
        if ((*tokens)->type == TOKEN_OPERATOR) {
            Token op = **tokens;
            (*tokens)++;
            ASTNode* right = parseExpression(tokens, error);
            ASTNode* opNode = createNode(AST_EXPRESSION, op);
            opNode->left = left;
            opNode->right = right;
            return opNode;
        }

        return left;
    }

    if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "(") == 0) {
        (*tokens)++; // skip '('
        ASTNode* inner = parseExpression(tokens, error);
        if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ")") == 0) {
            (*tokens)++; // skip ')'
            return inner;
        } else {
            snprintf(error->message, sizeof(error->message), "Expected ')' after expression");
            return inner;
        }
    }

    snprintf(error->message, sizeof(error->message), "Invalid expression starting with '%s'", (*tokens)->value);
    return NULL;
}

ASTNode* parseStatement(Token** tokens, ParserError* error) {
    while ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
        (*tokens)++;
    }

    Token token = **tokens;

    if (token.type == TOKEN_KEYWORD) {
        if (strcmp(token.value, "func") == 0) {
            (*tokens)++;
            Token funcName = **tokens;
            (*tokens)++; // name
            (*tokens)++; // (
            (*tokens)++; // )
            (*tokens)++; // {
            ASTNode* node = createNode(AST_FUNC_DEF, funcName);
            node->body = parseBlock(tokens, error);
            return node;
        }

        if (strcmp(token.value, "print") == 0) {
            (*tokens)++;
            ASTNode* expr = parseExpression(tokens, error);
            if (!expr) return NULL;
            ASTNode* node = createNode(AST_PRINT, token);
            node->left = expr;
            return node;
        }

        if (strcmp(token.value, "input") == 0) {
            (*tokens)++;
            ASTNode* expr = parseExpression(tokens, error);
            if (!expr) return NULL;
            ASTNode* node = createNode(AST_INPUT, token);
            node->left = expr;
            return node;
        }

        if (strcmp(token.value, "return") == 0) {
            (*tokens)++;
            ASTNode* node = createNode(AST_RETURN, token);
            node->left = parseExpression(tokens, error);
            return node;
        }

        if (strcmp(token.value, "if") == 0) {
            (*tokens)++;
            ASTNode* cond = parseExpression(tokens, error);
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "{") == 0) {
                (*tokens)++;
                ASTNode* body = parseBlock(tokens, error);
                ASTNode* ifNode = createNode(AST_IF_STATEMENT, token);
                ifNode->left = cond;
                ifNode->body = body;
                return ifNode;
            } else {
                snprintf(error->message, sizeof(error->message), "Expected '{' after if condition");
                return NULL;
            }
        }

        if (strcmp(token.value, "while") == 0) {
            (*tokens)++;
            ASTNode* cond = parseExpression(tokens, error);
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "{") == 0) {
                (*tokens)++;
                ASTNode* body = parseBlock(tokens, error);
                ASTNode* whileNode = createNode(AST_WHILE_LOOP, token);
                whileNode->left = cond;
                whileNode->body = body;
                return whileNode;
            } else {
                snprintf(error->message, sizeof(error->message), "Expected '{' after while condition");
                return NULL;
            }
        }
    }

    if (token.type == TOKEN_IDENTIFIER) {
        (*tokens)++;
        if ((*tokens)->type == TOKEN_OPERATOR && strcmp((*tokens)->value, "=") == 0) {
            (*tokens)++;
            ASTNode* right = parseExpression(tokens, error);
            ASTNode* node = createNode(AST_VAR_ASSIGN, token);
            node->right = right;
            return node;
        } else {
            snprintf(error->message, sizeof(error->message), "Expected '=' after identifier");
            return NULL;
        }
    }

    snprintf(error->message, sizeof(error->message), "Unexpected token '%s'", token.value);
    return NULL;
}

ASTNode* parseBlock(Token** tokens, ParserError* error) {
    ASTNode* head = NULL;
    ASTNode* current = NULL;
    ASTNode* lastIf = NULL;

    while ((*tokens)->type != TOKEN_EOF && !((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "}") == 0)) {
        if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "else") == 0) {
            if (lastIf && lastIf->nodeType == AST_IF_STATEMENT) {
                (*tokens)++; // skip 'else'
                if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "{") == 0) {
                    (*tokens)++;
                    ASTNode* elseBlock = parseBlock(tokens, error);
                    lastIf->right = elseBlock;
                    continue;
                } else {
                    snprintf(error->message, sizeof(error->message), "Expected '{' after 'else'");
                    return NULL;
                }
            } else {
                snprintf(error->message, sizeof(error->message), "Unexpected 'else' without matching 'if'");
                return NULL;
            }
        }

        ASTNode* stmt = parseStatement(tokens, error);
        if (!stmt) return NULL;

        if (!head) head = stmt;
        else current->next = stmt;
        current = stmt;

        if (stmt->nodeType == AST_IF_STATEMENT) lastIf = stmt;
        else lastIf = NULL;
    }

    if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "}") == 0) {
        (*tokens)++;
    }

    return head;
}

ASTNode* parse(Token* tokens) {
    ParserError error = {0, 0, ""};
    ASTNode* result = parseBlock(&tokens, &error);
    if (strlen(error.message) > 0) reportParserError(&error);
    return result;
}

void printAST(ASTNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) printf("  ");
    printf("%s\n", node->token.value);
    printAST(node->left, depth + 1);
    printAST(node->right, depth + 1);
    printAST(node->body, depth + 1);
    printAST(node->next, depth);
}
