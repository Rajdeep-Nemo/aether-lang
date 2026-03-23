#include "builtins.hpp"
#include "Utils/error.hpp"
#include <iostream>
#include <string>
#include <vector>

// Helper 1: strictly match the string tag to the Aether ValueType
bool match_tag(const std::string& tag, ValueType type) {
    if (tag == "i8" && type == ValueType::VAL_I8) return true;
    if (tag == "i16" && type == ValueType::VAL_I16) return true;
    if (tag == "i32" && type == ValueType::VAL_I32) return true;
    if (tag == "i64" && type == ValueType::VAL_I64) return true;

    if (tag == "u8" && type == ValueType::VAL_U8) return true;
    if (tag == "u16" && type == ValueType::VAL_U16) return true;
    if (tag == "u32" && type == ValueType::VAL_U32) return true;
    if (tag == "u64" && type == ValueType::VAL_U64) return true;

    if (tag == "f32" && type == ValueType::VAL_F32) return true;
    if (tag == "f64" && type == ValueType::VAL_F64) return true;

    if (tag == "char" && type == ValueType::VAL_CHAR) return true;
    if (tag == "string" && type == ValueType::VAL_STRING) return true;
    if (tag == "bool" && type == ValueType::VAL_BOOLEAN) return true;

    return false;
}

// Helper 2: Print the raw data based on its type
void print_raw_value(const RuntimeValue& val) {
    switch (val.type) {
        // All signed integers use the 64-bit 'i' payload
        case ValueType::VAL_I8: case ValueType::VAL_I16:
        case ValueType::VAL_I32: case ValueType::VAL_I64:
            std::cout << val.i; break;

        // All unsigned integers use the 64-bit 'u' payload
        case ValueType::VAL_U8: case ValueType::VAL_U16:
        case ValueType::VAL_U32: case ValueType::VAL_U64:
            std::cout << val.u; break;

        case ValueType::VAL_F32: case ValueType::VAL_F64:
            std::cout << val.f; break;

        case ValueType::VAL_CHAR:
            std::cout << val.c; break;

        case ValueType::VAL_STRING:
            std::cout << *(val.str_ptr); break;

        case ValueType::VAL_BOOLEAN:
            std::cout << (val.b ? "true" : "false"); break;

        case ValueType::VAL_NIL:
            std::cout << "NIL"; break;

        default:
            std::cout << "<unknown>"; break;
    }
}

RuntimeValue builtin_print(const std::vector<RuntimeValue>& args, size_t line) {
    RuntimeValue nil{};
    nil.type = ValueType::VAL_NIL;

    if (args.empty()) return nil;

    // If the first argument isn't a string, just print it normally (fallback)
    if (args[0].type != ValueType::VAL_STRING) {
        for (const auto& arg : args) {
            print_raw_value(arg);
        }
        return nil;
    }

    std::string format_str = *(args[0].str_ptr);
    size_t arg_index = 1; // Start looking at the 2nd argument

    for (size_t i = 0; i < format_str.length(); ++i) {
        if (format_str[i] == '{') {
            size_t end_brace = format_str.find('}', i);
            if (end_brace == std::string::npos) {
                report_runtime_error(line, "Unmatched '{' in format string.");
                return nil;
            }

            // Extract the tag (e.g., "i32" from "{i32}")
            std::string tag = format_str.substr(i + 1, end_brace - i - 1);

            // Check if user forgot to pass enough arguments
            if (arg_index >= args.size()) {
                report_runtime_error(line, "Not enough arguments for print format.");
                return nil;
            }

            // STRICT TYPE CHECK
            if (!match_tag(tag, args[arg_index].type)) {
                report_runtime_error(line, "Type mismatch. Expected {" + tag + "} but got a different type.");
                return nil;
            }

            // If it passed the check, print it!
            print_raw_value(args[arg_index]);
            arg_index++;

            i = end_brace; // Jump the loop past the '}'
        } else {
            // Print normal string characters
            std::cout << format_str[i];
        }
    }

    return nil;
}

// println is just print + a newline!
RuntimeValue builtin_println(const std::vector<RuntimeValue>& args, size_t line) {
    builtin_print(args, line);
    std::cout << std::endl;

    RuntimeValue nil{};
    nil.type = ValueType::VAL_NIL;
    return nil;
}