#include "include/parser.h"
#include <stdlib.h>

Node* create_node(Token token) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* parse(Token* tokens) {
    Node* root = create_node(tokens[0]);
    // More complex parsing logic goes here
    return root;
}
