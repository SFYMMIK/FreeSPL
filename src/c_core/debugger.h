#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "token.h"
#include "parser.h"

void debuggerCheck(Token* tokens, int token_count, ASTNode* root);

#endif // DEBUGGER_H
