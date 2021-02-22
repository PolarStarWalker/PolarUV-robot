#include "Logger.hpp"

LogLVL_t LogLVL = LogLVL_t::Warnings;

Logger &operator<<(Logger &logger, const std::string_view &str) {
    logger.stream_ << str;
    return logger;
}