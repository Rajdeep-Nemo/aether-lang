cmake_minimum_required(VERSION 3.31)
project(aether-lang)

# Enable AddressSanitizer for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library source files
set(AETHER_LIB_SOURCES
    src/AST/ast.cpp
    src/Builtins/builtin.cpp
    src/Evaluator/evaluator.cpp
    src/Lexer/lexer.cpp
    src/Parser/parser.cpp
    src/Runtime/environment.cpp
    src/Runtime/object.cpp
    src/Runtime/value.cpp
    src/Utils/error.cpp
    src/Utils/arena.cpp
)

# Main executable source
set(AETHER_MAIN_SOURCES
    src/main.cpp
)

# Header include directories
set(AETHER_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AST
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Builtins
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Evaluator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils
)

# Create static library
add_library(aether_lib STATIC ${AETHER_LIB_SOURCES})

# Create the executable
add_executable(aether ${AETHER_MAIN_SOURCES})

# Link executable to library
target_link_libraries(aether PRIVATE aether_lib)

# Include directories for library
target_include_directories(aether_lib PUBLIC ${AETHER_INCLUDE_DIRS})

# Include directories for executable
target_include_directories(aether PRIVATE ${AETHER_INCLUDE_DIRS})

# Set compiler flags for better error reporting
if(MSVC)
    target_compile_options(aether PRIVATE /W4)
else()
    target_compile_options(aether PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Add tests subdirectory
add_subdirectory(tests)

