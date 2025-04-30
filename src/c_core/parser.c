#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "error_handling.h"
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
    if (error != NULL) {
        printf("Parser Error [Line %d, Column %d]: %s\n", error->line, error->column, error->message);
    }
}

ASTNode* parseExpression(Token** tokens, ParserError* error) {
    if ((*tokens)->type == TOKEN_EOF) {
        snprintf(error->message, sizeof(error->message), "Unexpected end of file while parsing expression");
        reportParserError(error);
        return NULL;
    }

    ASTNode* left = createNode(AST_EXPRESSION, **tokens);
    (*tokens)++;

    if ((*tokens)->type == TOKEN_OPERATOR) {
        Token op = **tokens;
        (*tokens)++;
        if ((*tokens)->type == TOKEN_EOF) {
            snprintf(error->message, sizeof(error->message), "Expected right-hand side of operator '%s'", op.value);
            reportParserError(error);
            return left;
        }
        ASTNode* right = createNode(AST_EXPRESSION, **tokens);
        (*tokens)++;

        ASTNode* opNode = createNode(AST_EXPRESSION, op);
        opNode->left = left;
        opNode->right = right;
        return opNode;
    }

    return left;
}

ASTNode* parseStatement(Token** tokens, ParserError* error) {
    // Skip unexpected loose operators/symbols
    while ((*tokens)->type == TOKEN_OPERATOR || (*tokens)->type == TOKEN_SYMBOL) {
        (*tokens)++;
    }

    Token token = **tokens;

    if (token.type == TOKEN_KEYWORD) {
        if (strcmp(token.value, "func") == 0) {
            (*tokens)++;
            Token funcName = **tokens;
            (*tokens)++;

            if (strcmp((*tokens)->value, "(") == 0) {
                (*tokens)++;
                while ((*tokens)->type != TOKEN_SYMBOL || strcmp((*tokens)->value, ")") != 0) {
                    (*tokens)++;
                }
                (*tokens)++;

                if (strcmp((*tokens)->value, "{") == 0) {
                    (*tokens)++;
                    ASTNode* funcNode = createNode(AST_FUNC_DEF, funcName);
                    funcNode->body = parseBlock(tokens, error);
                    return funcNode;
                } else {
                    snprintf(error->message, sizeof(error->message), "Expected '{' after function definition");
                    reportParserError(error);
                    return NULL;
                }
            }
        } else if (strcmp(token.value, "return") == 0) {
            (*tokens)++;
            ASTNode* returnNode = createNode(AST_RETURN, token);
            returnNode->left = parseExpression(tokens, error);
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
                (*tokens)++;
            }
            return returnNode;
        } else if (strcmp(token.value, "while") == 0) {
            (*tokens)++;
            ASTNode* cond = parseExpression(tokens, error);
            if (strcmp((*tokens)->value, "{") == 0) {
                (*tokens)++;
                ASTNode* body = parseBlock(tokens, error);
                ASTNode* whileNode = createNode(AST_WHILE_LOOP, token);
                whileNode->left = cond;
                whileNode->body = body;
                return whileNode;
            }
        } else if (strcmp(token.value, "if") == 0) {
            (*tokens)++;
            ASTNode* cond = parseExpression(tokens, error);
            if (strcmp((*tokens)->value, "{") == 0) {
                (*tokens)++;
                ASTNode* body = parseBlock(tokens, error);
                ASTNode* ifNode = createNode(AST_IF_STATEMENT, token);
                ifNode->left = cond;
                ifNode->body = body;
                return ifNode;
            }
        } else if (strcmp(token.value, "print") == 0) {
            (*tokens)++;
            ASTNode* expr = parseExpression(tokens, error);
            ASTNode* printNode = createNode(AST_PRINT, token);
            printNode->left = expr;
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
                (*tokens)++;
            }
            return printNode;
        } else if (strcmp(token.value, "input") == 0) {
            (*tokens)++;
            ASTNode* expr = createNode(AST_INPUT, token);
            expr->left = parseExpression(tokens, error); // the prompt
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
                (*tokens)++;
            }
            return expr;
        }
    }

    if (token.type == TOKEN_IDENTIFIER || token.type == TOKEN_NUMBER) {
        (*tokens)++;
        if (token.type == TOKEN_IDENTIFIER && (*tokens)->type == TOKEN_OPERATOR && strcmp((*tokens)->value, "=") == 0) {
            (*tokens)++;
            ASTNode* expr = parseExpression(tokens, error);
            ASTNode* assignNode = createNode(AST_VAR_ASSIGN, token);
            assignNode->right = expr;
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
                (*tokens)++;
            }
            return assignNode;
        } else if (token.type == TOKEN_NUMBER) {
            ASTNode* numberNode = createNode(AST_EXPRESSION, token);
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
                (*tokens)++;
            }
            return numberNode;
        } else {
            snprintf(error->message, sizeof(error->message), "Expected '=' after identifier");
            reportParserError(error);
            return NULL;
        }
    }

    snprintf(error->message, sizeof(error->message), "Unexpected token '%s'", token.value);
    reportParserError(error);
    return NULL;
}

ASTNode* parseBlock(Token** tokens, ParserError* error) {
    ASTNode* head = NULL;
    ASTNode* current = NULL;

    while ((*tokens)->type != TOKEN_EOF && !((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "}") == 0)) {
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
    } else if ((*tokens)->type != TOKEN_EOF) {
        snprintf(error->message, sizeof(error->message), "Expected '}' at end of block");
        reportParserError(error);
    }

    return head;
}

ASTNode* parse(Token* tokens) {
    ParserError error = {0, 0, ""};
    return parseBlock(&tokens, &error);
}

void printAST(ASTNode* node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->nodeType) {
        case AST_FUNC_DEF:
            printf("Function: %s\n", node->token.value);
            break;
        case AST_RETURN:
            printf("Return\n");
            break;
        case AST_VAR_ASSIGN:
            printf("Assign: %s\n", node->token.value);
            break;
        case AST_PRINT:
            printf("Print\n");
            break;
        case AST_INPUT:
            printf("Input\n");
            break;
        case AST_WHILE_LOOP:
            printf("While\n");
            break;
        case AST_IF_STATEMENT:
            printf("If\n");
            break;
        case AST_EXPRESSION:
            printf("Expr: %s\n", node->token.value);
            break;
        default:
            printf("Unknown\n");
            break;
    }

    printAST(node->left, depth + 1);
    printAST(node->right, depth + 1);
    printAST(node->body, depth + 1);
    printAST(node->next, depth);
}
