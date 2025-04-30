#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "error_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int debug = 0;
    const char *filename = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [--debug] <source_file.spl>\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--debug") == 0) {
            debug = 1;
        } else {
            filename = argv[i];
        }
    }

    if (!filename) {
        fprintf(stderr, "Error: No source file specified.\n");
        return 1;
    }

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

    if (debug) {
        printf("[AST]\n");
        printAST(ast, 0);
    }

    set_debug_mode(debug);
    execute_program(ast);

    free(source);
    return 0;
}
