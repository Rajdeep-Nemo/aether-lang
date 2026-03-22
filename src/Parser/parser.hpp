#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include "token.hpp"
#include "arena.hpp"
#include "ast.hpp"
// Parser blueprint
struct Parser {
    std::vector<Token> tokens;
    size_t current_position;
    Arena *arena;
};
// Checks the next token without consuming the current one
Token peek(Parser *parser);
// Function to advance to the next token
Token advance(Parser *parser);
// Function to check if we have reached the end of the file
bool is_at_end(const Parser *parser);
// Function to parse the tokens into AST
ASTNode *parse(Parser *parser);
// Parses unary
ASTNode *parse_unary(Parser *parser);
// Parses a number literal (integers/floating points)
ASTNode *parse_primary(Parser *parser);
// Parses * and /
ASTNode *parse_multiplication_and_division(Parser *parser);
// Parses + and -
ASTNode *parse_addition_and_subtraction(Parser *parser);

#endif //PARSER_HPP
