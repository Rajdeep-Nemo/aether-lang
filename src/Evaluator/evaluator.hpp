#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP
#include "ast.hpp"
#include "environment.hpp"
#include "value.hpp"

// Forward declare ASTNode
struct ASTNode;
// Evaluates an AST and returns the mathematical result
RuntimeValue evaluate(const ASTNode* node, Environment* env);

#endif // EVALUATOR_HPP