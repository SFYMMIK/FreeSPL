# FreeSPL Programming Language

## Overview
FreeSPL is a minimal, fast, and cross-platform programming language inspired by the syntax of Go and Python, with its own unique features. This project is currently under development and aims to provide a lightweight language with robust error handling and support for basic programming constructs like variables, conditionals, loops, and functions.

## Syntax Is Avalible In The `syntax.md` File.

## Project Structure
The project is organized as follows:

```
FreeSPL/
├── src/
│   ├── c_core/
│   │   ├── lexer.c
│   │   ├── parser.c
│   │   ├── memory.c (no content inside)
│   │   └── math.c (still non-existent)
│   ├── go_runtime/
│   │   ├── runtime.go
│   │   ├── error_handling.go (still non-existent)
│   │   └── interpreter.go
│   └── main.go
└── build/
    ├── Makefile
    └── build.sh
```

- **`src/c_core/`**: Contains core components written in C, including the lexer, parser, memory management, and basic math operations.
- **`src/go_runtime/`**: Contains Go files responsible for the runtime, error handling, and AST interpretation.
- **`src/main.go`**: The main entry point for the FreeSPL interpreter.
- **`build/`**: Contains build scripts and a Makefile for compiling the C components and building the Go project.

## Features
### 1. Lexical Analysis (Lexer)
- The lexer scans the source code and breaks it down into tokens such as identifiers, numbers, operators, keywords, and symbols.
- Error handling is implemented to catch unexpected characters and other lexical errors.

### 2. Parsing (Parser)
- The parser processes the tokens from the lexer to build an Abstract Syntax Tree (AST) that represents the structure of the code.
- Supports parsing of basic constructs including:
  - Variable assignments (`int x = 5;`)
  - Conditionals (`if`, `else`)
  - Loops (`while`)
- Error handling is provided to report syntax errors, such as unexpected tokens or missing braces.

### 3. Runtime and Interpreter
- The runtime, written in Go, executes the AST, managing variables, evaluating expressions, and executing control flow structures.
- Error handling in the runtime ensures that issues like undefined variables or invalid operations are caught and reported during execution.

## Example Usage
The following is a simple example of a FreeSPL program that demonstrates the language's syntax:

```c
int x = 5;
if (x > 3) {
    x = x + 2;
} else {
    x = x - 2;
}
while (x < 10) {
    x = x + 1;
}
```

## Building the Project
### Requirements
- **GCC**: For compiling the C components.
- **Go**: For compiling and running the Go runtime.

### Steps
1. **Compile the C components**:
   Navigate to the `build/` directory and run `make` to compile the C components into an executable.

   ```bash
   cd build
   make
   ```

2. **Build the Go runtime**:
   Run the `build.sh` script to build the Go runtime.

   ```bash
   ./build.sh
   ```

3. **Run FreeSPL**:
   After building, you can run the FreeSPL interpreter with the generated executable.

   ```bash
   ./freespl
   ```

## Error Handling
FreeSPL includes robust error handling across all components:
- **Lexer Errors**: Handles unexpected characters and other lexical issues.
- **Parser Errors**: Reports syntax errors, such as unexpected tokens or missing elements.
- **Runtime Errors**: Catches execution-time issues like undefined variables or invalid operations, ensuring that the user receives clear feedback.

## Cross-Platform Compatibility
FreeSPL is designed to be cross-platform, supporting Linux, Windows, macOS, and BSD. The build system and codebase are structured to ensure compatibility across these operating systems.

## Contributing
Contributions to FreeSPL are welcome. Please feel free to submit pull requests or report issues on the GitHub repository.

## License
FreeSPL is released under the BSD-3-Clause license. See the `LICENSE` file for more details.
