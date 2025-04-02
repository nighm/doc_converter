/**
 * @file main.cpp
 * @brief 程序入口
 * @author ChatGPT
 * @date 2024-03-26
 */

#include <QtWidgets/QApplication>
#include "doc_converter/main_window.hpp"
#include "doc_converter/logger.hpp"

using namespace doc_converter;

int main(int argc, char* argv[]) {
    try {
        // 初始化日志系统
        Logger::getInstance().init("doc_converter.log");

        // 创建Qt应用程序
        QApplication app(argc, argv);
        
        // 创建主窗口
        doc_converter::MainWindow mainWindow;
        mainWindow.show();

        // 运行应用程序
        return app.exec();
    } catch (const std::exception& e) {
        Logger::getInstance().error("程序异常退出: " + std::string(e.what()));
        return 1;
    }
} 