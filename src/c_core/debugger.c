#include "debugger.h"
#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isImportUsed(ASTNode* node, const char* importName) {
    if (node == NULL) return 0;

    if (node->token.type == TOKEN_IDENTIFIER || node->token.type == TOKEN_KEYWORD) {
        if (strcmp(node->token.value, importName) == 0) {
            return 1;
        }
    }

    return isImportUsed(node->left, importName) ||
           isImportUsed(node->right, importName) ||
           isImportUsed(node->body, importName) ||
           isImportUsed(node->next, importName);
}

void debuggerCheck(Token* tokens, int token_count, ASTNode* root) {
    for (int i = 0; i < token_count; i++) {
        if (tokens[i].type == TOKEN_IMPORT || tokens[i].type == TOKEN_IMPORT_FROM_C) {
            if (!isImportUsed(root, tokens[i].value)) {
                printf("[DebuggerWarning] Unused import: %s\n", tokens[i].value);
                printf("Continue compilation? [Y/n]: ");

                char response[10];
                fgets(response, sizeof(response), stdin);

                if (response[0] == 'n' || response[0] == 'N') {
                    printf("Compilation aborted due to unused import.\n");
                    exit(1);
                }
            }
        }
    }
}
