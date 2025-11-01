#include "../include/CommandFactory.hpp"
#include "../include/core/SlideRepository.hpp"
#include <iostream>
#include <string>
#include <sstream>

void runSlideEditor(std::istream& input) {
    SlideFactory slideFactory;
    SlideRepository repo;
    JsonSerializer serializer;
    CommandFactory cmdFactory(slideFactory, repo, serializer);

    std::cout << "SlideEditor CLI - Commands: create <title> <content> <theme>, addtext <id> <text> --size <float> [--font <font>] [--color <color>] [--line-width <float>], addshape <id> <type> <scale>, move <from> <to>, removetext <id> <index>, removeshape <id> <index>, save <file>, load <file>, display, help [command], exit" << std::endl;
    std::cout << "Note: For simplicity, assume no spaces in arguments except between tokens. Use 'help' for details." << std::endl;

    std::string command;
    while (std::getline(input, command)) {
        if (command.empty()) {
            cmdFactory.displayHelp();
            continue;
        }
        if (command == "exit") {
            break;
        }

        std::istringstream iss(command);
        auto cmd = cmdFactory.createCommand(command);
        if (cmd) {
            cmd->execute();
        }
    }
}

int main() {
    runSlideEditor(std::cin);
    return 0;
}