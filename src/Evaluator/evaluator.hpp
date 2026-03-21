#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP
#include "ast.hpp"
// Forward declare ASTNode
struct ASTNode;
// Evaluates an AST and returns the mathematical result
double evaluate(const ASTNode *node);

#endif //EVALUATOR_HPP