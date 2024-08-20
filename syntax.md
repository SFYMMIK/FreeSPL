
# FreeSPL Syntax Guide

## Overview
FreeSPL is designed to be a minimal and efficient programming language with a syntax that blends elements of Go and Python, along with its own unique features. This guide will explain the basic syntax of FreeSPL and provide examples to help you write and understand FreeSPL programs.

## Basic Syntax

### 1. Variables and Data Types
FreeSPL supports basic data types like `int` and `float`. Variables are declared and initialized using a simple syntax.

#### Example:
```c
int x = 10;
float y = 3.14;
```

### 2. Operators
FreeSPL supports standard arithmetic and comparison operators.

#### Arithmetic Operators:
- `+` : Addition
- `-` : Subtraction
- `*` : Multiplication
- `/` : Division

#### Comparison Operators:
- `==` : Equal to
- `!=` : Not equal to
- `<` : Less than
- `>` : Greater than
- `<=` : Less than or equal to
- `>=` : Greater than or equal to

#### Example:
```c
int result = x + 5;
if (x > y) {
    x = x - 1;
}
```

### 3. Control Structures

#### If-Else Statements
Conditional logic is handled using `if` and `else` statements.

#### Example:
```c
if (x > 0) {
    x = x + 1;
} else {
    x = x - 1;
}
```

#### While Loops
Loops are implemented using the `while` keyword.

#### Example:
```c
while (x < 10) {
    x = x + 1;
}
```

### 4. Functions
FreeSPL allows you to define functions for reusable code blocks. A function can be defined using the `func` keyword.

#### Syntax:
```c
func function_name(parameters) {
    // Function body
}
```

#### Example:
```c
func add(int a, int b) {
    return a + b;
}

int result = add(5, 3);
```

### 5. Comments
Comments in FreeSPL are written using `//` for single-line comments or `/* */` for multi-line comments.

#### Example:
```c
// This is a single-line comment

/*
This is a 
multi-line comment
*/
```

## Example Programs

### Example 1: Factorial Calculation
This example calculates the factorial of a number using a loop.

```c
int factorial(int n) {
    int result = 1;
    while (n > 0) {
        result = result * n;
        n = n - 1;
    }
    return result;
}

int fact = factorial(5);
```

### Example 2: Fibonacci Sequence
This example generates the Fibonacci sequence up to a given number.

```c
func fibonacci(int n) {
    int a = 0;
    int b = 1;
    while (n > 0) {
        int temp = a;
        a = b;
        b = temp + b;
        n = n - 1;
    }
    return a;
}

int fib = fibonacci(10);
```

## Building and Running Programs

### Step 1: Write Your Code
Write your FreeSPL code in a text editor and save it with a `.spl` extension, or directly in the interpreter.

### Step 2: Build the Project
Use the provided build system to compile the project.

```bash
cd build
make
./build.sh
```

### Step 3: Run the Program
After building, run your FreeSPL program using the interpreter:

```bash
./freespl <your_file>.spl
```

### Step 4: Debugging
If your program has errors, FreeSPL will report them during the lexing, parsing, or runtime stages, helping you debug and fix your code.

## Conclusion
This guide provides a basic understanding of FreeSPL syntax and how to write programs in it. As the language evolves, more features may be added, but the core principles of simplicity and efficiency will remain.
