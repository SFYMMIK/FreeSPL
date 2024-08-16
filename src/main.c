#include "include/lexer.h"
#include "include/parser.h"
#include "include/codegen.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    // Check for .spl or .SPL file extension
    const char *file_extension = strrchr(argv[1], '.');
    if (!file_extension || (strcmp(file_extension, ".spl") != 0 && strcmp(file_extension, ".SPL") != 0)) {
        printf("Error: File must have a .spl or .SPL extension.\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Could not open file: %s\n", argv[1]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* source = malloc(file_size + 1);
    fread(source, 1, file_size, file);
    fclose(file);
    source[file_size] = '\0';

    Token* tokens = lexer(source);
    Node* ast = parse(tokens);
    generate_code(ast);

    free(source);
    free(tokens);
    return 0;
}
