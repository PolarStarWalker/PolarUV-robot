#ifndef POLAR_UV_LOGGER_HPP
#define POLAR_UV_LOGGER_HPP

#include <sstream>
#include <iostream>

enum class LogLVL_t : size_t {
    Info = 0,
    Debug = 1,
    Warnings = 2,
    Errors = 3
};

extern LogLVL_t LogLVL;

class Logger {
public:

private:
    std::stringstream stream_;
    const LogLVL_t lvl_;
public:

    explicit Logger(LogLVL_t lvl) : lvl_(lvl) {}

    friend Logger &operator<<(Logger &logger, const std::string_view &str);

    ~Logger() {
        if (lvl_ >= LogLVL) {
            std::cout << stream_.rdbuf() << std::endl;
        }
    }
};

Logger &operator<<(Logger &logger, const std::string_view &str);


#endif
