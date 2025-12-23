#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../interfaces/IShape.hpp"
#include <nlohmann/json.hpp>

struct Text {
    std::string content;
    float size = 1.0f;
    std::string font = "Arial"; 
    std::string color = "Black";
    float lineWidth = 1.0f;

    nlohmann::json toJson() const;
    std::string toString() const;
};

class Slide {    
public:
    Slide(int id, const std::string& title, const std::string& content, const std::string& theme);
    int getId() const;
    std::string getTitle() const;
    std::string getContent() const;
    std::string getTheme() const;
    void setTitle(const std::string& title);
    void addText(const Text& text);
    void addShape(std::unique_ptr<IShape> shape);
    void removeText(std::size_t index);
    void removeShape(std::size_t index);
    Text takeText(std::size_t index);
    void insertText(std::size_t index, const Text& text);
    std::unique_ptr<IShape> takeShape(std::size_t index);
    void insertShape(std::size_t index, std::unique_ptr<IShape> shape);
    const std::vector<Text>& getTexts() const;
    const std::vector<std::unique_ptr<IShape>>& getShapes() const;
    nlohmann::json toJson() const;
    std::string toString() const;

private:
    int id_;
    std::string title_;
    std::string content_;
    std::string theme_;
    std::vector<Text> texts_;
    std::vector<std::unique_ptr<IShape>> shapes_;
};