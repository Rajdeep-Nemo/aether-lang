#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP
#include "ast.hpp"
#include "environment.hpp"
#include "value.hpp"

// Forward declare ASTNode
struct ASTNode;
// Implicit casting for variables from 64 bit to user desired bits
RuntimeValue cast_with_bounds_check(RuntimeValue val, DataType target_type, size_t line);
// Evaluates an AST and returns the mathematical result
RuntimeValue evaluate(const ASTNode* node, Environment* env);

#endif // EVALUATOR_HPP