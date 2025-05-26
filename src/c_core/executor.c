#include "parser.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Symulacja zmiennych — uproszczona
const char* get_variable_value(const char* name) {
    return NULL; // Zmiennych nie obsługujemy jeszcze
}

int evalExpression(ASTNode* node) {
    if (!node) return 0;

    if (node->token.type == TOKEN_NUMBER) {
        return atoi(node->token.value);
    }

    if (node->left && node->right && node->token.type == TOKEN_OPERATOR) {
        int left = evalExpression(node->left);
        int right = evalExpression(node->right);
        if (strcmp(node->token.value, "+") == 0) return left + right;
        if (strcmp(node->token.value, "-") == 0) return left - right;
        if (strcmp(node->token.value, "*") == 0) return left * right;
        if (strcmp(node->token.value, "/") == 0) return right != 0 ? left / right : 0;
        if (strcmp(node->token.value, "=") == 0) return left == right;
        if (strcmp(node->token.value, "<") == 0) return left < right;
        if (strcmp(node->token.value, ">") == 0) return left > right;
    }

    return 0;
}

void execute(ASTNode* node);

void executeBlock(ASTNode* node) {
    while (node != NULL) {
        execute(node);
        node = node->next;
    }
}

void execute(ASTNode* node) {
    if (!node) return;

    switch (node->nodeType) {
        case AST_FUNC_DEF:
            if (strcmp(node->token.value, "main") == 0) {
                executeBlock(node->body);
            }
            break;

        case AST_VAR_ASSIGN:
            // Na razie nie implementujemy zmiennych
            evalExpression(node->right);
            break;

        case AST_PRINT:
            if (node->left) {
                ASTNode* expr = node->left;

                if (expr->left && expr->right && expr->token.type == TOKEN_OPERATOR) {
                    int val = evalExpression(expr);
                    printf("%d\n", val);
                } else if (expr->token.type == TOKEN_NUMBER) {
                    printf("%s\n", expr->token.value);
                } else if (expr->token.type == TOKEN_IDENTIFIER) {
                    const char* val = get_variable_value(expr->token.value);
                    printf("%s\n", val ? val : expr->token.value);
                } else if (expr->nodeType == AST_EXPRESSION) {
                    printf("%s\n", expr->token.value);
                } else {
                    printf("%s\n", expr->token.value);
                }
            }
            break;

        case AST_INPUT:
            // Pomijamy obsługę input na razie
            break;

        case AST_IF_STATEMENT: {
            int cond = evalExpression(node->left);
            if (cond) {
                executeBlock(node->body);
            } else if (node->right) {
                executeBlock(node->right);
            }
            break;
        }

        case AST_WHILE_LOOP: {
            while (evalExpression(node->left)) {
                executeBlock(node->body);
            }
            break;
        }

        case AST_RETURN:
        case AST_LOOP:
        case AST_BREAK:
        case AST_EXPRESSION:
            // Skipping
            break;

        default:
            printf("[UNSUPPORTED NODE TYPE: %d]\n", node->nodeType);
            break;
    }
}

// Globalna flaga debugowania
static int DEBUG_MODE = 0;

// Ustawianie trybu debugowania z main.c
void set_debug_mode(int enabled) {
    DEBUG_MODE = enabled;
}

// Główna funkcja uruchamiająca program (wołana z main.c)
void execute_program(ASTNode* root) {
    if (DEBUG_MODE) {
        printf("[RUNNING in DEBUG MODE]\n");
    }
    execute(root);
}
