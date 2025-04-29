#include "executor.h"
#include "token.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute(ASTNode* node) {
    while (node != NULL) {
        switch (node->nodeType) {
            case AST_PRINT:
                if (node->left != NULL) {
                    printf("%s\n", node->left->token.value);
                }
                break;
            case AST_RETURN:
                if (node->left != NULL) {
                    printf("[Return] %s\n", node->left->token.value);
                }
                return; // stop execution after return
            case AST_VAR_ASSIGN:
                printf("[Assign] %s = %s\n", node->token.value, node->right->token.value);
                break;
            case AST_IF_STATEMENT:
                if (node->left && strcmp(node->left->token.value, "1") == 0) {
                    execute(node->body);
                }
                break;
            case AST_WHILE_LOOP:
                // Simple simulation: only executes once if condition is "1"
                if (node->left && strcmp(node->left->token.value, "1") == 0) {
                    execute(node->body);
                }
                break;
            case AST_FUNC_DEF:
                printf("[Function Defined] %s\n", node->token.value);
                execute(node->body);
                break;
            default:
                // Other types like expressions
                break;
        }

        node = node->next;
    }
}

void execute_program(ASTNode* node) {
    execute(node);
}
