#ifndef PARSER_HPP
#define PARSER_HPP
#include "arena.hpp"
#include "ast.hpp"
#include "token.hpp"
#include <vector>
// Parser blueprint
struct Parser {
    std::vector<Token> tokens;
    size_t current_position;
    Arena* arena;
};
// Checks the next token without consuming the current one
Token peek(Parser* parser);
// Function to advance to the next token
Token advance(Parser* parser);
// Function to check if we have reached the end of the file
bool is_at_end(const Parser* parser);
// Returns the DataType corresponding to the TokenType
DataType get_data_type(TokenType type);
// Function to parse the tokens into AST
ASTNode* parse(Parser* parser);
// Parses unary
ASTNode* parse_unary(Parser* parser);
// Parses comparison
ASTNode* parse_comparison(Parser* parser);
// Parses a number literal (integers/floating points)
ASTNode* parse_primary(Parser* parser);
// Parses * and /
ASTNode* parse_multiplication_and_division(Parser* parser);
// Parses + and -
ASTNode* parse_addition_and_subtraction(Parser* parser);
// Parses a block
ASTNode* parse_block(Parser* parser);
// Parses a statement
ASTNode* parse_statement(Parser* parser);
// Parses an assignment
ASTNode* parse_assignment(Parser* parser);
// Synchronize the parser after a syntax error (Heart attack stopper)
void synchronize(Parser* parser);

#endif // PARSER_HPP
