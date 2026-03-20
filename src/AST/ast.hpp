#ifndef AST_HPP
#define AST_HPP

#include <cstdint>
#include <token.hpp>

enum class NodeType {
    NUM_LITERAL,
    BINARY_EXPR
};

struct ASTNode;

struct NumberLiteralPayload {
    enum class NumberType {
        TYPE_I8,
        TYPE_I16,
        TYPE_I32,
        TYPE_I64,
        TYPE_U8,
        TYPE_U16,
        TYPE_U32,
        TYPE_U64,
        TYPE_F32,
        TYPE_F64
    };
    union {
        int64_t i;
        uint64_t u;
        double f;
    };

    NumberType number_type;
};

struct BinaryExprPayload {
    ASTNode *left_node;
    ASTNode *right_node;
    TokenType operator_type;
};

struct ASTNode {
    NodeType node_type;
    union {
        NumberLiteralPayload number_literal;
        BinaryExprPayload binary_expr;
    };
};

#endif
