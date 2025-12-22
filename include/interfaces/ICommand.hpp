#pragma once

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() {}
    virtual bool isUndoable() const { return false; }
};