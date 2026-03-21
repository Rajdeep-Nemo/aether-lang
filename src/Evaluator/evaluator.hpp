#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP
#include "ast.hpp"
#include "value.hpp"
// Forward declare ASTNode
struct ASTNode;
// Evaluates an AST and returns the mathematical result
RuntimeValue evaluate(const ASTNode *node);

#endif //EVALUATOR_HPP