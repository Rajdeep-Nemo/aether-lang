#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"

// Function to initialize our scanner
void init_scanner(std::string src);
// Function that checks if we read the complete file or not
bool is_at_end();
// Function that moves the pointer 'current' forward
char advance();
// Function to check the next character
char peek();
// Function to check the second next character
char peek_next();
// Function to skip whitespace characters
void skip_whitespace();
// Function to create a token
Token create_token(TokenType token_type);
// Function for error reporting
Token error_token(const char* message);
// Function to read input file into a buffer
std::string read_file(const char *path);
// Helper function to evaluate conditional advances - '!=' , '=='
bool match(char expected);
// Helper function to check if it is a number literal
bool is_digit(char c);
// Helper function to check if it is a number literal
bool is_alpha(char c);
// Helper to check keyword
TokenType check_keyword(int start ,int length , const char* rest ,TokenType type);
// Function to evaluate tokens
Token scan_token();
// Function to manage the process
bool run_file(const char* path);
// Checks the next token without consuming the current one
Token peek_token();


#endif
