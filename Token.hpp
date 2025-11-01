#pragma once

#include <string>

struct Token {
    enum class Type {
        Word,
        Number,
        Float,
        Flag,
        QuoteString,
        End,
        Error
    };

    Type type;
    std::string value;

    Token(Type t = Type::End, std::string v = "")
        : type(t), value(std::move(v)) {}
};