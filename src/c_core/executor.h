#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

void execute_program(ASTNode* node);
void set_debug_mode(int enabled);

#endif // EXECUTOR_H
