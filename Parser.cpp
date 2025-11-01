#include "../../include/parser/Parser.hpp"
#include "../../include/commands/Commands.hpp"
#include "../../include/parser/Tokenizer.hpp"
#include <iostream>
#include <map>

Parser::Parser(SlideFactory& slideFactory, ISlideRepository& repo, JsonSerializer& serializer)
    : slideFactory_(slideFactory), repo_(repo), serializer_(serializer) {}

std::unique_ptr<ICommand> Parser::parse(std::istream& input) const {
    Tokenizer tokenizer;
    auto words = tokenizer.tokenize(input);
    Lexer lexer;
    auto tokens = lexer.tokenize(words);
    if (tokens.empty()) {
        std::cout << "Invalid command." << std::endl;
        return nullptr;
    }

    State state = State::START;
    std::string command, title, content, theme, textContent, font = "Arial", color = "Black";
    int slideId = 0;
    float size = 0.0f, scale = 0.0f, lineWidth = 1.0f;
    std::size_t index = 0, fromIndex = 0, toIndex = 0;
    std::string shapeType, filepath, helpCommand;

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];
        switch (state) {
            case State::START:
                if (token.type == Token::Type::Word) {
                    command = token.value;
                    if (command == "create") 
                        state = State::CREATE_TITLE;
                    else if (command == "addtext") 
                        state = State::ADDTEXT_ID;
                    else if (command == "addshape") 
                        state = State::ADDSHAPE_ID;
                    else if (command == "move") 
                        state = State::MOVE_FROM;
                    else if (command == "removetext") 
                        state = State::REMOVETEXT_ID;
                    else if (command == "removeshape") 
                        state = State::REMOVESHAPE_ID;
                    else if (command == "save") 
                        state = State::SAVE_FILE;
                    else if (command == "load") 
                        state = State::LOAD_FILE;
                    else if (command == "display") 
                        state = State::DISPLAY;
                    else if (command == "help") 
                        state = State::HELP;
                    else 
                        state = State::ERROR;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::CREATE_TITLE:
                if (token.type == Token::Type::Word) {
                    title = token.value;
                    state = State::CREATE_CONTENT;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::CREATE_CONTENT:
                if (token.type == Token::Type::Word) {
                    content = token.value;
                    state = State::CREATE_THEME;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::CREATE_THEME:
                if (token.type == Token::Type::Word) {
                    theme = token.value;
                    return std::make_unique<CreateSlideCommand>(slideFactory_, repo_, title, content, theme);
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDTEXT_ID:
                if (token.type == Token::Type::Number) {
                    slideId = std::stoi(token.value);
                    state = State::ADDTEXT_TEXT;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDTEXT_TEXT:
                if (token.type == Token::Type::Word) {
                    textContent = token.value;
                    state = State::ADDTEXT_SIZE_FLAG;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDTEXT_SIZE_FLAG:
                if (token.type == Token::Type::Flag && token.value == "--size") {
                    state = State::ADDTEXT_SIZE_VALUE;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDTEXT_SIZE_VALUE:
                if (token.type == Token::Type::Float) {
                    size = std::stof(token.value);
                    state = State::ADDTEXT_FONT_FLAG;
                } 
                else {
                    state = State::ERROR;
                }

                break;

            case State::ADDTEXT_FONT_FLAG:
                if (token.type == Token::Type::Flag) {
                    if (token.value == "--font") {
                        state = State::ADDTEXT_FONT_VALUE;
                    } 
                    else if (token.value == "--color") {
                        state = State::ADDTEXT_COLOR_VALUE;
                    } 
                    else if (token.value == "--line-width") {
                        state = State::ADDTEXT_LINEWIDTH_VALUE;
                    } 
                    else {
                        state = State::ERROR;
                    }
                } 
                else {
                    return std::make_unique<AddTextCommand>(repo_, slideId, textContent, size, font, color, lineWidth);
                }
                
                break;

            case State::ADDTEXT_FONT_VALUE:
                if (token.type == Token::Type::Word) {
                    font = token.value;
                    state = State::ADDTEXT_COLOR_FLAG;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDTEXT_COLOR_FLAG:
                if (token.type == Token::Type::Flag) {
                    if (token.value == "--color") {
                        state = State::ADDTEXT_COLOR_VALUE;
                    } 
                    else if (token.value == "--line-width") {
                        state = State::ADDTEXT_LINEWIDTH_VALUE;
                    } 
                    else {
                        state = State::ERROR;
                    }
                } 
                else {
                    return std::make_unique<AddTextCommand>(repo_, slideId, textContent, size, font, color, lineWidth);
                }
                
                break;

            case State::ADDTEXT_COLOR_VALUE:
                if (token.type == Token::Type::Word) {
                    color = token.value;
                    state = State::ADDTEXT_LINEWIDTH_FLAG;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDTEXT_LINEWIDTH_FLAG:
                if (token.type == Token::Type::Flag && token.value == "--line-width") {
                    state = State::ADDTEXT_LINEWIDTH_VALUE;
                } 
                else {
                    return std::make_unique<AddTextCommand>(repo_, slideId, textContent, size, font, color, lineWidth);
                }
                
                break;

            case State::ADDTEXT_LINEWIDTH_VALUE:
                if (token.type == Token::Type::Float) {
                    lineWidth = std::stof(token.value);
                    return std::make_unique<AddTextCommand>(repo_, slideId, textContent, size, font, color, lineWidth);
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDSHAPE_ID:
                if (token.type == Token::Type::Number) {
                    slideId = std::stoi(token.value);
                    state = State::ADDSHAPE_TYPE;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDSHAPE_TYPE:
                if (token.type == Token::Type::Word) {
                    shapeType = token.value;
                    state = State::ADDSHAPE_SCALE;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::ADDSHAPE_SCALE:
                if (token.type == Token::Type::Float) {
                    scale = std::stof(token.value);
                    return std::make_unique<AddShapeCommand>(repo_, slideId, shapeType, scale);
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::MOVE_FROM:
                if (token.type == Token::Type::Number) {
                    fromIndex = std::stoul(token.value);
                    state = State::MOVE_TO;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::MOVE_TO:
                if (token.type == Token::Type::Number) {
                    toIndex = std::stoul(token.value);
                    return std::make_unique<MoveSlideCommand>(repo_, fromIndex, toIndex);
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::REMOVETEXT_ID:
                if (token.type == Token::Type::Number) {
                    slideId = std::stoi(token.value);
                    state = State::REMOVETEXT_INDEX;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::REMOVETEXT_INDEX:
                if (token.type == Token::Type::Number) {
                    index = std::stoul(token.value);
                    return std::make_unique<RemoveTextCommand>(repo_, slideId, index);
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::REMOVESHAPE_ID:
                if (token.type == Token::Type::Number) {
                    slideId = std::stoi(token.value);
                    state = State::REMOVESHAPE_INDEX;
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::REMOVESHAPE_INDEX:
                if (token.type == Token::Type::Number) {
                    index = std::stoul(token.value);
                    return std::make_unique<RemoveShapeCommand>(repo_, slideId, index);
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::SAVE_FILE:
                if (token.type == Token::Type::Word) {
                    filepath = token.value;
                    return std::make_unique<SaveCommand>(serializer_, repo_, filepath);
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::LOAD_FILE:
                if (token.type == Token::Type::Word) {
                    filepath = token.value;
                    return std::make_unique<LoadCommand>(serializer_, repo_, slideFactory_, filepath);
                } 
                else {
                    state = State::ERROR;
                }
                
                break;

            case State::DISPLAY:
                return std::make_unique<DisplayCommand>(repo_);
                break;

            case State::HELP:
                if (i + 1 < tokens.size() && tokens[i + 1].type == Token::Type::Word) {
                    helpCommand = tokens[i + 1].value;
                    displayHelp(helpCommand);
                } 
                else {
                    displayHelp();
                }
                
                return nullptr;

            case State::ERROR:
                std::cout << "Invalid command or arguments. Use 'help' for assistance." << std::endl;
                return nullptr;
        }
    }

    if (state == State::DISPLAY) {
        return std::make_unique<DisplayCommand>(repo_);
    } 
    else if (state == State::HELP) {
        displayHelp();
        return nullptr;
    } 
    else if (state == State::ADDTEXT_FONT_FLAG || state == State::ADDTEXT_COLOR_FLAG || state == State::ADDTEXT_LINEWIDTH_FLAG) {
        return std::make_unique<AddTextCommand>(repo_, slideId, textContent, size, font, color, lineWidth);
    }

    std::cout << "Incomplete command. Use 'help' for assistance." << std::endl;
    return nullptr;
}

void Parser::displayHelp(const std::string& command) const {
    if (command.empty()) {
        std::cout << "Available commands:\n"
                  << "  create <title> <content> <theme> - Create a new slide\n"
                  << "  addtext <id> <text> --size <float> [--font <font>] [--color <color>] [--line-width <float>] - Add text to a slide\n"
                  << "  addshape <id> <type> <scale> - Add a shape to a slide\n"
                  << "  move <from> <to> - Move slide from index to another\n"
                  << "  removetext <id> <index> - Remove text from a slide\n"
                  << "  removeshape <id> <index> - Remove a shape from a slide\n"
                  << "  save <file> - Save presentation to file\n"
                  << "  load <file> - Load presentation from file\n"
                  << "  display - Display all slides\n"
                  << "  help [command] - Show this help or command details\n"
                  << "  exit - Exit the program\n"
                  << "Note: For simplicity, assume no spaces in arguments. Use 'help <command>' for details.\n";
    } 
    else if (command == "create") {
        std::cout << "create <title> <content> <theme>\n"
                  << "  Creates a new slide with the specified title, content, and theme.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "addtext") {
        std::cout << "addtext <id> <text> --size <float> [--font <font>] [--color <color>] [--line-width <float>]\n"
                  << "  Adds text to the slide with ID <id>.\n"
                  << "  Flags:\n"
                  << "    --size <float>   : Required. Sets text size (e.g., 1.5).\n"
                  << "    --font <font>    : Optional. Sets font (e.g., Arial). Default: Arial.\n"
                  << "    --color <color>  : Optional. Sets color (e.g., Red). Default: Black.\n"
                  << "    --line-width <float> : Optional. Sets line width (e.g., 2.0). Default: 1.0.\n";
    } 
    else if (command == "addshape") {
        std::cout << "addshape <id> <type> <scale>\n"
                  << "  Adds a shape (Triangle, Circle, Rectangle, Ellipse) to the slide with ID <id>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "move") {
        std::cout << "move <from> <to>\n"
                  << "  Moves the slide at index <from> to index <to>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "removetext") {
        std::cout << "removetext <id> <index>\n"
                  << "  Removes text at <index> from the slide with ID <id>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "removeshape") {
        std::cout << "removeshape <id> <index>\n"
                  << "  Removes shape at <index> from the slide with ID <id>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "save") {
        std::cout << "save <file>\n"
                  << "  Saves the presentation to <file> in JSON format.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "load") {
        std::cout << "load <file>\n"
                  << "  Loads the presentation from <file>.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "display") {
        std::cout << "display\n"
                  << "  Displays all slides.\n"
                  << "  No flags available.\n";
    } 
    else if (command == "help") {
        std::cout << "help [command]\n"
                  << "  Shows all commands or details for a specific command.\n"
                  << "  No flags available.\n";
    } 
    else {
        std::cout << "Unknown command: " << command << ". Use 'help' to list all commands.\n";
    }
}