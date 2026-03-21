#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"

static constexpr char empty[] = "";
// struct to iterate through the source code
struct Scanner {
    std::string source;
    const char *start = empty;
    const char *current = empty;
    std::size_t line{};
};
// Instance created
static Scanner scanner;
// Function to initialize our scanner
void init_scanner(std::string src);
// Function to skip whitespace characters
void skip_whitespace();
// Function to create a token
Token create_token(TokenType token_type);
// Function for error reporting
Token error_token(const char *message);
// Function to read input file into a buffer
std::string read_file(const char *path);
// Helper function to evaluate conditional advances - '!=' , '=='
bool match(char expected);
// Helper function to check if it is a number literal
bool is_digit(char c);
// Helper function to check if it is a number literal
bool is_alpha(char c);
// Helper to check keyword
TokenType check_keyword(int start, int length, const char *rest, TokenType type);
// Function to evaluate tokens
Token scan_token();
// Function to manage the process
bool run_file(const char *path);


#endif
