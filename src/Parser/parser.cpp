#include "parser.hpp"
#include "error.hpp"
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
// Returns the DataType corresponding to the TokenType
DataType get_data_type(const TokenType type) {
    switch (type) {
    case TokenType::I8:
        return DataType::TYPE_I8;
    case TokenType::I16:
        return DataType::TYPE_I16;
    case TokenType::I32:
        return DataType::TYPE_I32;
    case TokenType::I64:
        return DataType::TYPE_I64;
    case TokenType::U8:
        return DataType::TYPE_U8;
    case TokenType::U16:
        return DataType::TYPE_U16;
    case TokenType::U32:
        return DataType::TYPE_U32;
    case TokenType::U64:
        return DataType::TYPE_U64;
    case TokenType::F32:
        return DataType::TYPE_F32;
    case TokenType::F64:
        return DataType::TYPE_F64;
    case TokenType::BOOL:
        return DataType::TYPE_BOOL;
    case TokenType::CHAR:
        return DataType::TYPE_CHAR;
    case TokenType::STRING:
        return DataType::TYPE_STRING;
    default:
        return DataType::TYPE_UNKNOWN; // Fallback
    }
}
// Parse the tokens into AST
ASTNode *parse(Parser *parser) {
    return parse_statement(parser);
}
// Parses unary
ASTNode *parse_unary(Parser *parser) {
    if (peek(parser).type == TokenType::MINUS) {
        const Token op = advance(parser);
        ASTNode *right = parse_unary(parser);
        const size_t line = parser->tokens[parser->current_position].line;
        return create_unary_expr_node(line, parser->arena, right, op.type);
    }
    return parse_primary(parser);
}
// Parses comparison
ASTNode *parse_comparison(Parser *parser) {
    ASTNode *left = parse_addition_and_subtraction(parser);
    while (peek(parser).type == TokenType::EQUAL_EQUAL ||
           peek(parser).type == TokenType::BANG_EQUAL || peek(parser).type == TokenType::LESS ||
           peek(parser).type == TokenType::LESS_EQUAL || peek(parser).type == TokenType::GREATER ||
           peek(parser).type == TokenType::GREATER_EQUAL) {

        const Token op = advance(parser);
        ASTNode *right = parse_addition_and_subtraction(parser);
        const size_t line = parser->tokens[parser->current_position].line;
        left = create_binary_expr_node(line, parser->arena, left, right, op.type);
    }
    return left;
}
// Parses a number literal and parentheses - '(' and ')'
ASTNode *parse_primary(Parser *parser) {
    const Token current = peek(parser);
    if (current.type == TokenType::INT_LITERAL) {
        advance(parser);
        const size_t line = parser->tokens[parser->current_position].line;
        return create_int_node(line, parser->arena, std::stoll(current.lexeme));
    }
    if (current.type == TokenType::FLOAT_LITERAL) {
        advance(parser);
        const size_t line = parser->tokens[parser->current_position].line;
        return create_double_node(line, parser->arena, std::stod(current.lexeme));
    }
    if (current.type == TokenType::LEFT_PAREN) {
        advance(parser);
        ASTNode *inner_expr = parse_addition_and_subtraction(parser);
        if (peek(parser).type != TokenType::RIGHT_PAREN) {
            return nullptr;
        }
        advance(parser);
        return inner_expr;
    }
    if (current.type == TokenType::TRUE) {
        advance(parser);
        const size_t line = parser->tokens[parser->current_position].line;
        return create_boolean_node(line, parser->arena, true);
    }
    if (current.type == TokenType::FALSE) {
        advance(parser);
        const size_t line = parser->tokens[parser->current_position].line;
        return create_boolean_node(line, parser->arena, false);
    }
    if (current.type == TokenType::IDENTIFIER) {
        const std::string_view safe_lexeme = parser->tokens[parser->current_position].lexeme;
        advance(parser);
        const size_t line = parser->tokens[parser->current_position].line;
        return create_var_access_node(line, parser->arena, safe_lexeme);
    }
    return nullptr;
}
// Parses * and /
ASTNode *parse_multiplication_and_division(Parser *parser) {
    ASTNode *left = parse_unary(parser);
    while (peek(parser).type == TokenType::STAR || peek(parser).type == TokenType::SLASH) {
        // "op" is the operator
        const Token op = advance(parser);
        ASTNode *right = parse_unary(parser);
        const size_t line = parser->tokens[parser->current_position].line;
        left = create_binary_expr_node(line, parser->arena, left, right, op.type);
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
        const size_t line = parser->tokens[parser->current_position].line;
        left = create_binary_expr_node(line, parser->arena, left, right, op.type);
    }
    return left;
}
// Parses a statement
ASTNode *parse_statement(Parser *parser) {
    if (peek(parser).type == TokenType::LET) {
        advance(parser);

        const Token &name_token = parser->tokens[parser->current_position];
        advance(parser);

        if (peek(parser).type == TokenType::COLON) {
            advance(parser);
        } else {
            report_parser_error(peek(parser), "Expected ':' after variable name.");
            return nullptr;
        }

        const Token type_token = advance(parser);
        const DataType var_type = get_data_type(type_token.type);

        if (var_type == DataType::TYPE_UNKNOWN) {
            report_parser_error(peek(parser), "Unknown type annotation.");
            return nullptr;
        }

        ASTNode *expr = nullptr;
        if (peek(parser).type == TokenType::EQUAL) {
            advance(parser);
            expr = parse_comparison(parser);
        }

        if (peek(parser).type == TokenType::SEMICOLON) {
            advance(parser);
        } else {
            report_parser_error(peek(parser), "Expected ';' at the end of declaration.");
            return nullptr;
        }
        const size_t line = parser->tokens[parser->current_position].line;
        return create_var_declaration_node(line, parser->arena, name_token.lexeme, var_type, expr);
    }

    ASTNode *expr = parse_assignment(parser);
    // Consumes the last semicolon, if absent throws error
    if (peek(parser).type == TokenType::SEMICOLON) {
        advance(parser);
    } else {
        report_parser_error(peek(parser), "Expected ';' at the end of statement.");
        return nullptr;
    }
    return expr;
}
// Parses an assignment
ASTNode *parse_assignment(Parser *parser) {

    ASTNode *expr = parse_comparison(parser);

    if (peek(parser).type == TokenType::EQUAL) {
        advance(parser);

        ASTNode *value = parse_comparison(parser);

        if (expr != nullptr && expr->node_type == NodeType::VAR_ACCESS) {
            std::string_view var_name = expr->var_access.var_name;
            const size_t line = parser->tokens[parser->current_position].line;
            return create_assignment_node(line, parser->arena, var_name, value);
        }
        report_parser_error(peek(parser), "Invalid assignment target.");
        return nullptr;
    }

    return expr;
}
// Synchronize the parser after a syntax error (Heart attack stopper)
void synchronize(Parser *parser) {
    // Consume the token that caused the error
    advance(parser);
    // Keep throwing away tokens until a safe boundary is found
    while (!is_at_end(parser)) {
        // If the token just thrown away was a semicolon, the previous statement is over.
        if (parser->tokens[parser->current_position - 1].type == TokenType::SEMICOLON) {
            return;
        }
        // If the NEXT token is 'let', a new statement is starting.
        if (peek(parser).type == TokenType::LET) {
            return;
        }
        // Otherwise, throw the token away and keep looking
        advance(parser);
    }
}