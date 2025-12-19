#include "../../include/parser/Tokenizer.hpp"
#include <sstream>
#include <cctype>

std::vector<std::string> Tokenizer::tokenize(std::istream& input) const {
    std::vector<std::string> tokens;
    std::string line;
    std::getline(input, line);
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
        tokens.push_back(word);
    }
 
    return tokens;
}