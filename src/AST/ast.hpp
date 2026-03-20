#ifndef AST_HPP
#define AST_HPP

#include <cstdint>
#include <token.hpp>

struct ASTNode;

// The "tag" that identifies the specific type of this AST node (e.g., Number, Binary Expr)
// so the interpreter/compiler knows which payload to safely read from the union.
enum class NodeType {
    NUM_LITERAL,
    BINARY_EXPR
};

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

// The master tagged union representing a single node in the syntax tree.
// It guarantees every node uses the exact same amount of memory for lightning-fast Arena allocation.
struct ASTNode {
    NodeType node_type;
    union {
        NumberLiteralPayload number_literal;
        BinaryExprPayload binary_expr;
    };
};

#endif
