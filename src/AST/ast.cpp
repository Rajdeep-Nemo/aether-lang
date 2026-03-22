#include "ast.hpp"
#include "arena.hpp"
// Factory for signed integer
ASTNode *create_int_node(Arena *arena, const int64_t value) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_I64;
    node->number_literal.i = value;
    return node;
}
// Factory for unsigned integer
ASTNode *create_uint_node(Arena *arena, const uint64_t value) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_U64;
    node->number_literal.u = value;
    return node;
}
// Factory for floating points
ASTNode *create_double_node(Arena *arena, const double value) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_F64;
    node->number_literal.f = value;
    return node;
}
// Factory for boolean
ASTNode *create_boolean_node(Arena *arena,const bool value) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::BOOLEAN_LITERAL;
    node->boolean_literal.value = value;
    return node;
}
// Factory for binary expressions
ASTNode *create_binary_expr_node(Arena *arena, ASTNode *left, ASTNode *right, const TokenType operator_type) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::BINARY_EXPR;
    node->binary_expr.left_node = left;
    node->binary_expr.right_node = right;
    node->binary_expr.operator_type = operator_type;
    return node;
}
// Factory for unary expressions
ASTNode *create_unary_expr_node(Arena *arena, ASTNode *right, TokenType operator_type) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::UNARY_EXPR;
    node->unary_expr.right = right;
    node->unary_expr.operator_type = operator_type;
    return node;
}
// Variable declaration
ASTNode *create_var_declaration_node(Arena *arena,const std::string_view var_name,const DataType type_annotation, ASTNode *value) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::VAR_DECLARATION;
    node->var_declaration.var_name = var_name;
    node->var_declaration.type_annotation = type_annotation;
    node->var_declaration.value = value;
    return node;
}
// Variable access node
ASTNode *create_var_access_node(Arena *arena, std::string_view var_name) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::VAR_ACCESS;
    node->var_access.var_name = var_name;
    return node;
}
// Variable assignment node
ASTNode *create_assignment_node(Arena *arena, std::string_view var_name, ASTNode *value) {
    void *memory = alloc_arena(arena, sizeof(ASTNode));
    auto *node = static_cast<ASTNode *>(memory);
    node->node_type = NodeType::ASSIGNMENT_EXPR;
    node->var_assignment.var_name = var_name;
    node->var_assignment.value = value;
    return node;
}