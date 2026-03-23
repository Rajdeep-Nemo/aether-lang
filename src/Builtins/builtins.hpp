#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include "Runtime/value.hpp"
#include <vector>

// Helper functions
bool match_tag(const std::string& tag, ValueType type);
void print_raw_value(const RuntimeValue& val);

// The actual native functions
RuntimeValue builtin_print(const std::vector<RuntimeValue>& args);
RuntimeValue builtin_println(const std::vector<RuntimeValue>& args);


#endif // BUILTINS_HPP