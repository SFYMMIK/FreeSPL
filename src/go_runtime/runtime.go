package main

import (
    "fmt"
    "errors"
)

// runtime.go - Runs the FreeSPL program

var variables map[string]int

func run(ast *ASTNode) {
    variables = make(map[string]int)
    err := execute(ast)
    if err != nil {
        fmt.Println("Runtime Error:", err)
    }
}

func execute(node *ASTNode) error {
    if node == nil {
        return nil
    }

    switch node.Token.Type {
    case TOKEN_IDENTIFIER:
        return executeAssignment(node)
    case TOKEN_KEYWORD:
        switch node.Token.Value {
        case "if":
            return executeIf(node)
        case "while":
            return executeWhile(node)
        }
    }

    return execute(node.Next)
}

func executeAssignment(node *ASTNode) error {
    varName := node.Token.Value
    value, err := evaluate(node.Right)
    if err != nil {
        return err
    }
    variables[varName] = value
    fmt.Printf("%s = %d\n", varName, value)
    return nil
}

func executeIf(node *ASTNode) error {
    condition, err := evaluate(node.Left)
    if err != nil {
        return err
    }
    if condition != 0 {
        return execute(node.Right)
    } else if node.Next != nil {
        return execute(node.Next)
    }
    return nil
}

func executeWhile(node *ASTNode) error {
    for {
        condition, err := evaluate(node.Left)
        if err != nil {
            return err
        }
        if condition == 0 {
            break
        }
        if err := execute(node.Right); err != nil {
            return err
        }
    }
    return nil
}

func evaluate(node *ASTNode) (int, error) {
    if node == nil {
        return 0, errors.New("nil node in expression")
    }
    if node.Token.Type == TOKEN_NUMBER {
        value, err := strconv.Atoi(node.Token.Value)
        if err != nil {
            return 0, err
        }
        return value, nil
    }
    if node.Token.Type == TOKEN_IDENTIFIER {
        value, exists := variables[node.Token.Value]
        if !exists {
            return 0, fmt.Errorf("undefined variable: %s", node.Token.Value)
        }
        return value, nil
    }
    return 0, fmt.Errorf("invalid token in expression: %s", node.Token.Value)
}
