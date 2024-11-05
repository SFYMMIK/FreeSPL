package main

import (
	"errors"
	"fmt"
)

/*
#cgo CFLAGS: -I../c_core
#cgo LDFLAGS: -L../c_core -lcore
#include "lexer.h"
#include "parser.h"
*/
import "C"

// TokenType represents the type of a token.
type TokenType int

const (
	TOKEN_IDENTIFIER TokenType = iota
	TOKEN_NUMBER
	TOKEN_OPERATOR
	TOKEN_KEYWORD
	TOKEN_SYMBOL
	TOKEN_EOF
	TOKEN_UNKNOWN
)

// Token represents a lexical token.
type Token struct {
	Type  TokenType
	Value string
}

// ASTNode represents a node in the abstract syntax tree.
type ASTNode struct {
	Token Token
	Left  *ASTNode
	Right *ASTNode
	Next  *ASTNode
}

// Lexer and Parser in Go
func lex(input string) []Token {
	var tokens []Token
	p := 0
	inputLen := len(input)

	for p < inputLen {
		if isWhitespace(input[p]) {
			p++
			continue
		}

		if isAlpha(input[p]) {
			start := p
			for p < inputLen && isAlnum(input[p]) {
				p++
			}
			value := input[start:p]
			if isKeyword(value) {
				tokens = append(tokens, Token{Type: TOKEN_KEYWORD, Value: value})
			} else {
				tokens = append(tokens, Token{Type: TOKEN_IDENTIFIER, Value: value})
			}
		} else if isDigit(input[p]) {
			start := p
			for p < inputLen && isDigit(input[p]) {
				p++
			}
			tokens = append(tokens, Token{Type: TOKEN_NUMBER, Value: input[start:p]})
		} else if isOperator(input[p]) {
			start := p
			for p < inputLen && isOperator(input[p]) {
				p++
			}
			tokens = append(tokens, Token{Type: TOKEN_OPERATOR, Value: input[start:p]})
		} else if isSymbol(input[p]) {
			tokens = append(tokens, Token{Type: TOKEN_SYMBOL, Value: string(input[p])})
			p++
		} else {
			tokens = append(tokens, Token{Type: TOKEN_UNKNOWN, Value: string(input[p])})
			p++
		}
	}

	tokens = append(tokens, Token{Type: TOKEN_EOF, Value: "EOF"})
	return tokens
}

// parseBlock processes a list of tokens into an AST block node
func parseBlock(tokens *[]Token) *ASTNode {
	if len(*tokens) == 0 {
		return nil
	}

	var root *ASTNode = nil
	var currentNode *ASTNode = nil

	for len(*tokens) > 0 {
		token := (*tokens)[0]
		*tokens = (*tokens)[1:]

		switch token.Type {
		case TOKEN_KEYWORD:
			if token.Value == "end" {
				return root
			}

			// Create a new AST node for the statement
			newNode := &ASTNode{
				Token: token,
			}

			if root == nil {
				root = newNode
				currentNode = newNode
			} else {
				currentNode.Next = newNode
				currentNode = newNode
			}

		case TOKEN_IDENTIFIER, TOKEN_OPERATOR, TOKEN_NUMBER, TOKEN_SYMBOL:
			// Handle other token types similarly by appending to the current statement node
			newNode := &ASTNode{
				Token: token,
			}

			if currentNode == nil {
				currentNode = newNode
				root = newNode
			} else {
				currentNode.Right = newNode
				currentNode = newNode
			}

		default:
			panic(errors.New("unexpected token type: " + fmt.Sprint(token.Type)))
		}
	}

	return root
}

// parse is the entry point for parsing a list of tokens into an AST
func parse(tokens []Token) *ASTNode {
	return parseBlock(&tokens)
}

// Helper functions for lexing
func isWhitespace(c byte) bool {
	return c == ' ' || c == '\n' || c == '\t' || c == '\r'
}

func isAlpha(c byte) bool {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
}

func isAlnum(c byte) bool {
	return isAlpha(c) || isDigit(c)
}

func isDigit(c byte) bool {
	return c >= '0' && c <= '9'
}

func isOperator(c byte) bool {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!'
}

func isSymbol(c byte) bool {
	return c == '(' || c == ')' || c == '{' || c == '}' || c == ';'
}

func isKeyword(value string) bool {
	keywords := []string{"if", "else", "while", "for", "return", "int", "float", "void"}
	for _, keyword := range keywords {
		if value == keyword {
			return true
		}
	}
	return false
}

// run executes the AST
func executeAST(node *ASTNode) {
	if node == nil {
		return
	}

	switch node.Token.Type {
	case TOKEN_KEYWORD:
		fmt.Println("Keyword:", node.Token.Value)
	case TOKEN_IDENTIFIER:
		fmt.Println("Identifier:", node.Token.Value)
	case TOKEN_NUMBER:
		fmt.Println("Number:", node.Token.Value)
	case TOKEN_OPERATOR:
		fmt.Println("Operator:", node.Token.Value)
	case TOKEN_SYMBOL:
		fmt.Println("Symbol:", node.Token.Value)
	case TOKEN_EOF:
		fmt.Println("End of File")
	default:
		fmt.Println("Unknown token:", node.Token.Value)
	}

	executeAST(node.Left)
	executeAST(node.Right)
	executeAST(node.Next)
}

// main function
func mainFunction() {
	code := `
		int x = 5;
		if (x > 3) {
			x = x + 2;
		} else {
			x = x - 2;
		}
		while (x < 10) {
			x = x + 1;
		}
	`

	tokens := lex(code)
	ast := parse(tokens)
	run(ast)
}
