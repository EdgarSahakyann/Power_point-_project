#pragma once

#include "Tokenizer.hpp"
#include "../error/Error.hpp"
#include "../model/SlideFactory.hpp"
#include "../interfaces/ISlideRepository.hpp"
#include "../serialization/JsonSerializer.hpp"
#include <expected>
#include <memory>

class ICommand;

class Parser {
public:
    Parser(SlideFactory& f, ISlideRepository& r, JsonSerializer& s);
    std::expected<std::unique_ptr<ICommand>, Error> parse(std::istream& input);

private:
    SlideFactory& factory_;
    ISlideRepository& repo_;
    JsonSerializer& serializer_;

    std::expected<std::unique_ptr<ICommand>, Error> parseCreate(Tokenizer& t);
    std::expected<std::unique_ptr<ICommand>, Error> parseMove(Tokenizer& t);
    std::expected<std::unique_ptr<ICommand>, Error> parseDisplay();
    std::expected<std::unique_ptr<ICommand>, Error> parseSave(Tokenizer& t);
};