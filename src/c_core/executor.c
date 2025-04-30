#include "executor.h"
#include "token.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

static int debug_mode = 0;
static jmp_buf break_jmp;
static jmp_buf continue_jmp;

#define MAX_VARIABLES 100

typedef struct {
    char name[100];
    char value[256];
} Variable;

static Variable variables[MAX_VARIABLES];
static int variable_count = 0;

void set_debug_mode(int enabled) {
    debug_mode = enabled;
}

const char* get_variable_value(const char* name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    return NULL;
}

void set_variable_value(const char* name, const char* value) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            strcpy(variables[i].value, value);
            return;
        }
    }
    if (variable_count < MAX_VARIABLES) {
        strcpy(variables[variable_count].name, name);
        strcpy(variables[variable_count].value, value);
        variable_count++;
    }
}

int evaluate_expression(ASTNode* node) {
    if (!node) return 0;

    if (node->left && node->right && node->token.type == TOKEN_OPERATOR) {
        int lhs = evaluate_expression(node->left);
        int rhs = evaluate_expression(node->right);
        switch (node->token.value[0]) {
            case '+': return lhs + rhs;
            case '-': return lhs - rhs;
            case '*': return lhs * rhs;
            case '/': return rhs != 0 ? lhs / rhs : 0;
        }
    }

    if (node->token.type == TOKEN_NUMBER) {
        return atoi(node->token.value);
    } else if (node->token.type == TOKEN_IDENTIFIER) {
        const char* val = get_variable_value(node->token.value);
        return val ? atoi(val) : 0;
    }

    return 0;
}

void execute(ASTNode* node);

void executeLoopBody(ASTNode* body) {
    if (setjmp(break_jmp) == 0) {
        while (1) {
            if (setjmp(continue_jmp) == 0) {
                execute(body);
            }
        }
    }
}

void execute(ASTNode* node) {
    while (node != NULL) {
        switch (node->nodeType) {
            case AST_PRINT:
                if (node->left) {
                    if (node->left->token.type == TOKEN_IDENTIFIER) {
                        const char* val = get_variable_value(node->left->token.value);
                        printf("%s\n", val ? val : node->left->token.value);
                    } else {
                        printf("%s\n", node->left->token.value);
                    }
                }
                break;

            case AST_RETURN:
                if (node->left) {
                    int result = evaluate_expression(node->left);
                    if (debug_mode)
                        printf("[Return] %d\n", result);
                }
                return;

            case AST_VAR_ASSIGN: {
                int result = evaluate_expression(node->right);
                char str[32];
                snprintf(str, sizeof(str), "%d", result);
                set_variable_value(node->token.value, str);
                if (debug_mode)
                    printf("[Assign] %s = %s\n", node->token.value, str);
                break;
            }

            case AST_IF_STATEMENT:
                if (evaluate_expression(node->left)) {
                    execute(node->body);
                } else if (node->right) {
                    execute(node->right->body);
                }
                break;

            case AST_WHILE_LOOP:
                while (evaluate_expression(node->left))
                    executeLoopBody(node->body);
                break;

            case AST_FUNC_DEF:
                if (debug_mode)
                    printf("[Function Defined] %s\n", node->token.value);
                execute(node->body);
                break;

            case AST_INPUT:
                if (node->left != NULL) {
                    printf("%s", node->left->token.value);
                }
                {
                    char buffer[256];
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';
                    set_variable_value("input", buffer);
                    if (debug_mode)
                        printf("[Input Received] %s\n", buffer);
                }
                break;

            case AST_LOOP:
                longjmp(continue_jmp, 1);
                break;

            case AST_BREAK:
                longjmp(break_jmp, 1);
                break;

            default:
                break;
        }

        node = node->next;
    }
}

void execute_program(ASTNode* node) {
    execute(node);
}
