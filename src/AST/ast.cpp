#include "ast.hpp"
#include "arena.hpp"

// Factory for signed integer
ASTNode *create_int_node(Arena *arena,const int64_t value) {
    void *memory = alloc_arena(arena , sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_I64;
    node->number_literal.i = value;
    return node;
}
// Factory for unsigned integer
ASTNode *create_uint_node(Arena *arena,const uint64_t value) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_U64;
    node->number_literal.u = value;
    return node;
}
// Factory for floating points
ASTNode *create_double_node(Arena *arena,const double value) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_F64;
    node->number_literal.f = value;
    return node;
}
// Factory for binary expressions
ASTNode *create_binary_expr_node(Arena *arena, ASTNode *left, ASTNode *right, TokenType operator_type) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::BINARY_EXPR;
    node->binary_expr.left_node = left;
    node->binary_expr.right_node = right;
    node->binary_expr.operator_type = operator_type;
    return node;
}
