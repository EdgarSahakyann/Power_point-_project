#include "../../include/commands/Commands.hpp"
#include "../../include/core/Shape.hpp"
#include <stdexcept>
#include <iostream>

CreateSlideCommand::CreateSlideCommand(SlideFactory& factory, ISlideRepository& repo, const std::string& title,
                                      const std::string& content, const std::string& theme)
    : factory_(factory), repo_(repo), title_(title), content_(content), theme_(theme) {}

void CreateSlideCommand::execute() {
    auto slide = factory_.createSlide(title_, content_, theme_);
    createdId_ = slide->getId();
    repo_.addSlide(std::move(slide));
    std::cout << "Created slide with ID: " << createdId_ << std::endl;
}

void CreateSlideCommand::undo() {
    if (createdId_ >= 0) {
        repo_.removeSlideById(createdId_);
        std::cout << "Undo: removed slide with ID: " << createdId_ << std::endl;
        createdId_ = -1;
    }

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
        addedIndex_ = slide->getTexts().size() - 1;
        std::cout << "Added text to slide " << slideId_ << " at index " << addedIndex_ << std::endl;
    } 
    else {
        std::cout << "Slide not found: " << slideId_ << std::endl;
    }
}

void AddTextCommand::undo() {
    auto slide = repo_.getSlideById(slideId_);
    if (slide && addedIndex_ != (std::size_t)-1) {
        try {
            slide->removeText(addedIndex_);
            std::cout << "Undo: removed text at index " << addedIndex_ << " from slide " << slideId_ << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "Undo failed: " << e.what() << std::endl;
        }
        addedIndex_ = (std::size_t)-1;
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
            addedIndex_ = slide->getShapes().size() - 1;
            std::cout << "Added shape " << shapeType_ << " to slide " << slideId_ << " at index " << addedIndex_ << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
        }
    } 
    else {
        std::cout << "Slide not found: " << slideId_ << std::endl;
    }
}

void AddShapeCommand::undo() {
    auto slide = repo_.getSlideById(slideId_);
    if (slide && addedIndex_ != (std::size_t)-1) {
        try {
            slide->removeShape(addedIndex_);
            std::cout << "Undo: removed shape at index " << addedIndex_ << " from slide " << slideId_ << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "Undo failed: " << e.what() << std::endl;
        }
        addedIndex_ = (std::size_t)-1;
    }
}

MoveSlideCommand::MoveSlideCommand(ISlideRepository& repo, std::size_t fromIndex, std::size_t toIndex)
    : repo_(repo), fromIndex_(fromIndex), toIndex_(toIndex) {}

void MoveSlideCommand::execute() {
    try {
        repo_.moveSlide(fromIndex_, toIndex_);
        executed_ = true;
        std::cout << "Moved slide from " << fromIndex_ << " to " << toIndex_ << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
}

void MoveSlideCommand::undo() {
    if (!executed_) return;
    try {
        repo_.moveSlide(toIndex_, fromIndex_);
        std::cout << "Undo: moved slide back from " << toIndex_ << " to " << fromIndex_ << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Undo failed: " << e.what() << std::endl;
    }
    executed_ = false;
}

RemoveTextCommand::RemoveTextCommand(ISlideRepository& repo, int slideId, std::size_t index)
    : repo_(repo), slideId_(slideId), index_(index) {}

void RemoveTextCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (slide) {
        try {
            removedText_ = slide->takeText(index_);
            removed_ = true;
            std::cout << "Removed text at index " << index_ << " from slide " << slideId_ << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << " for slide " << slideId_ << std::endl;
        }
    } 
    else {
        std::cout << "Slide not found: " << slideId_ << std::endl;
    }
}

void RemoveTextCommand::undo() {
    if (!removed_) return;
    auto slide = repo_.getSlideById(slideId_);
    if (slide) {
        try {
            slide->insertText(index_, removedText_);
            std::cout << "Undo: reinserted text at index " << index_ << " on slide " << slideId_ << std::endl;
            removed_ = false;
        } catch (const std::out_of_range& e) {
            std::cout << "Undo failed: " << e.what() << std::endl;
        }
    }
}

RemoveShapeCommand::RemoveShapeCommand(ISlideRepository& repo, int slideId, std::size_t index)
    : repo_(repo), slideId_(slideId), index_(index) {}

void RemoveShapeCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (slide) {
        try {
            removedShape_ = slide->takeShape(index_);
            removed_ = true;
            std::cout << "Removed shape at index " << index_ << " from slide " << slideId_ << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << " for slide " << slideId_ << std::endl;
        }
    } 
    else {
        std::cout << "Slide not found: " << slideId_ << std::endl;
    }
}

void RemoveShapeCommand::undo() {
    if (!removed_) return;
    auto slide = repo_.getSlideById(slideId_);
    if (slide && removedShape_) {
        try {
            slide->insertShape(index_, std::move(removedShape_));
            std::cout << "Undo: reinserted shape at index " << index_ << " on slide " << slideId_ << std::endl;
            removed_ = false;
        } catch (const std::out_of_range& e) {
            std::cout << "Undo failed: " << e.what() << std::endl;
        }
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