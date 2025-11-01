#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "Token.hpp"
#include <istream>

class Tokenizer {
public:
    explicit Tokenizer(std::istream& input);
    Token nextToken();
    Token peek() const;

    bool isEOF() const noexcept;
    bool isError() const noexcept;
    void reset();

private:
    std::istream& input_;
    int lookahead_;
    bool error_ = false;

    int getChar();
    void putBack(int ch);
};

#endif // TOKENIZER_HPP