#include "parser.hpp"
#include "token.hpp"
// Checks the next token without consuming the current one
Token peek(Parser *parser) {
    if (is_at_end(parser)) {
        return parser->tokens[parser->current_position];
    }
    return parser->tokens[parser->current_position];
}
// Consumes and returns the token
Token advance(Parser *parser) {
    Token t = peek(parser);
    if (!is_at_end(parser)) {
        parser->current_position++;
    }
    return t;
}
// Function to check if the current token matches the expected token type
bool is_at_end(const Parser *parser) {
    if (parser->current_position >= parser->tokens.size() ||
        parser->tokens[parser->current_position].type == TokenType::END_OF_FILE) {
        return true;
    }
    return false;
}
// Parse the tokens into AST
ASTNode *parse(Parser *parser) {
    return parse_addition_and_subtraction(parser);
}
// Parses a number literal (integers/floating points)
ASTNode *parse_number(Parser *parser) {
    const Token current = peek(parser);
    if (current.type == TokenType::INT_LITERAL) {
        advance(parser);
        return create_int_node(parser->arena, std::stoll(current.lexeme));
    }
    if (current.type == TokenType::FLOAT_LITERAL) {
        advance(parser);
        return create_double_node(parser->arena, std::stod(current.lexeme));
    }
    return nullptr;
}
// Parses * and /
ASTNode *parse_multiplication_and_division(Parser *parser) {
    ASTNode *left = parse_number(parser);
    while (peek(parser).type == TokenType::STAR || peek(parser).type == TokenType::SLASH) {
        // "op" is the operator
        const Token op = advance(parser);
        ASTNode *right = parse_number(parser);
        left = create_binary_expr_node(parser->arena, left, right, op.type);
    }
    return left;
}
// Parses + and -
ASTNode *parse_addition_and_subtraction(Parser *parser) {
    ASTNode *left = parse_multiplication_and_division(parser);
    while (peek(parser).type == TokenType::PLUS || peek(parser).type == TokenType::MINUS) {
        // "op" is the operator
        const Token op = advance(parser);
        ASTNode *right = parse_multiplication_and_division(parser);
        left = create_binary_expr_node(parser->arena, left, right, op.type);
    }
    return left;
}