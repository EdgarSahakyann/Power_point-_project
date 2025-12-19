#pragma once

#include <vector>
#include <string>
#include <istream>

// Simple tokenizer used by the Parser implementation in src/parser.
class Tokenizer {
public:
    Tokenizer() = default;
    std::vector<std::string> tokenize(std::istream& input) const;
};