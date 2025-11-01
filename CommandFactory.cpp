#include "../include/CommandFactory.hpp"

CommandFactory::CommandFactory(SlideFactory& slideFactory, ISlideRepository& repo, JsonSerializer& serializer)
    : slideFactory_(slideFactory), repo_(repo), serializer_(serializer), 
      lexer_(), parser_(slideFactory, repo, serializer) {}

std::unique_ptr<ICommand> CommandFactory::createCommand(const std::string& commandStr) const {
    std::istringstream iss(commandStr);
    return parser_.parse(iss);
}

void CommandFactory::displayHelp(const std::string& command) const {
    parser_.displayHelp(command);
}