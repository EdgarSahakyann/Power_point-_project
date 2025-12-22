#include "../include/CommandFactory.hpp"
#include "../include/core/SlideRepository.hpp"
#include "../include/CommandHistory.hpp"
#include "../include/serialization/SvgSerializer.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

void runSlideEditor(std::istream& input) {
    SlideFactory slideFactory;
    SlideRepository repo;
    JsonSerializer serializer;
    CommandFactory cmdFactory(slideFactory, repo, serializer);
    CommandHistory history;

    std::cout << "SlideEditor CLI - Commands: create <title> <content> <theme>, addtext <id> <text> --size <float> [--font <font>] [--color <color>] [--line-width <float>], addshape <id> <type> <scale>, move <from> <to>, removetext <id> <index>, removeshape <id> <index>, save <file>, load <file>, export-svg <output.svg>, display, help [command], exit" << std::endl;
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

        if (command == "undo") {
            history.undo();
            continue;
        }
        if (command == "redo") {
            history.redo();
            continue;
        }

        std::istringstream iss(command);
        std::string firstArg;
        iss >> firstArg;
        
        if (firstArg == "export-svg") {
            std::string outputFile;
            iss >> outputFile;
            if (outputFile.empty()) {
                std::cerr << "Usage: export-svg <output.svg>" << std::endl;
            } else {
                try {
                    SvgSerializer svgSerializer;
                    svgSerializer.save(repo, outputFile);
                } catch (const std::exception& e) {
                    std::cerr << "Error exporting SVG: " << e.what() << std::endl;
                }
            }
            continue;
        }

        auto cmd = cmdFactory.createCommand(command);
        if (cmd) {
            cmd->execute();
            history.pushExecuted(std::move(cmd));
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 4 && std::string(argv[1]) == "--export-svg") {
        std::string inputFile = argv[2];
        std::string outputFile = argv[3];
        
        try {
            SlideFactory slideFactory;
            SlideRepository repo;
            JsonSerializer jsonSerializer;
            SvgSerializer svgSerializer;
            
            jsonSerializer.load(repo, slideFactory, inputFile);
            
            svgSerializer.save(repo, outputFile);
            
            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
    
    runSlideEditor(std::cin);
    return 0;
}