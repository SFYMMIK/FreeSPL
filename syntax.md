
# FreeSPL Syntax Guide

## 1. Variables
- Variables are declared using the `let` keyword, followed by the variable name and an optional type.
- **Example**:
  ```spl
  let x = 10
  ```

## 2. Control Structures
- **If-Else**: Python-like indentation with Go-like syntax.
  ```spl
  if condition {
    // code
  } else {
    // code
  }
  ```

- **Loops**: Only `for` loops are supported.
  ```spl
  for i = 0; i < 10; i++ {
    // code
  }
  ```

## 3. Functions
- Functions are defined using `func`, with parameters and optional return types.
  ```spl
  func add(a, b) -> int {
    return a + b
  }
  ```

## 4. Data Types
- **Supported Types**: `int`, `float`, `string`, `bool`
- Complex types like arrays and maps are still under development.

## 5. Comments
- **Single-line**: `//`
- Multi-line comments are not yet implemented.

## 6. Error Handling
- Error handling uses Go-like error returns, but it is still a work in progress.

## 7. Future Features (Which wont be added since this repo is archived)
- Array and map types.
- Multi-line comments.
