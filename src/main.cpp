#include "AST/ast.hpp"
#include "Builtins/builtins.hpp"
#include "Evaluator/evaluator.hpp"
#include "Lexer/lexer.hpp"
#include "Parser/parser.hpp"
#include "Runtime/environment.hpp"
#include "Runtime/value.hpp"
#include "Utils/arena.hpp"
#include <format>
#include <iostream>
#include <vector>
// To be removed ------------------------------------1
extern std::vector<std::string*> string_pool;
//-------------------------------------------------1
int main(const int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "No input file provided.\n";
        std::cout << std::format("Usage: {} <file.at>\n", argv[0]);
        std::cout << "Program terminated.\n";
        return 1;
    }
    if (argc == 2) {
        // Main start -----------------------------------------------------------------------------------------
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
        Environment global_env = new Environment();

        // 1. Register 'print'
        RuntimeValue print_fn{};
        print_fn.type = ValueType::VAL_NATIVE_FN;
        print_fn.native = builtin_print; // Point it to our C++ wrapper
        global_env.define("print", print_fn);

        // 2. Register 'println'
        RuntimeValue println_fn{};
        println_fn.type = ValueType::VAL_NATIVE_FN;
        println_fn.native = builtin_println;
        global_env.define("println", println_fn);

        // Execution
        while (!is_at_end(&parser)) {
            const ASTNode* root = parse(&parser);
            // If parsing fails, enter panic mode
            if (root == nullptr) {
                // Panic Mode: Fast-forward to the next safe statement
                synchronize(&parser);
                // Skip the evaluator and try to parse the next line
                continue;
            }
            const RuntimeValue result = evaluate(root, &global_env);
            switch (result.type) {
            case ValueType::VAL_I64:
                std::cout << result.i << '\n';
                break;
            case ValueType::VAL_U64:
                std::cout << result.u << '\n';
                break;
            case ValueType::VAL_F64:
                std::cout << result.f << '\n';
                break;
            case ValueType::VAL_STRING:
                std::cout << *result.str_ptr << '\n';
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
        // To be removed after proper garbage collector implementation----------------2
        // Clean up all dynamically allocated Aether strings!
        for (std::string* str : string_pool) {
            delete str;
        }
        string_pool.clear();
        //-------------------------------------------------------------------------2
        return 0;
        // Main end ------------------------------------------------------------------------------------------------------
    }
    if (argc > 2) {
        std::cout << "Too many arguments.\n";
        std::cout << std::format("Usage: {} <file.at>\n", argv[0]);
        std::cout << "Program terminated.\n";
        return 1;
    }
    return 0;
}