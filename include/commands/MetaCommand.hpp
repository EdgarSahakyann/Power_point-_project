#pragma once

#include "../interfaces/ICommand.hpp"
#include <vector>
#include <memory>
#include <functional>

class MetaCommand : public ICommand {
public:
    virtual ~MetaCommand() = default;

    void addCommand(std::unique_ptr<ICommand> cmd) {
        if (cmd) {
            commands_.push_back(std::move(cmd));
        }
    }

    void execute() override {
        for (auto& cmd : commands_) {
            if (cmd) {
                cmd->execute();
            }
        }
    }

    void undo() override {
        for (auto it = commands_.rbegin(); it != commands_.rend(); ++it) {
            if (*it) {
                (*it)->undo();
            }
        }
    }

    bool isUndoable() const override {
        if (commands_.empty()) return false;
        for (const auto& cmd : commands_) {
            if (!cmd || !cmd->isUndoable()) {
                return false;
            }
        }
        return true;
    }

    std::size_t getCommandCount() const {
        return commands_.size();
    }

    void clear() {
        commands_.clear();
    }

protected:
    std::vector<std::unique_ptr<ICommand>> commands_;
};

class MacroCommand : public MetaCommand {
public:
    explicit MacroCommand(const std::string& name = "Macro") : name_(name) {}

    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }

private:
    std::string name_;
};

class ConditionalMetaCommand : public MetaCommand {
public:
    using ConditionFunc = std::function<bool()>;

    explicit ConditionalMetaCommand(ConditionFunc condition)
        : condition_(condition) {}

    void execute() override {
        if (condition_()) {
            MetaCommand::execute();
        }
    }

    bool isUndoable() const override {
        if (!condition_()) return false;
        return MetaCommand::isUndoable();
    }

private:
    ConditionFunc condition_;
};
