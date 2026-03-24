#ifndef VALUE_HPP
#define VALUE_HPP
#include <cstdint>
#include <string>
#include <vector>

enum class ValueType {
    VAL_I8,
    VAL_I16,
    VAL_I32,
    VAL_I64,
    VAL_U8,
    VAL_U16,
    VAL_U32,
    VAL_U64,
    VAL_F32,
    VAL_F64,
    VAL_CHAR,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_NIL,
    VAL_NATIVE_FN
};

struct RuntimeValue;
using NativeFn = RuntimeValue (*)(const std::vector<RuntimeValue>& args, size_t line);

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
