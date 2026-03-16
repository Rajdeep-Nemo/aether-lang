cmake_minimum_required(VERSION 3.31)
project(aether-lang)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(AETHER_SOURCES
    src/main.cpp
    src/AST/ast.cpp
    src/Builtins/builtin.cpp
    src/Interpreter/interpreter.cpp
    src/Lexer/lexer.cpp
    src/Parser/parser.cpp
    src/Runtime/environment.cpp
    src/Runtime/object.cpp
    src/Runtime/value.cpp
    src/Util/error.cpp
)

# Add header include directories
set(AETHER_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AST
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Builtins
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Interpreter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Util
)

# Create the executable
add_executable(aether ${AETHER_SOURCES})

# Add include directories
target_include_directories(aether PRIVATE ${AETHER_INCLUDE_DIRS})

# Set compiler flags for better error reporting
if(MSVC)
    target_compile_options(aether PRIVATE /W4)
else()
    target_compile_options(aether PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Add tests subdirectory
add_subdirectory(tests)

