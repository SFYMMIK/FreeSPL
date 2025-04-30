#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "error_handling.h"
#include "debugger.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file.spl>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *source = malloc(size + 1);
    if (!source) {
        perror("Failed to allocate memory");
        fclose(file);
        return 1;
    }

    if (fread(source, 1, size, file) != (size_t)size) {
        perror("Failed to read file");
        free(source);
        fclose(file);
        return 1;
    }

    source[size] = '\0';
    fclose(file);

    int token_count = 0;
    Token* tokens = lex(source, &token_count);

    ASTNode* ast = parse(tokens);

    printf("[AST]\n");
    printAST(ast, 0);

    // Call the debugger here before executing
    debuggerCheck(tokens, token_count, ast);

    execute_program(ast);

    free(source);
    return 0;
}
