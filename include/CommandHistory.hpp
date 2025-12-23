#pragma once

#include "interfaces/ICommand.hpp"
#include <memory>
#include <stack>

class CommandHistory {
public:
    void pushExecuted(std::unique_ptr<ICommand> cmd) {
        if (!cmd) return;
        if (cmd->isUndoable()) {
            while (!redo_.empty()) redo_.pop();
            undo_.push(std::move(cmd));
        }
    }

    bool canUndo() const { return !undo_.empty(); }

    bool canRedo() const { return !redo_.empty(); }

    std::size_t getUndoCount() const { return undo_.size(); }

    std::size_t getRedoCount() const { return redo_.size(); }

    void undo() {
        if (undo_.empty()) return;
        auto cmd = std::move(undo_.top());
        undo_.pop();
        cmd->undo();
        redo_.push(std::move(cmd));
    }

    void redo() {
        if (redo_.empty()) return;
        auto cmd = std::move(redo_.top());
        redo_.pop();
        cmd->execute();
        undo_.push(std::move(cmd));
    }

    void clear() {
        while (!undo_.empty()) undo_.pop();
        while (!redo_.empty()) redo_.pop();
    }

private:
    std::stack<std::unique_ptr<ICommand>> undo_;
    std::stack<std::unique_ptr<ICommand>> redo_;
};
