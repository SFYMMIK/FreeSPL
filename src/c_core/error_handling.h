#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

typedef struct {
    int line;
    int column;
    char message[256];
} LexerError;

void reportLexerError(LexerError* error);

#endif // ERROR_HANDLING_H
