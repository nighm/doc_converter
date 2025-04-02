/**
 * @file logger.cpp
 * @brief 日志系统的实现
 * @author ChatGPT
 * @date 2024-03-26
 */

#include "doc_converter/logger.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace doc_converter {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

bool Logger::init(const std::string& logFile) {
    try {
        fileStream_.open(logFile, std::ios::app);
        if (!fileStream_.is_open()) {
            return false;
        }
        output_ = &fileStream_;
        return true;
    } catch (...) {
        return false;
    }
}

void Logger::setLevel(LogLevel level) {
    currentLevel_ = level;
}

void Logger::error(const std::string& message) {
    if (currentLevel_ >= LogLevel::ERROR) {
        log(LogLevel::ERROR, message);
    }
}

void Logger::warn(const std::string& message) {
    if (currentLevel_ >= LogLevel::WARN) {
        log(LogLevel::WARN, message);
    }
}

void Logger::info(const std::string& message) {
    if (currentLevel_ >= LogLevel::INFO) {
        log(LogLevel::INFO, message);
    }
}

void Logger::debug(const std::string& message) {
    if (currentLevel_ >= LogLevel::DEBUG) {
        log(LogLevel::DEBUG, message);
    }
}

void Logger::trace(const std::string& message) {
    if (currentLevel_ >= LogLevel::TRACE) {
        log(LogLevel::TRACE, message);
    }
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::TRACE: return "TRACE";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    // 获取当前时间
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    // 输出日志
    *output_ << "[" << oss.str() << "] [" << getLevelString(level) << "] " << message << std::endl;
}

} // namespace doc_converter 