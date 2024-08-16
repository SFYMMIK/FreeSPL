
=========================================================
                          SPL
=========================================================
Welcome to Simple-Programming-Language or SPL, a custom programming language with features like GUI creation, HTTP requests, error and log handling, randomization, and more. This guide will help you understand how to use the language, connect different components, and work with the provided utilities.

---------------------------------------------------------
1. Project Setup
---------------------------------------------------------
Ensure you have the following project structure:

SPL/
├── src/
│   ├── lexer.c
│   ├── parser.c
│   ├── codegen.c
│   ├── main.c
│   ├── gui.c
│   ├── requests.c
│   ├── error_handling.c
│   ├── utils.c
│   └── extrafunc.c
│       ├── include/
│               ├── lexer.h
│               ├── parser.h
│               ├── codegen.h
│               ├── gui.h
│               ├── requests.h
│               ├── error_handling.h
│               ├── utils.h
│               └── extrafunc.h
├── lib/
│   ├── libgui.so  (placeholder for compiled shared library for GUI)
│   └── librequests.so (placeholder for compiled shared library for HTTP requests)
├── Makefile
└── readme.txt

---------------------------------------------------------
2. Building the Compiler
---------------------------------------------------------
1. Navigate to the root of your project directory.
2. Run the following command to build the compiler:
   ```
   make
   ```

This will generate the 'SPL-COMPILE' executable, which you can use to compile and run programs written in the SPL programming language.

---------------------------------------------------------
3. Using the Language
---------------------------------------------------------
### 3.1. Basic Structure

Every program in SPL begins with `#use` statements to include the necessary utilities. You can specify whether you want your program to run as a GUI or CLI application when compiling.

**Example for CLI:**
```
#use core;
#use extrafunc;
```

**Example for GUI:**
```
#use core;
#use GUI;
```

### 3.2. Compiling and Running Your Program

Once you've written your SPL program, use the following commands to compile and run it:

**For CLI-based Programs:**
```
./spl-compiler --compile --cli my_program.spl
```

**For GUI-based Programs:**
```
./spl-compiler --compile --gui my_program.spl
```

This compiles and runs your program based on the type of user interface you specified.

### 3.3. Variable Declarations

Variables can be declared using two different methods:

1. Type-inferred declaration using `:=`:
```
x := 10;
y := "Hello, World!";
```

2. Explicit type declaration using `var`:
```
var age int;
age = 42;

var name string = "Alice";
```

### 3.4. Functions

Functions are declared using the `func` keyword. Functions can accept parameters and return values.

Example:
```
func add(a int, b int) int {
    return a + b;
}

func main(core) {
    result := add(10, 5);
    print(result);  // Outputs: 15
};
```

### 3.5. Object-Oriented Programming

Classes are defined using the `class` keyword. Classes can have attributes and methods.

Example:
```
class Animal {
    var name string;

    func setName(name string) {
        this.name = name;
    };

    func speak() {
        print(this.name + " makes a sound");
    };
};

class Dog(Animal) {
    func speak() {
        print(this.name + " barks");
    };
};
```

### 3.6. Loops

Loops can be declared both inside and outside functions. A loop declared outside functions will run independently of the function logic.

Example:
```
loop {
    print("This runs in the background.");
    sleep(1000);  // Pauses for 1 second
};

func main(core) {
    loop {
        print("This loop runs inside main.");
        sleep(1000);
    };
};
```

### 3.7. Error Handling

Use `try` and `except` blocks to handle errors. The `errorhandl` function logs errors to a `.errorFile`.

Example:
```
try {
    result := 10 / 0;
} except (ZeroDivisionError) {
    errorhandl("ZeroDivisionError", "Attempted division by zero.", "Ensure the denominator is not zero.");
};
```

### 3.8. Log Handling

Use `loghandl` to automatically log function calls, parameters, and program execution details to a `.log` file.

Example:
```
loghandl("Program started.");

func greet(name string) {
    print("Hello, " + name + "!");
};

func main(core) {
    greet("Alice");
    loghandl("Program ended.");
};
```

### 3.9. File Handling

Use `open`, `write`, `cfile`, and `dfile` for file operations.

- `cfile`: Creates a new file.
- `dfile`: Deletes an existing file.
- `open`: Opens a file for reading or writing.
- `write`: Writes content to an open file.

Example:
```
cfile("example.txt");
file := open("example.txt", "w");
write(file, "Hello, File!");
```

### 3.10. HTTP Requests

Use the `requests` library to perform HTTP GET, POST, PUT, and DELETE requests.

Example:
```
response := requests.get("http://example.com");
if response.status_code == 200 {
    print("GET request successful: " + response.content);
};
```

### 3.11. GUI Applications

Use the `GUI` library to create graphical user interfaces (GUIs).

Example:
```
func on_button_click() {
    print("Button was clicked!");
};

func main(core) {
    window := GUI.Window("My GUI App", 400, 300);
    label := GUI.Label(window, "Welcome to the GUI App!");
    button := GUI.Button(window, "Click Me!", on_button_click);
    window.mainloop();
};
```

### 3.12. Randomization

Use the `randomize` function from `extrafunc` to generate random numbers, shuffle lists, or randomize strings.

Example:
```
func main(core) {
    rand_num := randomize(1, 100);
    print("Random number: " + rand_num);

    rand_list := randomize([1, 2, 3, 4, 5]);
    print("Randomized list: " + rand_list);

    rand_text := randomize("hello");
    print("Randomized text: " + rand_text);
};
```

---------------------------------------------------------
4. Combining Everything
---------------------------------------------------------
Here's a complete example combining variables, functions, error handling, logging, file handling, HTTP requests, GUI, and randomization:

Example:
```
#use core;
#use extrafunc;
#use GUI;
#use requests;

func on_button_click() {
    loghandl("Button clicked.");
    file := open("log.txt", "w");
    write(file, "Button was clicked!");
    print("Button was clicked!");
};

func main(core) {
    loghandl("Program started.");

    rand_num := randomize(1, 100);
    print("Random number: " + rand_num);

    response := requests.get("http://example.com");
    if response.status_code == 200 {
        print("GET request successful: " + response.content);
    };

    window := GUI.Window("My GUI App", 400, 300);
    label := GUI.Label(window, "Welcome to the GUI App!");
    button := GUI.Button(window, "Click Me!", on_button_click);
    window.mainloop();

    loghandl("Program ended.");
};
```

---------------------------------------------------------
5. Notes and Troubleshooting
---------------------------------------------------------
- Ensure all required libraries (GTK, libcurl, etc.) are installed.
- If you encounter memory issues, use `valgrind` to check for leaks.
- Customize the language and add more features as you develop your projects!

---------------------------------------------------------
6. Compiling SPL Files
---------------------------------------------------------
After building the compiler, you can compile your SPL programs using the following syntax:

1. **Compiling a CLI-based Program**:
   ```
   ./spl-compiler --compile --cli filename.spl
   ```
   This command compiles the `filename.spl` file as a console-based program.

2. **Compiling a GUI-based Program**:
   ```
   ./spl-compiler --compile --gui filename.spl
   ```
   This command compiles the `filename.spl` file as a GUI-based program, without showing the terminal during execution.

The `--compile` option indicates that you want to compile the given SPL file, while the `--gui` or `--cli` options specify the type of output program you are generating.

=========================================================
                    End of readme.txt
=========================================================
