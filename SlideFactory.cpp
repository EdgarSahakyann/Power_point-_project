#include "../../include/core/SlideFactory.hpp"
#include "../../include/core/Shape.hpp"

std::unique_ptr<Slide> SlideFactory::createSlide(const std::string& title, const std::string& content, const std::string& theme) {
    return std::make_unique<Slide>(nextId_++, title, content, theme);
}

std::unique_ptr<Slide> SlideFactory::fromJson(const nlohmann::json& j, int& maxId) {
    int id = j["id"];
    maxId = std::max(maxId, id + 1);
    auto slide = std::make_unique<Slide>(id, j["title"], j["content"], j["theme"]);

    if (j.contains("texts") && j["texts"].is_array()) {
        for (const auto& textJson : j["texts"]) {
            Text text;
            text.content = textJson["content"];
            text.size = textJson["size"];
            text.font = textJson.value("font", "Arial");
            text.color = textJson.value("color", "Black");
            text.lineWidth = textJson.value("lineWidth", 1.0f);
            slide->addText(text);
        }
    }

    if (j.contains("shapes") && j["shapes"].is_array()) {
        for (const auto& shapeJson : j["shapes"]) {
            slide->addShape(ShapeFactory::fromJson(shapeJson));
        }
    }

    return slide;
}

void SlideFactory::resetId(int startId) { nextId_ = startId; }