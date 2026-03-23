#ifndef VALUE_HPP
#define VALUE_HPP
#include <cstdint>
#include <vector>
#include <string>

enum class ValueType {
    VAL_INT,
    VAL_UINT,
    VAL_FLOAT,
    VAL_CHAR,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_NIL,
    VAL_NATIVE_FN
};

struct RuntimeValue;
using NativeFn = RuntimeValue (*)(const std::vector<RuntimeValue>& args);

struct RuntimeValue {
    ValueType type;
    union {
        int64_t i;
        uint64_t u;
        double f;
        char c;
        std::string* str_ptr;
        bool b;
        void* nil;
        NativeFn native;
    };
};

#endif
