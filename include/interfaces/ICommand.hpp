#pragma once

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    // Optional undo operation; default is no-op.
    virtual void undo() {}
    // Whether this command should be recorded in history.
    virtual bool isUndoable() const { return false; }
};