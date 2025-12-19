#pragma once

#include <string>
#include <stdexcept>

class Error : public std::runtime_error {
public:
    enum class Code {
        SyntaxError,
        MissingArgument,
        InvalidNumber,
        UnmatchedQuote,
        UnknownCommand,
        InternalError
    };

    explicit Error(Code code, const std::string& msg)
        : std::runtime_error(msg), code_(code) {}

    Code code() const noexcept { return code_; }

private:
    Code code_;
};

inline Error syntax_error(const std::string& msg) {
    return Error(Error::Code::SyntaxError, msg);
}

inline Error missing_arg(const std::string& cmd) {
    return Error(Error::Code::MissingArgument, "Missing argument for '" + cmd + "'");
}

inline Error invalid_number(const std::string& token) {
    return Error(Error::Code::InvalidNumber, "Invalid number: '" + token + "'");
}

inline Error unknown_command(const std::string& cmd) {
    return Error(Error::Code::UnknownCommand, "Unknown command: '" + cmd + "'");
}