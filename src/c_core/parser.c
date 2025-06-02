// parser.c
#include "parser.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    ASTNode construction / destruction
*/

ASTNode* createNode(ASTNodeType nodeType, Token token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->nodeType = nodeType;
    node->token    = token;
    node->left     = NULL;
    node->right    = NULL;
    node->body     = NULL;
    node->next     = NULL;
    return node;
}

static void freeAST(ASTNode* node) {
    if (!node) return;
    freeAST(node->left);
    freeAST(node->right);
    freeAST(node->body);
    freeAST(node->next);
    free(node);
}

static void reportParserError(ParserError* error) {
    if (error && strlen(error->message) > 0) {
        printf("Parser Error [Line %d, Column %d]: %s\n",
               error->line, error->column, error->message);
    }
}

/*
    Non-static forward declarations (match parser.h):
*/

ASTNode* parseExpression(Token** tokens, ParserError* error);
ASTNode* parseStatement (Token** tokens, ParserError* error);
ASTNode* parseBlock     (Token** tokens, ParserError* error);

/*
    Static helpers for expression grammar with full precedence, including assignment at lowest level.
    Grammar (highest‐level → lowest‐level precedence):
      expression      := assignment
      assignment      := logicalOr [ "=" assignment ]
      logicalOr       := logicalAnd { "||" logicalAnd }
      logicalAnd      := equality { "&&" equality }
      equality        := relational { ("==" | "!=") relational }
      relational      := additive { ("<" | "<=" | ">" | ">=") additive }
      additive        := term { ("+" | "-") term }
      term            := factor { ("*" | "/" | "%") factor }
      factor          := ("+" | "-" | "!") factor
                       | NUMBER | STRING | IDENTIFIER [ "(" ")" ]
                       | "(" expression ")"
*/

static ASTNode* parseAssignment(Token** tokens, ParserError* error);
static ASTNode* parseLogicalOr (Token** tokens, ParserError* error);
static ASTNode* parseLogicalAnd(Token** tokens, ParserError* error);
static ASTNode* parseEquality  (Token** tokens, ParserError* error);
static ASTNode* parseRelational(Token** tokens, ParserError* error);
static ASTNode* parseAdditive  (Token** tokens, ParserError* error);
static ASTNode* parseTerm      (Token** tokens, ParserError* error);
static ASTNode* parseFactor    (Token** tokens, ParserError* error);

/*
    parse(): top‐level entry.  We call parseBlock until EOF, then report any error.
*/
ASTNode* parse(Token* tokens) {
    ParserError error = {0, 0, ""};
    ASTNode* root = parseBlock(&tokens, &error);
    if (strlen(error.message) > 0) {
        reportParserError(&error);
        freeAST(root);
        return NULL;
    }
    return root;
}

/*
    parseBlock:
      - Consumes statements until TOKEN_EOF or a '}'.
      - Attaches 'else' blocks to the previous 'if'.
      - Returns linked list of statements in this block.
*/
ASTNode* parseBlock(Token** tokens, ParserError* error) {
    ASTNode* head    = NULL;
    ASTNode* current = NULL;
    ASTNode* lastIf  = NULL;

    while ((*tokens)->type != TOKEN_EOF &&
           !((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "}") == 0)) {

        // Handle 'else' only if it directly follows an unmatched 'if'
        if ((*tokens)->type == TOKEN_KEYWORD && strcmp((*tokens)->value, "else") == 0) {
            if (lastIf && lastIf->nodeType == AST_IF_STATEMENT && lastIf->right == NULL) {
                (*tokens)++;  // consume 'else'
                if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "{") == 0) {
                    (*tokens)++;  // consume '{'
                    ASTNode* elseBlock = parseBlock(tokens, error);
                    lastIf->right = elseBlock;
                    continue;
                } else {
                    error->line = error->column = 0;
                    snprintf(error->message, sizeof(error->message),
                             "Expected '{' after 'else'");
                    return NULL;
                }
            } else {
                error->line = error->column = 0;
                snprintf(error->message, sizeof(error->message),
                         "Unexpected 'else' without matching 'if'");
                return NULL;
            }
        }

        ASTNode* stmt = parseStatement(tokens, error);
        if (!stmt) return NULL;

        if (!head) head = stmt;
        else current->next = stmt;
        current = stmt;

        if (stmt->nodeType == AST_IF_STATEMENT) lastIf = stmt;
        else lastIf = NULL;
    }

    // If we stopped on '}', consume it
    if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "}") == 0) {
        (*tokens)++;
    }
    return head;
}

/*
    parseStatement:
      - Skips stray semicolons.
      - Handles 'func', 'print', 'input', 'return', 'if', 'while'.
      - Otherwise, parses an expression (includes assignments, calls).
      - Requires a trailing ';' after expressions, print, input, return, or single‐stmt bodies.
*/
ASTNode* parseStatement(Token** tokens, ParserError* error) {
    // Skip multiple semicolons
    while ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
        (*tokens)++;
        if ((*tokens)->type == TOKEN_EOF) return NULL;
    }

    Token tk = **tokens;

    // KEYWORD STATEMENTS:
    if (tk.type == TOKEN_KEYWORD) {
        // --- 'func' <name> "(" ")" "{" <block> "}"
        if (strcmp(tk.value, "func") == 0) {
            (*tokens)++;  // consume 'func'
            Token funcName = **tokens;
            if (funcName.type != TOKEN_IDENTIFIER) {
                error->line = error->column = 0;
                snprintf(error->message, sizeof(error->message),
                         "Expected function name after 'func'");
                return NULL;
            }
            (*tokens)++;  // consume identifier

            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "(") == 0) {
                (*tokens)++;
            } else {
                error->line = error->column = 0;
                snprintf(error->message, sizeof(error->message),
                         "Expected '(' after function name");
                return NULL;
            }

            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ")") == 0) {
                (*tokens)++;
            } else {
                error->line = error->column = 0;
                snprintf(error->message, sizeof(error->message),
                         "Expected ')' after '(' in function definition");
                return NULL;
            }

            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "{") == 0) {
                (*tokens)++;
            } else {
                error->line = error->column = 0;
                snprintf(error->message, sizeof(error->message),
                         "Expected '{' to start function body");
                return NULL;
            }

            ASTNode* node = createNode(AST_FUNC_DEF, funcName);
            node->body = parseBlock(tokens, error);
            if (!node->body && strlen(error->message) > 0) {
                freeAST(node);
                return NULL;
            }
            return node;
        }

        // --- 'print' <expr> ';'
        if (strcmp(tk.value, "print") == 0) {
            (*tokens)++;  // consume 'print'
            ASTNode* expr = parseExpression(tokens, error);
            if (!expr) return NULL;
            ASTNode* node = createNode(AST_PRINT, tk);
            node->left = expr;
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
                (*tokens)++;
            }
            return node;
        }

        // --- 'input' <expr> ';'
        if (strcmp(tk.value, "input") == 0) {
            (*tokens)++;  // consume 'input'
            ASTNode* expr = parseExpression(tokens, error);
            if (!expr) return NULL;
            ASTNode* node = createNode(AST_INPUT, tk);
            node->left = expr;
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
                (*tokens)++;
            }
            return node;
        }

        // --- 'return' <expr> ';'
        if (strcmp(tk.value, "return") == 0) {
            (*tokens)++;  // consume 'return'
            ASTNode* expr = parseExpression(tokens, error);
            if (!expr) return NULL;
            ASTNode* node = createNode(AST_RETURN, tk);
            node->left = expr;
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
                (*tokens)++;
            }
            return node;
        }

        // --- 'if' <expr> (block | single‐stmt)
        if (strcmp(tk.value, "if") == 0) {
            (*tokens)++;  // consume 'if'
            ASTNode* cond = parseExpression(tokens, error);
            if (!cond) return NULL;

            ASTNode* thenBody = NULL;
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "{") == 0) {
                (*tokens)++;  // consume '{'
                thenBody = parseBlock(tokens, error);
                if (!thenBody && strlen(error->message) > 0) {
                    freeAST(cond);
                    return NULL;
                }
            } else {
                thenBody = parseStatement(tokens, error);
                if (!thenBody && strlen(error->message) > 0) {
                    freeAST(cond);
                    return NULL;
                }
            }
            ASTNode* node = createNode(AST_IF_STATEMENT, tk);
            node->left = cond;
            node->body = thenBody;
            return node;
        }

        // --- 'while' <expr> (block | single‐stmt)
        if (strcmp(tk.value, "while") == 0) {
            (*tokens)++;  // consume 'while'
            ASTNode* cond = parseExpression(tokens, error);
            if (!cond) return NULL;

            ASTNode* loopBody = NULL;
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "{") == 0) {
                (*tokens)++;  // consume '{'
                loopBody = parseBlock(tokens, error);
                if (!loopBody && strlen(error->message) > 0) {
                    freeAST(cond);
                    return NULL;
                }
            } else {
                loopBody = parseStatement(tokens, error);
                if (!loopBody && strlen(error->message) > 0) {
                    freeAST(cond);
                    return NULL;
                }
            }
            ASTNode* node = createNode(AST_WHILE_LOOP, tk);
            node->left = cond;
            node->body = loopBody;
            return node;
        }
    }

    // Otherwise: parse as expression statement (includes assignments, calls)
    ASTNode* expr = parseExpression(tokens, error);
    if (!expr) return NULL;

    if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ";") == 0) {
        (*tokens)++;
    }
    return expr;
}

/*
    parseExpression (highest level): parseAssignment
*/
ASTNode* parseExpression(Token** tokens, ParserError* error) {
    return parseAssignment(tokens, error);
}

/*
    parseAssignment:
      assignment := logicalOr [ "=" assignment ]
*/
static ASTNode* parseAssignment(Token** tokens, ParserError* error) {
    ASTNode* left = parseLogicalOr(tokens, error);
    if (!left) return NULL;

    if ((*tokens)->type == TOKEN_OPERATOR && strcmp((*tokens)->value, "=") == 0) {
        Token opTok = **tokens;
        (*tokens)++;  // consume '='
        ASTNode* right = parseAssignment(tokens, error);
        if (!right) {
            freeAST(left);
            return NULL;
        }
        ASTNode* node = createNode(AST_VAR_ASSIGN, opTok);
        node->left  = left;
        node->right = right;
        return node;
    }
    return left;
}

/*
    parseLogicalOr:
      logicalOr := logicalAnd { "||" logicalAnd }*
*/
static ASTNode* parseLogicalOr(Token** tokens, ParserError* error) {
    ASTNode* left = parseLogicalAnd(tokens, error);
    if (!left) return NULL;

    while ((*tokens)->type == TOKEN_OPERATOR && strcmp((*tokens)->value, "||") == 0) {
        Token opTok = **tokens;
        (*tokens)++;
        ASTNode* right = parseLogicalAnd(tokens, error);
        if (!right) {
            freeAST(left);
            return NULL;
        }
        ASTNode* bin = createNode(AST_EXPRESSION, opTok);
        bin->left  = left;
        bin->right = right;
        left = bin;
    }
    return left;
}

/*
    parseLogicalAnd:
      logicalAnd := equality { "&&" equality }*
*/
static ASTNode* parseLogicalAnd(Token** tokens, ParserError* error) {
    ASTNode* left = parseEquality(tokens, error);
    if (!left) return NULL;

    while ((*tokens)->type == TOKEN_OPERATOR && strcmp((*tokens)->value, "&&") == 0) {
        Token opTok = **tokens;
        (*tokens)++;
        ASTNode* right = parseEquality(tokens, error);
        if (!right) {
            freeAST(left);
            return NULL;
        }
        ASTNode* bin = createNode(AST_EXPRESSION, opTok);
        bin->left  = left;
        bin->right = right;
        left = bin;
    }
    return left;
}

/*
    parseEquality:
      equality := relational { ("==" | "!=") relational }*
*/
static ASTNode* parseEquality(Token** tokens, ParserError* error) {
    ASTNode* left = parseRelational(tokens, error);
    if (!left) return NULL;

    while ((*tokens)->type == TOKEN_OPERATOR &&
           (strcmp((*tokens)->value, "==") == 0 || strcmp((*tokens)->value, "!=") == 0))
    {
        Token opTok = **tokens;
        (*tokens)++;
        ASTNode* right = parseRelational(tokens, error);
        if (!right) {
            freeAST(left);
            return NULL;
        }
        ASTNode* bin = createNode(AST_EXPRESSION, opTok);
        bin->left  = left;
        bin->right = right;
        left = bin;
    }
    return left;
}

/*
    parseRelational:
      relational := additive { ("<" | "<=" | ">" | ">=") additive }*
*/
static ASTNode* parseRelational(Token** tokens, ParserError* error) {
    ASTNode* left = parseAdditive(tokens, error);
    if (!left) return NULL;

    while ((*tokens)->type == TOKEN_OPERATOR &&
           (strcmp((*tokens)->value, "<") == 0  ||
            strcmp((*tokens)->value, "<=") == 0 ||
            strcmp((*tokens)->value, ">") == 0  ||
            strcmp((*tokens)->value, ">=") == 0))
    {
        Token opTok = **tokens;
        (*tokens)++;
        ASTNode* right = parseAdditive(tokens, error);
        if (!right) {
            freeAST(left);
            return NULL;
        }
        ASTNode* bin = createNode(AST_EXPRESSION, opTok);
        bin->left  = left;
        bin->right = right;
        left = bin;
    }
    return left;
}

/*
    parseAdditive:
      additive := term { ("+" | "-") term }*
*/
static ASTNode* parseAdditive(Token** tokens, ParserError* error) {
    ASTNode* left = parseTerm(tokens, error);
    if (!left) return NULL;

    while ((*tokens)->type == TOKEN_OPERATOR &&
           (strcmp((*tokens)->value, "+") == 0 || strcmp((*tokens)->value, "-") == 0))
    {
        Token opTok = **tokens;
        (*tokens)++;
        ASTNode* right = parseTerm(tokens, error);
        if (!right) {
            freeAST(left);
            return NULL;
        }
        ASTNode* bin = createNode(AST_EXPRESSION, opTok);
        bin->left  = left;
        bin->right = right;
        left = bin;
    }
    return left;
}

/*
    parseTerm:
      term := factor { ("*" | "/" | "%") factor }*
*/
static ASTNode* parseTerm(Token** tokens, ParserError* error) {
    ASTNode* left = parseFactor(tokens, error);
    if (!left) return NULL;

    while ((*tokens)->type == TOKEN_OPERATOR &&
           (strcmp((*tokens)->value, "*") == 0 ||
            strcmp((*tokens)->value, "/") == 0 ||
            strcmp((*tokens)->value, "%") == 0))
    {
        Token opTok = **tokens;
        (*tokens)++;
        ASTNode* right = parseFactor(tokens, error);
        if (!right) {
            freeAST(left);
            return NULL;
        }
        ASTNode* bin = createNode(AST_EXPRESSION, opTok);
        bin->left  = left;
        bin->right = right;
        left = bin;
    }
    return left;
}

/*
    parseFactor:
      factor := ("+" | "-" | "!") factor
               | NUMBER | STRING
               | IDENTIFIER [ "(" ")" ]
               | "(" expression ")"
*/
static ASTNode* parseFactor(Token** tokens, ParserError* error) {
    Token tk = **tokens;

    // Unary +, -, !
    if (tk.type == TOKEN_OPERATOR &&
        (strcmp(tk.value, "+") == 0 || strcmp(tk.value, "-") == 0 || strcmp(tk.value, "!") == 0))
    {
        Token opTok = tk;
        (*tokens)++;
        ASTNode* operand = parseFactor(tokens, error);
        if (!operand) return NULL;
        ASTNode* unaryNode = createNode(AST_EXPRESSION, opTok);
        unaryNode->left = operand;
        return unaryNode;
    }

    // NUMBER or STRING literal
    if (tk.type == TOKEN_NUMBER || tk.type == TOKEN_STRING) {
        ASTNode* litNode = createNode(AST_EXPRESSION, tk);
        (*tokens)++;
        return litNode;
    }

    // IDENTIFIER or keyword "loop" used as call: treat both as identifier
    if (tk.type == TOKEN_IDENTIFIER || (tk.type == TOKEN_KEYWORD && strcmp(tk.value, "loop") == 0)) {
        Token idTok = tk;
        (*tokens)++;  // consume IDENT (or "loop")

        // If next is "(" and then ")", that’s a call with no args:
        if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, "(") == 0) {
            (*tokens)++;  // consume "("
            if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ")") == 0) {
                (*tokens)++;  // consume ")"
                ASTNode* callNode = createNode(AST_EXPRESSION, idTok);
                return callNode;
            } else {
                error->line = error->column = 0;
                snprintf(error->message, sizeof(error->message),
                         "Expected ')' after '(' in function call");
                return NULL;
            }
        }

        // Otherwise, simple identifier node
        ASTNode* idNode = createNode(AST_EXPRESSION, idTok);
        return idNode;
    }

    // Parenthesized expression
    if (tk.type == TOKEN_SYMBOL && strcmp(tk.value, "(") == 0) {
        (*tokens)++;  // consume '('
        ASTNode* inner = parseExpression(tokens, error);
        if (!inner) return NULL;

        if ((*tokens)->type == TOKEN_SYMBOL && strcmp((*tokens)->value, ")") == 0) {
            (*tokens)++;  // consume ')'
            return inner;
        } else {
            error->line = error->column = 0;
            snprintf(error->message, sizeof(error->message),
                     "Expected ')' after expression");
            freeAST(inner);
            return NULL;
        }
    }

    // If none matched, it’s an invalid factor
    error->line = error->column = 0;
    snprintf(error->message, sizeof(error->message),
             "Invalid expression starting with '%s'", tk.value);
    return NULL;
}

/*
    printAST: unchanged from your original
*/
void printAST(ASTNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) printf("  ");
    printf("%s\n", node->token.value);
    printAST(node->left, depth + 1);
    printAST(node->right, depth + 1);
    printAST(node->body, depth + 1);
    printAST(node->next, depth);
}
