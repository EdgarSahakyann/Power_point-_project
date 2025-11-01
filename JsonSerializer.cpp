#include "../../include/serialization/JsonSerializer.hpp"
#include <fstream>
#include <iostream>

void JsonSerializer::save(const ISlideRepository& repo, const std::string& filepath) const {
    try {
        nlohmann::json presentation = repo.toJson();
        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cout << "Error: Cannot open file " << filepath << std::endl;
            return;
        }

        file << presentation.dump(4);
        file.close();
        std::cout << "Saved presentation to " << filepath << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error saving to " << filepath << ": " << e.what() << std::endl;
    }
}

void JsonSerializer::load(ISlideRepository& repo, SlideFactory& factory, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filepath << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    int maxId = 1;
    if (j.contains("slides") && j["slides"].is_array()) {
        for (const auto& slideJson : j["slides"]) {
            repo.addSlide(factory.fromJson(slideJson, maxId));
        }
    
        factory.resetId(maxId);
        std::cout << "Loaded presentation from " << filepath << std::endl;
    } 
    else {
        std::cout << "Invalid JSON format in " << filepath << std::endl;
    }
}