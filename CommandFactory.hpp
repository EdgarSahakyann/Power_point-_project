#pragma once

#include "parser/Parser.hpp"
#include "core/SlideFactory.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "serialization/JsonSerializer.hpp"

class CommandFactory {
public:
    CommandFactory(SlideFactory& slideFactory, ISlideRepository& repo, JsonSerializer& serializer);
    std::unique_ptr<ICommand> createCommand(const std::string& commandStr) const;
    void displayHelp(const std::string& command = "") const;

private:
    SlideFactory& slideFactory_;
    ISlideRepository& repo_;
    JsonSerializer& serializer_;
    Lexer lexer_;
    Parser parser_;
};