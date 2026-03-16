# Aether Programming Language

Aether is a custom programming language interpreter implemented in C++.

---

## 🛠 Architecture & Design

**Aether** is implemented using a **procedural C++17** approach. The codebase intentionally avoids heavy Object-Oriented Programming (OOP) patterns in favor of a data-driven design:

* **POD (Plain Old Data) Structures:** Utilizing `structs` for AST nodes and tokens to ensure a predictable memory layout and cache-friendly data handling.
* **Functional Decomposition:** Maintains a clear separation between raw data structures and the logic-heavy components (Lexer, Parser, and Interpreter).
* **Minimal Abstraction:** Prioritizes code transparency and linear execution flow over complex class hierarchies and virtual dispatch.

---

## Build Instructions

### Requirements
- CMake 3.31 or higher
- C++20 compatible compiler (e.g., GCC 10+, Clang 10+, or MSVC 2019+)
- GoogleTest (automatically downloaded and built by CMake for testing)

### Building the Project
1. Clone or navigate to the project directory.
2. Create a build directory:
   ```
   mkdir build
   cd build
   ```
3. Configure the project with CMake:
   ```
   cmake ..
   ```
4. Build the executable:
   ```
   make
   ```
   (On Windows, use `cmake --build .` instead of `make`.)

5. Run the interpreter:
   ```
   ./aether
   ```

### Running Tests
To build and run the tests:
1. After building, run:
   ```
   ctest
   ```
   Or manually run the test executables in the build/tests/ directory.

---

## Project Structure

```
aether-lang/
├── CMakeLists.txt          # CMake build configuration for the project
├── grammar.ebnf            # EBNF grammar definition for the Aether language syntax
├── LICENSE                 # MIT License for the project
├── README.md               # This file, project documentation
├── .gitignore              # Git ignore rules for build artifacts and IDE files
├── examples/               # Directory for example Aether programs
├── src/
│   ├── main.cpp            # Main entry point for the Aether interpreter executable
│   ├── AST/
│   │   ├── ast.cpp         # Implementation of abstract syntax tree data structures
│   │   └── ast.hpp         # Header defining AST node types and interfaces
│   ├── Builtins/
│   │   ├── builtin.cpp     # Implementation of built-in functions and operations
│   │   └── builtin.hpp     # Header for built-in function declarations
│   ├── Interpreter/
│   │   ├── interpreter.cpp # Core interpreter logic for executing Aether code
│   │   └── interpreter.hpp # Header for interpreter functions and execution context
│   ├── Lexer/
│   │   ├── lexer.cpp       # Implementation of lexical analysis (tokenization)
│   │   ├── lexer.hpp       # Header for lexer functions and token scanning
│   │   └── token.hpp       # Definitions for token types and structures
│   ├── Parser/
│   │   ├── parser.cpp      # Implementation of syntax parsing from tokens to AST
│   │   └── parser.hpp      # Header for parser functions and grammar rules
│   ├── Runtime/
│   │   ├── environment.cpp # Implementation of variable scoping and environments
│   │   ├── environment.hpp # Header for environment management
│   │   ├── object.cpp      # Implementation of runtime object representations
│   │   ├── object.hpp      # Header for object types and operations
│   │   ├── value.cpp       # Implementation of value types and operations
│   │   └── value.hpp       # Header for value type definitions
│   └── Util/
│       ├── error.cpp       # Implementation of error reporting and handling
│       └── error.hpp       # Header for error utility functions
├── stdlib/
│   └── math.at             # Mathematical functions and constants
└── tests/
    ├── CMakeLists.txt      # CMake configuration for building tests
    ├── InterpreterTest.cpp # Unit tests for the interpreter component
    ├── LexerTest.cpp       # Unit tests for the lexer component
    └── ParserTest.cpp      # Unit tests for the parser component
```

---
