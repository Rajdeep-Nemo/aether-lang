#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "value.hpp"
#include <string>
#include <unordered_map>

struct Environment {
    std::unordered_map<std::string, RuntimeValue> values;
    Environment* enclosing = nullptr;
    Environment() = default;
    Environment(Environment* enclosing) : enclosing(enclosing) {}
    void define(const std::string& name, RuntimeValue value);
    RuntimeValue get(const std::string& name, size_t line);
    void assign(const std::string& name, RuntimeValue value, size_t line);
};

#endif
