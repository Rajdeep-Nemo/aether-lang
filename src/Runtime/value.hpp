#ifndef VALUE_HPP
#define VALUE_HPP
#include <cstdint>

enum class ValueType {
    VAL_INT,
    VAL_UINT,
    VAL_FLOAT,
    VAL_CHAR,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_NIL,
};
struct RuntimeValue {
    ValueType type;
    union {
        int64_t i;
        uint64_t u;
        double f;
        char c;
        char* s;
        bool b;
        void* nil;
    };
};

#endif
