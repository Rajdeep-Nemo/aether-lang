#include "ast.hpp"
#include "arena.hpp"
// Factory for signed integer
ASTNode* create_int_node(const size_t line, Arena* arena, const int64_t value) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_I64;
    node->number_literal.i = value;
    node->line = line;
    return node;
}
// Factory for unsigned integer
ASTNode* create_uint_node(const size_t line, Arena* arena, const uint64_t value) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_U64;
    node->number_literal.u = value;
    node->line = line;
    return node;
}
// Factory for floating points
ASTNode* create_double_node(const size_t line, Arena* arena, const double value) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::NUM_LITERAL;
    node->number_literal.number_type = NumberLiteralPayload::NumberType::TYPE_F64;
    node->number_literal.f = value;
    node->line = line;
    return node;
}
// Factory for string literals
ASTNode* create_string_node(Arena* arena, const std::string_view value, const size_t line) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::STRING_LITERAL;
    node->line = line;
    node->string_literal.value = value;
    return node;
}
// Factory for boolean
ASTNode* create_boolean_node(const size_t line, Arena* arena, const bool value) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::BOOLEAN_LITERAL;
    node->boolean_literal.value = value;
    node->line = line;
    return node;
}
// Factory for binary expressions
ASTNode* create_binary_expr_node(const size_t line, Arena* arena, ASTNode* left, ASTNode* right, const TokenType operator_type) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::BINARY_EXPR;
    node->binary_expr.left_node = left;
    node->binary_expr.right_node = right;
    node->binary_expr.operator_type = operator_type;
    node->line = line;
    return node;
}
// Factory for unary expressions
ASTNode* create_unary_expr_node(const size_t line, Arena* arena, ASTNode* right, const TokenType operator_type) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::UNARY_EXPR;
    node->unary_expr.right = right;
    node->unary_expr.operator_type = operator_type;
    node->line = line;
    return node;
}
// Variable declaration
ASTNode* create_var_declaration_node(const size_t line, Arena* arena, const std::string_view var_name, const DataType type_annotation,
                                     ASTNode* value) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::VAR_DECLARATION;
    node->var_declaration.var_name = var_name;
    node->var_declaration.type_annotation = type_annotation;
    node->var_declaration.value = value;
    node->line = line;
    return node;
}
// Variable access node
ASTNode* create_var_access_node(const size_t line, Arena* arena, const std::string_view var_name) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::VAR_ACCESS;
    node->var_access.var_name = var_name;
    node->line = line;
    return node;
}
// Variable assignment node
ASTNode* create_assignment_node(const size_t line, Arena* arena, const std::string_view var_name, ASTNode* value) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::ASSIGNMENT_EXPR;
    node->var_assignment.var_name = var_name;
    node->var_assignment.value = value;
    node->line = line;
    return node;
}
// Block node
ASTNode* create_block_node(Arena* arena, ASTNode** statements, const size_t count, const size_t line) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::BLOCK_STATEMENT;
    node->line = line;
    node->block_statement.statements = statements;
    node->block_statement.count = count;
    return node;
}
// Function call node
ASTNode* create_function_call_node(size_t line, Arena* arena, ASTNode* callee, ASTNode** arguments, size_t arg_count) {
    void* memory = alloc_arena(arena, sizeof(ASTNode));
    auto* node = static_cast<ASTNode*>(memory);
    node->node_type = NodeType::FUNCTION_CALL;
    node->line = line;
    node->function_call.callee = callee;
    node->function_call.arguments = arguments;
    node->function_call.arg_count = arg_count;
    return node;
}