#include "../../include/commands/ActionCommand.hpp"
#include "../../include/core/Shape.hpp"
#include "../../include/core/SlideFactory.hpp"
#include <iostream>
#include <algorithm>

ModifyTextCommand::ModifyTextCommand(ISlideRepository& repo, int slideId, std::size_t textIndex,
                                     const std::string& newContent, float newSize,
                                     const std::string& newFont, const std::string& newColor)
    : repo_(repo), slideId_(slideId), textIndex_(textIndex), newContent_(newContent),
      newSize_(newSize), newFont_(newFont), newColor_(newColor) {}

void ModifyTextCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    const auto& texts = slide->getTexts();
    if (textIndex_ >= texts.size()) return;

    oldState_ = texts[textIndex_];
    Text modified = oldState_;

    if (!newContent_.empty()) modified.content = newContent_;
    if (newSize_ > 0) modified.size = newSize_;
    if (!newFont_.empty()) modified.font = newFont_;
    if (!newColor_.empty()) modified.color = newColor_;

    slide->removeText(textIndex_);
    slide->insertText(textIndex_, modified);
    executed_ = true;
    std::cout << "Modified text at index " << textIndex_ << " on slide " << slideId_ << std::endl;
}

void ModifyTextCommand::undo() {
    if (!executed_) return;
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    slide->removeText(textIndex_);
    slide->insertText(textIndex_, oldState_);
    executed_ = false;
    std::cout << "Undo: restored text at index " << textIndex_ << " on slide " << slideId_ << std::endl;
}

ModifyShapeCommand::ModifyShapeCommand(ISlideRepository& repo, int slideId, std::size_t shapeIndex,
                                       ModifyFunc modify, RevertFunc revert)
    : repo_(repo), slideId_(slideId), shapeIndex_(shapeIndex),
      modify_(modify), revert_(revert) {}

void ModifyShapeCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    const auto& shapes = slide->getShapes();
    if (shapeIndex_ >= shapes.size()) return;

    if (modify_) {
        modify_(*shapes[shapeIndex_]);
    }
    executed_ = true;
    std::cout << "Modified shape at index " << shapeIndex_ << " on slide " << slideId_ << std::endl;
}

void ModifyShapeCommand::undo() {
    if (!executed_) return;
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    const auto& shapes = slide->getShapes();
    if (shapeIndex_ >= shapes.size()) return;

    if (revert_) {
        revert_(*shapes[shapeIndex_]);
    }
    executed_ = false;
    std::cout << "Undo: reverted shape at index " << shapeIndex_ << " on slide " << slideId_ << std::endl;
}

ClearSlideCommand::ClearSlideCommand(ISlideRepository& repo, int slideId)
    : repo_(repo), slideId_(slideId) {}

void ClearSlideCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    savedTexts_ = slide->getTexts();
    savedShapes_.clear();
    for (const auto& shape : slide->getShapes()) {
        savedShapes_.push_back(ShapeFactory::fromJson(shape->toJson()));
    }

    while (!slide->getTexts().empty()) {
        slide->removeText(0);
    }
    while (!slide->getShapes().empty()) {
        slide->removeShape(0);
    }

    executed_ = true;
    std::cout << "Cleared slide " << slideId_ << std::endl;
}

void ClearSlideCommand::undo() {
    if (!executed_) return;
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    for (const auto& text : savedTexts_) {
        slide->addText(text);
    }
    for (auto& shape : savedShapes_) {
        slide->addShape(std::move(shape));
    }

    executed_ = false;
    std::cout << "Undo: restored slide " << slideId_ << std::endl;
}

RenameSlideCommand::RenameSlideCommand(ISlideRepository& repo, int slideId, const std::string& newTitle)
    : repo_(repo), slideId_(slideId), newTitle_(newTitle) {}

void RenameSlideCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    oldTitle_ = slide->getTitle();
    slide->setTitle(newTitle_);
    executed_ = true;
    std::cout << "Renamed slide " << slideId_ << " from '" << oldTitle_ << "' to '" << newTitle_ << "'" << std::endl;
}

void RenameSlideCommand::undo() {
    if (!executed_) return;
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    slide->setTitle(oldTitle_);
    executed_ = false;
    std::cout << "Undo: renamed slide " << slideId_ << " back to '" << oldTitle_ << "'" << std::endl;
}

DuplicateSlideCommand::DuplicateSlideCommand(ISlideRepository& repo, SlideFactory& factory, int sourceSlideId)
    : repo_(repo), sourceSlideId_(sourceSlideId), factory_(factory) {}

void DuplicateSlideCommand::execute() {
    auto source = repo_.getSlideById(sourceSlideId_);
    if (!source) return;

    auto duplicate = factory_.createSlide(source->getTitle() + " (copy)", source->getContent(), source->getTheme());

    for (const auto& text : source->getTexts()) {
        duplicate->addText(text);
    }
    for (const auto& shape : source->getShapes()) {
        duplicate->addShape(ShapeFactory::fromJson(shape->toJson()));
    }

    createdSlideId_ = duplicate->getId();
    repo_.addSlide(std::move(duplicate));
    std::cout << "Duplicated slide " << sourceSlideId_ << " as " << createdSlideId_ << std::endl;
}

void DuplicateSlideCommand::undo() {
    if (createdSlideId_ >= 0) {
        repo_.removeSlideById(createdSlideId_);
        createdSlideId_ = -1;
        std::cout << "Undo: removed duplicated slide" << std::endl;
    }
}

DeleteSlideCommand::DeleteSlideCommand(ISlideRepository& repo, SlideFactory& factory, int slideId)
    : repo_(repo), slideId_(slideId), factory_(factory) {}

void DeleteSlideCommand::execute() {
    auto slide = repo_.getSlideById(slideId_);
    if (!slide) return;

    savedSlideJson_ = slide->toJson();

    repo_.removeSlideById(slideId_);
    executed_ = true;
    std::cout << "Deleted slide " << slideId_ << std::endl;
}

void DeleteSlideCommand::undo() {
    if (!executed_ || savedSlideJson_.is_null()) return;

    int dummyMax = 0;
    auto restored = factory_.fromJson(savedSlideJson_, dummyMax);
    repo_.addSlide(std::move(restored));
    executed_ = false;
    std::cout << "Undo: restored deleted slide " << slideId_ << std::endl;
}

ReorderSlideCommand::ReorderSlideCommand(ISlideRepository& repo, int slideId, bool moveUp)
    : repo_(repo), slideId_(slideId), moveUp_(moveUp) {}

void ReorderSlideCommand::execute() {
    const auto& slides = repo_.getAllSlides();
    oldIndex_ = 0;

    for (std::size_t i = 0; i < slides.size(); ++i) {
        if (slides[i]->getId() == slideId_) {
            oldIndex_ = i;
            break;
        }
    }

    if (moveUp_) {
        if (oldIndex_ > 0) {
            newIndex_ = oldIndex_ - 1;
        } else {
            return;
        }
    } else {
        if (oldIndex_ < slides.size() - 1) {
            newIndex_ = oldIndex_ + 1;
        } else {
            return;
        }
    }

    repo_.moveSlide(oldIndex_, newIndex_);
    executed_ = true;
    std::cout << "Moved slide " << slideId_ << " from index " << oldIndex_ << " to " << newIndex_ << std::endl;
}

void ReorderSlideCommand::undo() {
    if (!executed_) return;
    repo_.moveSlide(newIndex_, oldIndex_);
    executed_ = false;
    std::cout << "Undo: moved slide " << slideId_ << " back to index " << oldIndex_ << std::endl;
}
