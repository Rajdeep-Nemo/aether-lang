#include "environment.hpp"
#include "error.hpp"

void Environment::define(const std::string &name,const RuntimeValue value) {
    values[name] = value;
}

RuntimeValue Environment::get(const std::string &name, const size_t line) {
    if (values.contains(name)) {
        return values[name];
    }
    if (enclosing != nullptr) {
        return enclosing->get(name, line);
    }
    report_runtime_error(line, "Undefined variable '" + name + "'.");
    return RuntimeValue{ValueType::VAL_NIL};
}

void Environment::assign(const std::string &name,const RuntimeValue value, const size_t line) {
    if (values.contains(name)) {
        values[name] = value;
        return;
    }
    if (enclosing != nullptr) {
        enclosing->assign(name, value, line);
        return;
    }
    report_runtime_error(line, "Undefined variable '" + name + "'.");
}