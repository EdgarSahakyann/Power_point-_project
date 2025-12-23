#include "../../include/core/Slide.hpp"

nlohmann::json Text::toJson() const {
    nlohmann::json j;
    j["content"] = content;
    j["size"] = size;
    j["font"] = font;
    j["color"] = color;
    j["lineWidth"] = lineWidth;
    
    return j;
}

std::string Text::toString() const {
    return content + " (size: " + std::to_string(size) + ", font: " + font + ", color: " + color + ", lineWidth: " + std::to_string(lineWidth) + ")";
}

Slide::Slide(int id, const std::string& title, const std::string& content, const std::string& theme)
    : id_(id), title_(title), content_(content), theme_(theme) {}

int Slide::getId() const { return id_; }
std::string Slide::getTitle() const { return title_; }
std::string Slide::getContent() const { return content_; }
std::string Slide::getTheme() const { return theme_; }

void Slide::setTitle(const std::string& title) { title_ = title; }

void Slide::addText(const Text& text) { 
    texts_.push_back(text);
}
void Slide::addShape(std::unique_ptr<IShape> shape) { shapes_.push_back(std::move(shape)); }

void Slide::removeText(std::size_t index) {
    if (index < texts_.size()) 
        texts_.erase(texts_.begin() + index);
    else 
        throw std::out_of_range("Text index out of range");
}

Text Slide::takeText(std::size_t index) {
    if (index < texts_.size()) {
        Text t = texts_[index];
        texts_.erase(texts_.begin() + index);
        return t;
    }
    throw std::out_of_range("Text index out of range");
}

void Slide::insertText(std::size_t index, const Text& text) {
    if (index <= texts_.size()) {
        texts_.insert(texts_.begin() + index, text);
        return;
    }
    throw std::out_of_range("Text insert index out of range");
}

void Slide::removeShape(std::size_t index) {
    if (index < shapes_.size()) 
        shapes_.erase(shapes_.begin() + index);
    else 
        throw std::out_of_range("Shape index out of range");
}

std::unique_ptr<IShape> Slide::takeShape(std::size_t index) {
    if (index < shapes_.size()) {
        auto ptr = std::move(shapes_[index]);
        shapes_.erase(shapes_.begin() + index);
        return ptr;
    }
    throw std::out_of_range("Shape index out of range");
}

void Slide::insertShape(std::size_t index, std::unique_ptr<IShape> shape) {
    if (index <= shapes_.size()) {
        shapes_.insert(shapes_.begin() + index, std::move(shape));
        return;
    }
    throw std::out_of_range("Shape insert index out of range");
}

const std::vector<Text>& Slide::getTexts() const { return texts_; }
const std::vector<std::unique_ptr<IShape>>& Slide::getShapes() const { return shapes_; }

nlohmann::json Slide::toJson() const {
    nlohmann::json j;
    j["id"] = id_;
    j["title"] = title_;
    j["content"] = content_;
    j["theme"] = theme_;

    nlohmann::json textsJson = nlohmann::json::array();
    for (const auto& text : texts_) 
        textsJson.push_back(text.toJson());

    j["texts"] = textsJson;
    nlohmann::json shapesJson = nlohmann::json::array();
    for (const auto& shape : shapes_) 
        shapesJson.push_back(shape->toJson());

    j["shapes"] = shapesJson;

    return j;
}

std::string Slide::toString() const {
    std::string str = "Slide ID: " + std::to_string(id_) + "\n";
    str += "Title: " + title_ + "\n";
    str += "Content: " + content_ + "\n";
    str += "Theme: " + theme_ + "\n";
    str += "Texts:\n";
    for (std::size_t i = 0; i < texts_.size(); ++i) {
        str += "  [" + std::to_string(i) + "] " + texts_[i].toString() + "\n";
    }
 
    str += "Shapes:\n";
    for (std::size_t i = 0; i < shapes_.size(); ++i) {
        str += "  [" + std::to_string(i) + "] " + shapes_[i]->toString() + "\n";
    }
 
    return str;
}