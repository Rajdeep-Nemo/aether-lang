#include "evaluator.hpp"
#include "ast.hpp"
#include "environment.hpp"
#include "error.hpp"

RuntimeValue evaluate(const ASTNode* node, Environment* env) {
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
    if (node->node_type == NodeType::BOOLEAN_LITERAL) {
        RuntimeValue runtime_boolean{};
        runtime_boolean.type = ValueType::VAL_BOOLEAN;
        runtime_boolean.b = node->boolean_literal.value;
        return runtime_boolean;
    }
    if (node->node_type == NodeType::BINARY_EXPR) {
        const RuntimeValue left_val = evaluate(node->binary_expr.left_node, env);
        const RuntimeValue right_val = evaluate(node->binary_expr.right_node, env);

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
                if (r_val == 0.0) {
                    report_runtime_error(node->line, "Division by zero.");
                    return RuntimeValue{ValueType::VAL_NIL};
                }
                RuntimeValue div{};
                div.type = ValueType::VAL_FLOAT;
                div.f = l_val / r_val;
                return div;
            }
            case TokenType::EQUAL_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val == r_val;
                return res;
            }
            case TokenType::BANG_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val != r_val;
                return res;
            }
            case TokenType::LESS: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val < r_val;
                return res;
            }
            case TokenType::LESS_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val <= r_val;
                return res;
            }
            case TokenType::GREATER: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val > r_val;
                return res;
            }
            case TokenType::GREATER_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val >= r_val;
                return res;
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
                if (r_val == 0) {
                    report_runtime_error(node->line, "Division by zero.");
                    return RuntimeValue{ValueType::VAL_NIL};
                }
                RuntimeValue div{};
                div.type = ValueType::VAL_UINT;
                div.u = l_val / r_val;
                return div;
            }
            case TokenType::EQUAL_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val == r_val;
                return res;
            }
            case TokenType::BANG_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val != r_val;
                return res;
            }
            case TokenType::LESS: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val < r_val;
                return res;
            }
            case TokenType::LESS_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val <= r_val;
                return res;
            }
            case TokenType::GREATER: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val > r_val;
                return res;
            }
            case TokenType::GREATER_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val >= r_val;
                return res;
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
                if (r_val == 0) {
                    report_runtime_error(node->line, "Division by zero.");
                    return RuntimeValue{ValueType::VAL_NIL};
                }
                RuntimeValue div{};
                div.type = ValueType::VAL_INT;
                div.i = l_val / r_val;
                return div;
            }
            case TokenType::EQUAL_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val == r_val;
                return res;
            }
            case TokenType::BANG_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val != r_val;
                return res;
            }
            case TokenType::LESS: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val < r_val;
                return res;
            }
            case TokenType::LESS_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val <= r_val;
                return res;
            }
            case TokenType::GREATER: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val > r_val;
                return res;
            }
            case TokenType::GREATER_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val >= r_val;
                return res;
            }
            default: {
                RuntimeValue nil{};
                nil.type = ValueType::VAL_NIL;
                return nil;
            }
            }
        }
        if (left_val.type == ValueType::VAL_BOOLEAN && right_val.type == ValueType::VAL_BOOLEAN) {
            bool l_val = left_val.b;
            bool r_val = right_val.b;
            switch (node->binary_expr.operator_type) {
            case TokenType::EQUAL_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val == r_val;
                return res;
            }
            case TokenType::BANG_EQUAL: {
                RuntimeValue res{};
                res.type = ValueType::VAL_BOOLEAN;
                res.b = l_val != r_val;
                return res;
            }
            default: {
                report_runtime_error(node->line, "Invalid operator for booleans.");
                RuntimeValue nil{};
                nil.type = ValueType::VAL_NIL;
                return nil;
            }
            }
        }
        report_runtime_error(
            node->line,
            "Type mismatch. Cannot implicitly mix signed and unsigned integers. Cast explicitly.");
        RuntimeValue nil{};
        nil.type = ValueType::VAL_NIL;
        return nil;
    }
    if (node->node_type == NodeType::UNARY_EXPR) {
        RuntimeValue right = evaluate(node->unary_expr.right, env);
        if (right.type == ValueType::VAL_INT) {
            RuntimeValue neg{};
            neg.type = ValueType::VAL_INT;
            neg.i = -right.i;
            return neg;
        }
        if (right.type == ValueType::VAL_FLOAT) {
            RuntimeValue neg{};
            neg.type = ValueType::VAL_FLOAT;
            neg.f = -right.f;
            return neg;
        }
        if (right.type == ValueType::VAL_UINT) {
            report_runtime_error(node->line, "Cannot apply unary minus to an unsigned integer.");
            return RuntimeValue{ValueType::VAL_NIL};
        }
    }
    if (node->node_type == NodeType::VAR_DECLARATION) {
        RuntimeValue val{};

        // If initialized (e.g., let x: i32 = 5;)
        if (node->var_declaration.value != nullptr) {
            val = evaluate(node->var_declaration.value, env);
        }
        // If not (e.g., let x: i32;) assign NIL
        else {
            val.type = ValueType::VAL_NIL;
        }

        // Saves it to memory! (Convert string_view to string for the unordered_map key)
        env->define(std::string(node->var_declaration.var_name), val);

        // Statements don't evaluate to a mathematical number, so returns NIL
        return RuntimeValue{ValueType::VAL_NIL};
    }
    if (node->node_type == NodeType::VAR_ACCESS) {
        return env->get(std::string(node->var_access.var_name), node->line);
    }
    if (node->node_type == NodeType::ASSIGNMENT_EXPR) {
        RuntimeValue val = evaluate(node->var_assignment.value, env);
        env->assign(std::string(node->var_assignment.var_name), val, node->line);
        return RuntimeValue{ValueType::VAL_NIL};
    }
    return RuntimeValue{ValueType::VAL_NIL};
}
