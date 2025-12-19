#pragma once

#include "Tokenizer.hpp"
#include "Lexer.hpp"
#include "Token.hpp"
#include "../interfaces/ICommand.hpp"
#include "../core/SlideFactory.hpp"
#include "../interfaces/ISlideRepository.hpp"
#include "../serialization/JsonSerializer.hpp"
#include <memory>
#include <istream>
#include <string>

enum class State {
    START,
    CREATE_TITLE,
    CREATE_CONTENT,
    CREATE_THEME,
    ADDTEXT_ID,
    ADDTEXT_TEXT,
    ADDTEXT_SIZE_FLAG,
    ADDTEXT_SIZE_VALUE,
    ADDTEXT_FONT_FLAG,
    ADDTEXT_FONT_VALUE,
    ADDTEXT_COLOR_FLAG,
    ADDTEXT_COLOR_VALUE,
    ADDTEXT_LINEWIDTH_FLAG,
    ADDTEXT_LINEWIDTH_VALUE,
    ADDSHAPE_ID,
    ADDSHAPE_TYPE,
    ADDSHAPE_SCALE,
    MOVE_FROM,
    MOVE_TO,
    REMOVETEXT_ID,
    REMOVETEXT_INDEX,
    REMOVESHAPE_ID,
    REMOVESHAPE_INDEX,
    SAVE_FILE,
    LOAD_FILE,
    DISPLAY,
    HELP,
    ERROR
};

class Parser {
public:
    Parser(SlideFactory& f, ISlideRepository& r, JsonSerializer& s);
    std::unique_ptr<ICommand> parse(std::istream& input) const;
    void displayHelp(const std::string& command = "") const;

private:
    SlideFactory& slideFactory_;
    ISlideRepository& repo_;
    JsonSerializer& serializer_;
};
