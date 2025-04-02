/**
 * @file basic_document.hpp
 * @brief 定义基本的文档类实现
 * @author ChatGPT
 * @date 2024-03-26
 * 
 * 本文件实现了一个基本的文档类，用于测试和作为其他文档类的基础。
 * 主要功能：
 * - 支持从文本文件加载
 * - 维护文档标题
 * - 管理文档元素列表
 */

#pragma once

#include "document.hpp"
#include "document_elements.hpp"
#include <string>
#include <vector>
#include <memory>
#include <fstream>

namespace doc_converter {

/**
 * @brief 基本文档类
 * 
 * 实现了 Document 接口的基本文档类，提供了简单的文档管理功能。
 * 主要用于测试和作为其他文档类的基础实现。
 */
class BasicDocument : public Document {
public:
    /**
     * @brief 构造函数
     * @param title 文档标题
     */
    explicit BasicDocument(const std::string& title = "") : title_(title) {}

    /**
     * @brief 从文件加载文档
     * @param filePath 文档文件路径
     * @return bool 加载是否成功
     * 
     * 目前仅支持简单的文本文件，每行作为一个段落。
     * 第一行作为文档标题。
     */
    bool loadFromFile(const std::string& filePath) override {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            return false;
        }

        // 读取第一行作为标题
        std::string line;
        if (std::getline(file, line)) {
            title_ = line;
            auto heading = std::make_shared<HeadingElement>(line, 1);
            elements_.push_back(heading);
        }

        // 读取剩余行作为段落
        while (std::getline(file, line)) {
            if (!line.empty()) {
                auto paragraph = std::make_shared<ParagraphElement>();
                paragraph->addText(line);
                elements_.push_back(paragraph);
            }
        }

        return true;
    }

    /**
     * @brief 获取文档标题
     * @return string 文档标题
     */
    std::string getTitle() const override {
        return title_;
    }

    /**
     * @brief 获取文档元素列表
     * @return const vector<shared_ptr<DocumentElement>>& 文档元素列表
     */
    const std::vector<std::shared_ptr<DocumentElement>>& getElements() const override {
        return elements_;
    }

    /**
     * @brief 添加文档元素
     * @param element 要添加的元素
     */
    void addElement(std::shared_ptr<DocumentElement> element) override {
        elements_.push_back(element);
    }

private:
    std::string title_;                                      ///< 文档标题
    std::vector<std::shared_ptr<DocumentElement>> elements_; ///< 文档元素列表
};

} // namespace doc_converter 