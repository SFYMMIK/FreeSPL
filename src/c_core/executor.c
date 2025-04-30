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
                return;

            case AST_VAR_ASSIGN:
                if (node->right != NULL) {
                    printf("[Assign] %s = %s\n", node->token.value, node->right->token.value);
                } else {
                    printf("[Assign] %s = (null)\n", node->token.value);
                }
                break;

            case AST_IF_STATEMENT:
                if (node->left && strcmp(node->left->token.value, "1") == 0)
                    execute(node->body);
                break;

            case AST_WHILE_LOOP:
                if (node->left && strcmp(node->left->token.value, "1") == 0)
                    execute(node->body);
                break;

            case AST_FUNC_DEF:
                printf("[Function Defined] %s\n", node->token.value);
                execute(node->body);
                break;

            case AST_INPUT:
                if (node->left != NULL) {
                    printf("%s", node->left->token.value);
                }
                char buffer[256];
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                printf("[Input Received] %s\n", buffer);
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
