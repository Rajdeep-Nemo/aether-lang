#include "evaluator.hpp"
#include "ast.hpp"
#include "environment.hpp"
#include "error.hpp"
#include <cstdint>
#include <string>
#include <vector>
// To be removed------------------------------------------1
//  Global pool to track dynamically allocated strings
std::vector<std::string*> string_pool;
//-------------------------------------------------------1

// Implicit casting for variables from 64 bit to user desired bits
RuntimeValue cast_with_bounds_check(RuntimeValue val, const DataType target_type, const size_t line) {
    // Checks if it is a string
    if (val.type != ValueType::VAL_I64 && val.type != ValueType::VAL_U64 && val.type != ValueType::VAL_F64) {
        return val;
    }

    // Negative numbers
    if (val.type == ValueType::VAL_I64) {
        const int64_t num = val.i;

        switch (target_type) {
        // i8
        case DataType::TYPE_I8:
            if (num >= INT8_MIN && num <= INT8_MAX) {
                val.type = ValueType::VAL_I8;
                return val;
            }
            report_runtime_error(line, "Value out of bounds for i8.");
            return RuntimeValue{ValueType::VAL_NIL};
        // i16
        case DataType::TYPE_I16:
            if (num >= INT16_MIN && num <= INT16_MAX) {
                val.type = ValueType::VAL_I16;
                return val;
            }
            report_runtime_error(line, "Value out of bounds for i16.");
            return RuntimeValue{ValueType::VAL_NIL};
        // i32
        case DataType::TYPE_I32:
            if (num >= INT32_MIN && num <= INT32_MAX) {
                val.type = ValueType::VAL_I32;
                return val;
            }
            report_runtime_error(line, "Value out of bounds for i32.");
            return RuntimeValue{ValueType::VAL_NIL};
        // i64
        case DataType::TYPE_I64:
            return val;

        // UNSIGNED
        case DataType::TYPE_U8:
        case DataType::TYPE_U16:
        case DataType::TYPE_U32:
        case DataType::TYPE_U64:
            report_runtime_error(line, "Type mismatch: Cannot assign a negative signed value to an unsigned type.");
            return RuntimeValue{ValueType::VAL_NIL};

        default:
            break;
        }
    }

    // Positive numbers
    else if (val.type == ValueType::VAL_U64) {
        const uint64_t num = val.u;

        switch (target_type) {
        // UNSIGNED
        case DataType::TYPE_U8:
            if (num <= UINT8_MAX) {
                val.type = ValueType::VAL_U8;
                return val;
            }
            report_runtime_error(line, "Value out of bounds for u8.");
            return RuntimeValue{ValueType::VAL_NIL};

        case DataType::TYPE_U16:
            if (num <= UINT16_MAX) {
                val.type = ValueType::VAL_U16;
                return val;
            }
            report_runtime_error(line, "Value out of bounds for u16.");
            return RuntimeValue{ValueType::VAL_NIL};

        case DataType::TYPE_U32:
            if (num <= UINT32_MAX) {
                val.type = ValueType::VAL_U32;
                return val;
            }
            report_runtime_error(line, "Value out of bounds for u32.");
            return RuntimeValue{ValueType::VAL_NIL};

        case DataType::TYPE_U64:
            return val;

        // SIGNED (Must check positive limits!)
        case DataType::TYPE_I8:
            if (num <= INT8_MAX) {
                val.type = ValueType::VAL_I8;
                val.i = static_cast<int64_t>(num);
                return val;
            }
            report_runtime_error(line, "Value out of bounds for i8.");
            return RuntimeValue{ValueType::VAL_NIL};

        case DataType::TYPE_I16:
            if (num <= INT16_MAX) {
                val.type = ValueType::VAL_I16;
                val.i = static_cast<int64_t>(num);
                return val;
            }
            report_runtime_error(line, "Value out of bounds for i16.");
            return RuntimeValue{ValueType::VAL_NIL};

        case DataType::TYPE_I32:
            if (num <= INT32_MAX) {
                val.type = ValueType::VAL_I32;
                val.i = static_cast<int64_t>(num);
                return val;
            }
            report_runtime_error(line, "Value out of bounds for i32.");
            return RuntimeValue{ValueType::VAL_NIL};

        case DataType::TYPE_I64:
            if (num <= INT64_MAX) {
                val.type = ValueType::VAL_I64;
                val.i = static_cast<int64_t>(num);
                return val;
            }
            report_runtime_error(line, "Value out of bounds for i64.");
            return RuntimeValue{ValueType::VAL_NIL};

        default:
            break;
        }
    }
    // Floating point
    else if (val.type == ValueType::VAL_F64) {
        const double num = val.f;
        switch (target_type) {
        case DataType::TYPE_F64:
            return val;

        case DataType::TYPE_F32:
            val.type = ValueType::VAL_F32;
            val.f = static_cast<double>(static_cast<float>(num));
            return val;

        // Prevent implicit float-to-int truncation
        case DataType::TYPE_I8:
        case DataType::TYPE_I16:
        case DataType::TYPE_I32:
        case DataType::TYPE_I64:
        case DataType::TYPE_U8:
        case DataType::TYPE_U16:
        case DataType::TYPE_U32:
        case DataType::TYPE_U64:
            report_runtime_error(line, "Type mismatch: Cannot implicitly cast a float to an integer. Use explicit casting to truncate decimals.");
            return RuntimeValue{ValueType::VAL_NIL};

        default:
            break;
        }
    }

    return val;
}

RuntimeValue evaluate(const ASTNode* node, Environment* env) {
    if (node == nullptr) {
        return RuntimeValue{ValueType::VAL_NIL};
    }
    if (node->node_type == NodeType::NUM_LITERAL) {
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_I64) {
            const RuntimeValue runtime_int{ValueType::VAL_I64, node->number_literal.i};
            return runtime_int;
        }
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_U64) {
            RuntimeValue runtime_uint{};
            runtime_uint.type = ValueType::VAL_U64;
            runtime_uint.u = node->number_literal.u;
            return runtime_uint;
        }
        if (node->number_literal.number_type == NumberLiteralPayload::NumberType::TYPE_F64) {
            RuntimeValue runtime_float{};
            runtime_float.type = ValueType::VAL_F64;
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
    if (node->node_type == NodeType::STRING_LITERAL) {
        RuntimeValue val{};
        val.type = ValueType::VAL_STRING;

        std::string_view raw_view = node->string_literal.value;

        // Strip the leading and trailing double quotes
        if (raw_view.length() >= 2 && raw_view.front() == '"' && raw_view.back() == '"') {
            raw_view = raw_view.substr(1, raw_view.length() - 2);
        }

        auto new_str = new std::string(raw_view);
        // To be removed -------------------------------------2
        string_pool.push_back(new_str);
        // to be removed -------------------------------------2
        val.str_ptr = new_str;
        return val;
    }
    if (node->node_type == NodeType::BINARY_EXPR) {
        const RuntimeValue left_val = evaluate(node->binary_expr.left_node, env);
        const RuntimeValue right_val = evaluate(node->binary_expr.right_node, env);

        // If either side is NIL, an error was already reported (like an undefined variable).
        // Fail silently and pass the NIL up the chain.
        if (left_val.type == ValueType::VAL_NIL || right_val.type == ValueType::VAL_NIL) {
            return RuntimeValue{ValueType::VAL_NIL};
        }

        if (left_val.type == ValueType::VAL_F64 || right_val.type == ValueType::VAL_F64) {
            double l_val = 0.0;
            if (left_val.type == ValueType::VAL_F64) {
                l_val = left_val.f;
            } else if (left_val.type == ValueType::VAL_I64) {
                l_val = static_cast<double>(left_val.i);
            } else if (left_val.type == ValueType::VAL_U64) {
                l_val = static_cast<double>(left_val.u);
            }
            double r_val = 0.0;
            if (right_val.type == ValueType::VAL_F64) {
                r_val = right_val.f;
            } else if (right_val.type == ValueType::VAL_I64) {
                r_val = static_cast<double>(right_val.i);
            } else if (right_val.type == ValueType::VAL_U64) {
                r_val = static_cast<double>(right_val.u);
            }
            switch (node->binary_expr.operator_type) {
            case TokenType::PLUS: {
                RuntimeValue add{};
                add.type = ValueType::VAL_F64;
                add.f = l_val + r_val;
                return add;
            }
            case TokenType::MINUS: {
                RuntimeValue sub{};
                sub.type = ValueType::VAL_F64;
                sub.f = l_val - r_val;
                return sub;
            }
            case TokenType::STAR: {
                RuntimeValue mul{};
                mul.type = ValueType::VAL_F64;
                mul.f = l_val * r_val;
                return mul;
            }
            case TokenType::SLASH: {
                if (r_val == 0.0) {
                    report_runtime_error(node->line, "Division by zero.");
                    return RuntimeValue{ValueType::VAL_NIL};
                }
                RuntimeValue div{};
                div.type = ValueType::VAL_F64;
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
        if (left_val.type == ValueType::VAL_U64 && right_val.type == ValueType::VAL_U64) {
            uint64_t l_val = left_val.u;
            uint64_t r_val = right_val.u;
            switch (node->binary_expr.operator_type) {
            case TokenType::PLUS: {
                RuntimeValue add{};
                add.type = ValueType::VAL_U64;
                add.u = l_val + r_val;
                return add;
            }
            case TokenType::MINUS: {
                RuntimeValue sub{};
                sub.type = ValueType::VAL_U64;
                sub.u = l_val - r_val;
                return sub;
            }
            case TokenType::STAR: {
                RuntimeValue mul{};
                mul.type = ValueType::VAL_U64;
                mul.u = l_val * r_val;
                return mul;
            }
            case TokenType::SLASH: {
                if (r_val == 0) {
                    report_runtime_error(node->line, "Division by zero.");
                    return RuntimeValue{ValueType::VAL_NIL};
                }
                RuntimeValue div{};
                div.type = ValueType::VAL_U64;
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
        if (left_val.type == ValueType::VAL_I64 && right_val.type == ValueType::VAL_I64) {
            int64_t l_val = left_val.i;
            int64_t r_val = right_val.i;
            switch (node->binary_expr.operator_type) {
            case TokenType::PLUS: {
                RuntimeValue add{};
                add.type = ValueType::VAL_I64;
                add.i = l_val + r_val;
                return add;
            }
            case TokenType::MINUS: {
                RuntimeValue sub{};
                sub.type = ValueType::VAL_I64;
                sub.i = l_val - r_val;
                return sub;
            }
            case TokenType::STAR: {
                RuntimeValue mul{};
                mul.type = ValueType::VAL_I64;
                mul.i = l_val * r_val;
                return mul;
            }
            case TokenType::SLASH: {
                if (r_val == 0) {
                    report_runtime_error(node->line, "Division by zero.");
                    return RuntimeValue{ValueType::VAL_NIL};
                }
                RuntimeValue div{};
                div.type = ValueType::VAL_I64;
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
        report_runtime_error(node->line, "Type mismatch. Cannot implicitly mix signed and unsigned integers. Cast explicitly.");
        RuntimeValue nil{};
        nil.type = ValueType::VAL_NIL;
        return nil;
    }
    if (node->node_type == NodeType::UNARY_EXPR) {
        RuntimeValue right = evaluate(node->unary_expr.right, env);
        if (right.type == ValueType::VAL_I64) {
            RuntimeValue neg{};
            neg.type = ValueType::VAL_I64;
            neg.i = -right.i;
            return neg;
        }
        if (right.type == ValueType::VAL_F64) {
            RuntimeValue neg{};
            neg.type = ValueType::VAL_F64;
            neg.f = -right.f;
            return neg;
        }
        if (right.type == ValueType::VAL_U64) {
            // The absolute maximum absolute value for an i64 is 9223372036854775808
            if (right.u == 9223372036854775808ULL) {
                RuntimeValue neg{};
                neg.type = ValueType::VAL_I64;
                neg.i = INT64_MIN;
                return neg;
            }
            if (right.u < 9223372036854775808ULL) {
                RuntimeValue neg{};
                neg.type = ValueType::VAL_I64;
                neg.i = -static_cast<int64_t>(right.u);
                return neg;
            }
            report_runtime_error(node->line, "Value too large to become a negative number.");
            return RuntimeValue{ValueType::VAL_NIL};
        }
    }
    if (node->node_type == NodeType::VAR_DECLARATION) {
        RuntimeValue val{};
        // If initialized (e.g., let x: i32 = 5;)
        if (node->var_declaration.value != nullptr) {
            val = evaluate(node->var_declaration.value, env);
            // Checking happens only if a type is provided
            if (node->var_declaration.type_annotation != DataType::TYPE_UNKNOWN) {
                // Uses the cast function with bounds checking
                val = cast_with_bounds_check(val, node->var_declaration.type_annotation, node->line);
                // If NIL, means out of bounds error
                if (val.type == ValueType::VAL_NIL) {
                    return val;
                }
            }
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
    if (node->node_type == NodeType::BLOCK_STATEMENT) {
        Environment block_env(env);

        for (size_t i = 0; i < node->block_statement.count; ++i) {
            evaluate(node->block_statement.statements[i], &block_env);
        }

        // When this 'if' block ends, C++ automatically destroys 'block_env',
        // taking all local variables with it.
        return RuntimeValue{ValueType::VAL_NIL};
    }
    if (node->node_type == NodeType::FUNCTION_CALL) {

        RuntimeValue callee = evaluate(node->function_call.callee, env);

        if (callee.type == ValueType::VAL_NIL) {
            return RuntimeValue{ValueType::VAL_NIL};
        }

        if (callee.type != ValueType::VAL_NATIVE_FN) {
            report_runtime_error(node->line, "Can only call functions.");
            return RuntimeValue{ValueType::VAL_NIL};
        }

        std::vector<RuntimeValue> args;
        for (size_t i = 0; i < node->function_call.arg_count; ++i) {
            args.push_back(evaluate(node->function_call.arguments[i], env));
        }

        return callee.native(args, node->line);
    }
    return RuntimeValue{ValueType::VAL_NIL};
}
