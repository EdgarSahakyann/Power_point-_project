#include "../../include/commands/Commands.hpp"
#include "../../include/core/Shape.hpp"
#include <stdexcept>
#include <iostream>

CreateSlideCommand::CreateSlideCommand(SlideFactory& factory, ISlideRepository& repo, const std::string& title,
                                      const std::string& content, const std::string& theme)
    : factory_(factory), repo_(repo), title_(title), content_(content), theme_(theme) {}

void CreateSlideCommand::execute() {
    auto slide = factory_.createSlide(title_, content_, theme_);
    int id = slide->getId();
    repo_.addSlide(std::move(slide));
    std::cout << "Created slide with ID: " << id << std::endl;
}

AddTextCommand::AddTextCommand(ISlideRepository& repo, int slideId, const std::string& textContent, float size,
                               const std::string& font, const std::string& color, float lineWidth)
    : repo_(repo), slideId_(slideId), textContent_(textContent), size_(size), font_(font), color_(color), lineWidth_(lineWidth) {}

void AddTextCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (slide) {
        Text text;
        text.content = textContent_;
        text.size = size_;
        text.font = font_;
        text.color = color_;
        text.lineWidth = lineWidth_;
        slide->addText(text);
        std::cout << "Added text to slide " << slideId_ << std::endl;
    } 
    else {
        std::cout << "Slide not found: " << slideId_ << std::endl;
    }
}

AddShapeCommand::AddShapeCommand(ISlideRepository& repo, unsigned slideId, const std::string& shapeType, float scale)
    : repo_(repo), slideId_(slideId), shapeType_(shapeType), scale_(scale) {}

void AddShapeCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (slide) {
        try {
            auto shape = ShapeFactory::createShape(shapeType_, scale_);
            slide->addShape(std::move(shape));
            std::cout << "Added shape " << shapeType_ << " to slide " << slideId_ << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
        }
    } 
    else {
        std::cout << "Slide not found: " << slideId_ << std::endl;
    }
}

MoveSlideCommand::MoveSlideCommand(ISlideRepository& repo, std::size_t fromIndex, std::size_t toIndex)
    : repo_(repo), fromIndex_(fromIndex), toIndex_(toIndex) {}

void MoveSlideCommand::execute() {
    try {
        repo_.moveSlide(fromIndex_, toIndex_);
        std::cout << "Moved slide from " << fromIndex_ << " to " << toIndex_ << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
}

RemoveTextCommand::RemoveTextCommand(ISlideRepository& repo, int slideId, std::size_t index)
    : repo_(repo), slideId_(slideId), index_(index) {}

void RemoveTextCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (slide) {
        try {
            slide->removeText(index_);
            std::cout << "Removed text at index " << index_ << " from slide " << slideId_ << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << " for slide " << slideId_ << std::endl;
        }
    } 
    else {
        std::cout << "Slide not found: " << slideId_ << std::endl;
    }
}

RemoveShapeCommand::RemoveShapeCommand(ISlideRepository& repo, int slideId, std::size_t index)
    : repo_(repo), slideId_(slideId), index_(index) {}

void RemoveShapeCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (slide) {
        try {
            slide->removeShape(index_);
            std::cout << "Removed shape at index " << index_ << " from slide " << slideId_ << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << " for slide " << slideId_ << std::endl;
        }
    } 
    else {
        std::cout << "Slide not found: " << slideId_ << std::endl;
    }
}

SaveCommand::SaveCommand(JsonSerializer& serializer, ISlideRepository& repo, const std::string& filepath)
    : serializer_(serializer), repo_(repo), filepath_(filepath) {}

void SaveCommand::execute() {
    serializer_.save(repo_, filepath_);
}

LoadCommand::LoadCommand(JsonSerializer& serializer, ISlideRepository& repo, SlideFactory& factory, const std::string& filepath)
    : serializer_(serializer), repo_(repo), factory_(factory), filepath_(filepath) {}

void LoadCommand::execute() {
    serializer_.load(repo_, factory_, filepath_);
}

DisplayCommand::DisplayCommand(ISlideRepository& repo) : repo_(repo) {}

void DisplayCommand::execute() {
    repo_.displayAll();
}