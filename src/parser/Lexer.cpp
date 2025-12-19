#include "../../include/parser/Lexer.hpp"
#include <sstream>
#include <stdexcept>

std::vector<Token> Lexer::tokenize(const std::vector<std::string>& words) const {
    std::vector<Token> tokens;
    for (const auto& word : words) {
        if (isFlag(word)) {
            tokens.emplace_back(Token::Type::Flag, word);
        } 
        else if (isNumber(word)) {
            tokens.emplace_back(Token::Type::Number, word);
        } 
        else if (isFloat(word)) {
            tokens.emplace_back(Token::Type::Float, word);
        } 
        else {
            tokens.emplace_back(Token::Type::Word, word);
        }
    }
 
    return tokens;
}

bool Lexer::isNumber(const std::string& str) const {
    try {
        std::size_t pos;
        std::stoi(str, &pos);
        return pos == str.size();
    } catch (...) {
        return false;
    }
}

bool Lexer::isFloat(const std::string& str) const {
    try {
        std::size_t pos;
        std::stof(str, &pos);
        return pos == str.size();
    } catch (...) {
        return false;
    }
}

bool Lexer::isFlag(const std::string& str) const {
    return str == "--font" || str == "--color" || str == "--size" || str == "--line-width";
}