#pragma once

#include "../interfaces/ICommand.hpp"
#include "../core/Slide.hpp"
#include "../core/SlideRepository.hpp"
#include "../core/SlideFactory.hpp"
#include <string>
#include <memory>
#include <functional>

class ActionCommand : public ICommand {
public:
    virtual ~ActionCommand() = default;

protected:
    ActionCommand() = default;
};

class ModifyTextCommand : public ActionCommand {
public:
    ModifyTextCommand(ISlideRepository& repo, int slideId, std::size_t textIndex,
                     const std::string& newContent, float newSize = -1.0f,
                     const std::string& newFont = "", const std::string& newColor = "");
    void execute() override;
    void undo() override;
    bool isUndoable() const override { return true; }

private:
    ISlideRepository& repo_;
    int slideId_;
    std::size_t textIndex_;
    std::string newContent_;
    float newSize_;
    std::string newFont_;
    std::string newColor_;
    Text oldState_;
    bool executed_ = false;
};

class ModifyShapeCommand : public ActionCommand {
public:
    using ModifyFunc = std::function<void(IShape&)>;
    using RevertFunc = std::function<void(IShape&)>;

    ModifyShapeCommand(ISlideRepository& repo, int slideId, std::size_t shapeIndex,
                      ModifyFunc modify, RevertFunc revert);
    void execute() override;
    void undo() override;
    bool isUndoable() const override { return true; }

private:
    ISlideRepository& repo_;
    int slideId_;
    std::size_t shapeIndex_;
    ModifyFunc modify_;
    RevertFunc revert_;
    bool executed_ = false;
};

class ClearSlideCommand : public ActionCommand {
public:
    explicit ClearSlideCommand(ISlideRepository& repo, int slideId);
    void execute() override;
    void undo() override;
    bool isUndoable() const override { return true; }

private:
    ISlideRepository& repo_;
    int slideId_;
    std::vector<Text> savedTexts_;
    std::vector<std::unique_ptr<IShape>> savedShapes_;
    bool executed_ = false;
};

class RenameSlideCommand : public ActionCommand {
public:
    RenameSlideCommand(ISlideRepository& repo, int slideId, const std::string& newTitle);
    void execute() override;
    void undo() override;
    bool isUndoable() const override { return true; }

private:
    ISlideRepository& repo_;
    int slideId_;
    std::string newTitle_;
    std::string oldTitle_;
    bool executed_ = false;
};

class DuplicateSlideCommand : public ActionCommand {
public:
    DuplicateSlideCommand(ISlideRepository& repo, SlideFactory& factory, int sourceSlideId);
    void execute() override;
    void undo() override;
    bool isUndoable() const override { return true; }

private:
    ISlideRepository& repo_;
    int sourceSlideId_;
    int createdSlideId_ = -1;
    SlideFactory& factory_;
};

class DeleteSlideCommand : public ActionCommand {
public:
    DeleteSlideCommand(ISlideRepository& repo, SlideFactory& factory, int slideId);
    void execute() override;
    void undo() override;
    bool isUndoable() const override { return true; }

private:
    ISlideRepository& repo_;
    int slideId_;
    nlohmann::json savedSlideJson_;
    SlideFactory& factory_;
    bool executed_ = false;
};

class ReorderSlideCommand : public ActionCommand {
public:
    ReorderSlideCommand(ISlideRepository& repo, int slideId, bool moveUp);
    void execute() override;
    void undo() override;
    bool isUndoable() const override { return true; }

private:
    ISlideRepository& repo_;
    int slideId_;
    bool moveUp_;
    std::size_t oldIndex_ = 0;
    std::size_t newIndex_ = 0;
    bool executed_ = false;
};

class BatchActionCommand : public ActionCommand {
public:
    void addAction(std::unique_ptr<ActionCommand> action) {
        if (action) {
            actions_.push_back(std::move(action));
        }
    }

    void execute() override {
        for (auto& action : actions_) {
            if (action) {
                action->execute();
            }
        }
        executed_ = true;
    }

    void undo() override {
        for (auto it = actions_.rbegin(); it != actions_.rend(); ++it) {
            if (*it) {
                (*it)->undo();
            }
        }
        executed_ = false;
    }

    bool isUndoable() const override {
        if (actions_.empty()) return false;
        for (const auto& action : actions_) {
            if (!action || !action->isUndoable()) {
                return false;
            }
        }
        return true;
    }

    std::size_t getActionCount() const { return actions_.size(); }

private:
    std::vector<std::unique_ptr<ActionCommand>> actions_;
    bool executed_ = false;
};
