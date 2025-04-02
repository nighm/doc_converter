/**
 * @file logger.hpp
 * @brief 日志系统的实现
 * @author ChatGPT
 * @date 2024-03-26
 * 
 * 本文件实现了简单的日志系统，支持不同级别的日志记录。
 */

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

namespace doc_converter {

/**
 * @brief 日志级别枚举
 */
enum class LogLevel {
    ERROR,  ///< 错误信息
    WARN,   ///< 警告信息
    INFO,   ///< 一般信息
    DEBUG,  ///< 调试信息
    TRACE   ///< 跟踪信息
};

/**
 * @brief 日志系统类
 * 
 * 提供了简单的日志记录功能，支持不同级别的日志输出。
 */
class Logger {
public:
    /**
     * @brief 获取日志系统实例
     * @return Logger& 日志系统实例的引用
     */
    static Logger& getInstance();

    /**
     * @brief 初始化日志系统
     * @param logFile 日志文件路径
     * @return bool 初始化是否成功
     */
    bool init(const std::string& logFile);

    /**
     * @brief 设置日志级别
     * @param level 日志级别
     */
    void setLevel(LogLevel level);

    /**
     * @brief 记录错误日志
     * @param message 日志消息
     */
    void error(const std::string& message);

    /**
     * @brief 记录警告日志
     * @param message 日志消息
     */
    void warn(const std::string& message);

    /**
     * @brief 记录信息日志
     * @param message 日志消息
     */
    void info(const std::string& message);

    /**
     * @brief 记录调试日志
     * @param message 日志消息
     */
    void debug(const std::string& message);

    /**
     * @brief 记录跟踪日志
     * @param message 日志消息
     */
    void trace(const std::string& message);

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief 获取日志级别的字符串表示
     * @param level 日志级别
     * @return string 日志级别的字符串
     */
    std::string getLevelString(LogLevel level);

    /**
     * @brief 记录日志
     * @param level 日志级别
     * @param message 日志消息
     */
    void log(LogLevel level, const std::string& message);

    LogLevel currentLevel_ = LogLevel::INFO;  ///< 当前日志级别
    std::ostream* output_ = &std::cout;       ///< 日志输出流
    std::ofstream fileStream_;                ///< 文件输出流
};

} // namespace doc_converter 