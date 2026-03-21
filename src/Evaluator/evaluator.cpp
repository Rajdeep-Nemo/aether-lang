#include "evaluator.hpp"
#include "ast.hpp"
double evaluate(const ASTNode *node) {
    if (node == nullptr) {
        return 0.0;
    }
    if (node->node_type == NodeType::NUM_LITERAL) {
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_I64) {
            return static_cast<double>(node->number_literal.i);
        }
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_U64) {
            return static_cast<double>(node->number_literal.u);
        }
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_F64) {
            return node->number_literal.f;
        }
    }
    if (node->node_type == NodeType::BINARY_EXPR) {
        const double left_val = evaluate(node->binary_expr.left_node);
        const double right_val = evaluate(node->binary_expr.right_node);

        switch (node->binary_expr.operator_type) {
        case TokenType::PLUS: return left_val + right_val;
        case TokenType::MINUS: return left_val - right_val;
        case TokenType::STAR: return left_val * right_val;
        case TokenType::SLASH: return left_val / right_val;
        default: return 0.0;
        }
    }
    return 0.0;
}