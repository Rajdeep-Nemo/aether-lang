#ifndef AST_HPP
#define AST_HPP

#include <string_view>
#include <cstdint>
#include <token.hpp>

// Forward declaration of the ASTNode struct
struct ASTNode;

// Enum to identify the 'type' of an AST node
enum class NodeType {
    NUM_LITERAL,
    BOOLEAN_LITERAL,
    BINARY_EXPR,
    UNARY_EXPR,
    VAR_DECLARATION,
    VAR_ACCESS,
    ASSIGNMENT_EXPR
};
// Stores the type of the data
enum class DataType {
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_F32,
    TYPE_F64,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_UNKNOWN
};
// Payload for a number literal node
struct NumberLiteralPayload {
    // Enum to specify the type of the number
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
    // Union to store the number value
    union {
        int64_t i;
        uint64_t u;
        double f;
    };

    NumberType number_type;
};
// Payload for a boolean literal node
struct BooleanLiteralPayload {
    bool value;
};
// Payload for a binary expression node
struct BinaryExprPayload {
    ASTNode *left_node;
    ASTNode *right_node;
    TokenType operator_type;
};
// Payload for a unary expression node
struct UnaryExprPayload {
    ASTNode *right;
    TokenType operator_type;
};
// Payload for a variable declaration node
struct VarDeclarationPayload {
    std::string_view var_name;
    DataType type_annotation;
    ASTNode *value;
};
// Payload for a variable access node
struct VarAccessPayload {
    std::string_view var_name;
};
// Payload for an assignment node
struct AssignmentPayload {
    std::string_view var_name;
    ASTNode *value;
};

// The master tagged union representing a single node in the syntax tree.
// It guarantees every node uses the exact same amount of memory for lightning-fast Arena allocation.
struct ASTNode {
    NodeType node_type{};
    size_t line{};
    union {
        NumberLiteralPayload number_literal;
        BooleanLiteralPayload boolean_literal;
        BinaryExprPayload binary_expr;
        UnaryExprPayload unary_expr;
        VarDeclarationPayload var_declaration{};
        VarAccessPayload var_access;
        AssignmentPayload var_assignment;
    };
};

// Forward declaration so the AST knows the Arena exists
struct Arena;

// Factory for signed integer
ASTNode *create_int_node(size_t line, Arena *arena, int64_t value);
// Factory for unsigned integer
ASTNode *create_uint_node(size_t line, Arena *arena, uint64_t value);
// Factory for floating points
ASTNode *create_double_node(size_t line, Arena *arena, double value);
// Factory for boolean
ASTNode *create_boolean_node(size_t line, Arena *arena, bool value);
// Factory for binary expressions
ASTNode *create_binary_expr_node(size_t line, Arena *arena, ASTNode *left, ASTNode *right, TokenType operator_type);
// Factory for unary expressions
ASTNode *create_unary_expr_node(size_t line, Arena *arena, ASTNode *right, TokenType operator_type);
// Variable declaration node
ASTNode *create_var_declaration_node(size_t line, Arena *arena, std::string_view var_name, DataType type_annotation, ASTNode *value);
// Variable access node
ASTNode *create_var_access_node(size_t line, Arena *arena, std::string_view var_name);
// Variable assignment node
ASTNode *create_assignment_node(size_t line, Arena *arena, std::string_view var_name, ASTNode *value);

#endif
