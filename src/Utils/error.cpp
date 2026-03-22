#include "error.hpp"
#include <iostream>

void report_scan_error(const size_t line, const std::string_view message) {
    std::cerr << "[Line " << line << "] Lexer Error: " << message << std::endl;
}

void report_parser_error(const Token &token, const std::string_view message) {
    if (token.type == TokenType::END_OF_FILE) {
        std::cerr << "[Line " << token.line << "] Syntax Error at end of file: " << message
                  << std::endl;
    } else {
        std::cerr << "[Line " << token.line << "] Syntax Error at '" << token.lexeme
                  << "': " << message << std::endl;
    }
}

void report_runtime_error(const size_t line, const std::string_view message) {
    std::cerr << "[Line " << line << "] Runtime Error: " << message << std::endl;
}