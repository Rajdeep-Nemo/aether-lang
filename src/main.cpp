#include "AST/ast.hpp"
#include "Evaluator/evaluator.hpp"
#include "Runtime/environment.hpp"
#include "Runtime/value.hpp"
#include "Lexer/lexer.hpp"
#include "Parser/parser.hpp"
#include "Utils/arena.hpp"
#include <format>
#include <iostream>
#include <vector>

int main(const int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "No input file provided.\n";
        std::cout << std::format("Usage: {} <file.at>\n", argv[0]);
        std::cout << "Program terminated.\n";
        return 1;
    }
    if (argc == 2) {
        // First check
        if (!run_file(argv[1])) {
            return 1;
        }
        // Token storage
        std::vector<Token> tokens;
        Token t;
        do {
            t = scan_token();
            tokens.push_back(t);
        } while (t.type != TokenType::END_OF_FILE);
        // Arena allocation
        Arena start_arena{};
        init_arena(&start_arena, 1024 * 1024);
        // Parser initialization
        Parser parser;
        parser.tokens = tokens;
        parser.current_position = 0;
        parser.arena = &start_arena;
        // Environment initialization
        Environment global_env;
        // Execution
        while (!is_at_end(&parser)) {
            const ASTNode *root = parse(&parser);
            // If parsing fails, enter panic mode
            if (root == nullptr) {
                // Panic Mode: Fast-forward to the next safe statement
                synchronize(&parser);
                // Skip the evaluator and try to parse the next line
                continue;
            }
            const RuntimeValue result = evaluate(root, &global_env);
            switch (result.type) {
            case ValueType::VAL_INT:
                std::cout << result.i << '\n';
                break;
            case ValueType::VAL_UINT:
                std::cout << result.u << '\n';
                break;
            case ValueType::VAL_FLOAT:
                std::cout << result.f << '\n';
                break;
            case ValueType::VAL_BOOLEAN:
                std::cout << (result.b ? "true" : "false") << '\n';
                break;
            case ValueType::VAL_NIL:
                std::cout << "NIL\n";
                break;
            default:
                std::cout << "Unknown Type\n";
                break;
            }
        }
        // Arena deallocation
        free_arena(&start_arena);
        return 0;
    }
    if (argc > 2) {
        std::cout << "Too many arguments.\n";
        std::cout << std::format("Usage: {} <file.at>\n", argv[0]);
        std::cout << "Program terminated.\n";
        return 1;
    }
    return 0;
}