#include "evaluator.hpp"
#include "ast.hpp"

#include <iostream>
RuntimeValue evaluate(const ASTNode *node) {
    if (node == nullptr) {
        return RuntimeValue{ValueType::VAL_NIL};
    }
    if (node->node_type == NodeType::NUM_LITERAL) {
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_I64) {
            const RuntimeValue runtime_int{ValueType::VAL_INT, node->number_literal.i};
            return runtime_int;
        }
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_U64) {
            RuntimeValue runtime_uint{};
            runtime_uint.type = ValueType::VAL_UINT;
            runtime_uint.u = node->number_literal.u;
            return runtime_uint;
        }
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_F64) {
            RuntimeValue runtime_float{};
            runtime_float.type = ValueType::VAL_FLOAT;
            runtime_float.f = node->number_literal.f;
            return runtime_float;
        }
    }
    if (node->node_type == NodeType::BINARY_EXPR) {
        const RuntimeValue left_val = evaluate(node->binary_expr.left_node);
        const RuntimeValue right_val = evaluate(node->binary_expr.right_node);

        if (left_val.type == ValueType::VAL_FLOAT || right_val.type == ValueType::VAL_FLOAT) {
            double l_val = 0.0;
            if (left_val.type == ValueType::VAL_FLOAT) {
                l_val = left_val.f;
            } else if (left_val.type == ValueType::VAL_INT) {
                l_val = static_cast<double>(left_val.i);
            } else if (left_val.type == ValueType::VAL_UINT) {
                l_val = static_cast<double>(left_val.u);
            }
            double r_val = 0.0;
            if (right_val.type == ValueType::VAL_FLOAT) {
                r_val = right_val.f;
            } else if (right_val.type == ValueType::VAL_INT) {
                r_val = static_cast<double>(right_val.i);
            } else if (right_val.type == ValueType::VAL_UINT) {
                r_val = static_cast<double>(right_val.u);
            }
            switch (node->binary_expr.operator_type) {
            case TokenType::PLUS: {
                RuntimeValue add{};
                add.type = ValueType::VAL_FLOAT;
                add.f = l_val + r_val;
                return add;
            }
            case TokenType::MINUS: {
                RuntimeValue sub{};
                sub.type = ValueType::VAL_FLOAT;
                sub.f = l_val - r_val;
                return sub;
            }
            case TokenType::STAR: {
                RuntimeValue mul{};
                mul.type = ValueType::VAL_FLOAT;
                mul.f = l_val * r_val;
                return mul;
            }
            case TokenType::SLASH: {
                RuntimeValue div{};
                div.type = ValueType::VAL_FLOAT;
                div.f = l_val / r_val;
                return div;
            }
            default: {
                RuntimeValue nil{};
                nil.type = ValueType::VAL_NIL;
                return nil;
            }
            }
        }
        if (left_val.type == ValueType::VAL_UINT && right_val.type == ValueType::VAL_UINT) {
            uint64_t l_val = left_val.u;
            uint64_t r_val = right_val.u;
            switch (node->binary_expr.operator_type) {
            case TokenType::PLUS: {
                RuntimeValue add{};
                add.type = ValueType::VAL_UINT;
                add.u = l_val + r_val;
                return add;
            }
            case TokenType::MINUS: {
                RuntimeValue sub{};
                sub.type = ValueType::VAL_UINT;
                sub.u = l_val - r_val;
                return sub;
            }
            case TokenType::STAR: {
                RuntimeValue mul{};
                mul.type = ValueType::VAL_UINT;
                mul.u = l_val * r_val;
                return mul;
            }
            case TokenType::SLASH: {
                RuntimeValue div{};
                div.type = ValueType::VAL_UINT;
                div.u = l_val / r_val;
                return div;
            }
            default: {
                RuntimeValue nil{};
                nil.type = ValueType::VAL_NIL;
                return nil;
            }
            }
        }
        if (left_val.type == ValueType::VAL_INT && right_val.type == ValueType::VAL_INT) {
            int64_t l_val = left_val.i;
            int64_t r_val = right_val.i;
            switch (node->binary_expr.operator_type) {
            case TokenType::PLUS: {
                RuntimeValue add{};
                add.type = ValueType::VAL_INT;
                add.i = l_val + r_val;
                return add;
            }
            case TokenType::MINUS: {
                RuntimeValue sub{};
                sub.type = ValueType::VAL_INT;
                sub.i = l_val - r_val;
                return sub;
            }
            case TokenType::STAR: {
                RuntimeValue mul{};
                mul.type = ValueType::VAL_INT;
                mul.i = l_val * r_val;
                return mul;
            }
            case TokenType::SLASH: {
                RuntimeValue div{};
                div.type = ValueType::VAL_INT;
                div.i = l_val / r_val;
                return div;
            }
            default: {
                RuntimeValue nil{};
                nil.type = ValueType::VAL_NIL;
                return nil;
            }
            }
        }
        std::cerr << "Runtime Error: Type mismatch. Cannot implicitly mix signed and unsigned integers. Cast explicitly.\n" << std::endl;
        RuntimeValue nil{};
        nil.type = ValueType::VAL_NIL;
        return nil;
    }
    return RuntimeValue{ValueType::VAL_NIL};
}

