#ifndef ERROR_HPP
#define ERROR_HPP

#include "../Lexer/token.hpp"
#include <string_view>

// Used by the Lexer
void report_scan_error(int line, std::string_view message);

// Used by the Parser
void report_parser_error(const Token& token, std::string_view message);

// Used by the Evaluator
void report_runtime_error(std::string_view message);

#endif