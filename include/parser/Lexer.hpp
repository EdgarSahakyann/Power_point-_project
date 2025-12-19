#pragma once

#include "Token.hpp"
#include <vector>
#include <string>

// Lexer uses the shared Token type declared in Token.hpp
class Lexer {
public:
    std::vector<Token> tokenize(const std::vector<std::string>& words) const;
private:
    bool isNumber(const std::string& str) const;
    bool isFloat(const std::string& str) const;
    bool isFlag(const std::string& str) const;
};