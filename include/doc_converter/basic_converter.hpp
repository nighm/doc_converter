/**
 * @file basic_converter.hpp
 * @brief 定义基本的转换器类实现
 * @author ChatGPT
 * @date 2024-03-26
 * 
 * 本文件实现了一个基本的转换器类，用于测试和作为其他转换器的基础。
 * 主要功能：
 * - 支持基本的文档转换
 * - 维护转换器名称和支持的格式
 * - 提供基本的转换逻辑
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
 * @brief 基本转换器类
 * 
 * 实现了 Converter 接口的基本转换器类，提供了简单的文档转换功能。
 * 主要用于测试和作为其他转换器的基础实现。
 */
class BasicConverter : public Converter {
public:
    /**
     * @brief 构造函数
     * @param name 转换器名称
     * @param supportedFormats 支持的格式列表
     */
    BasicConverter(const std::string& name, const std::vector<std::string>& supportedFormats)
        : name_(name), supportedFormats_(supportedFormats) {}

    /**
     * @brief 转换文档
     * @param doc 要转换的文档
     * @param outputPath 输出文件路径
     * @return bool 转换是否成功
     * 
     * 目前仅支持简单的文本输出，将文档内容写入文本文件。
     */
    bool convert(const Document& doc, const std::string& outputPath) override {
        try {
            std::ofstream file(outputPath);
            if (!file.is_open()) {
                return false;
            }

            // 写入标题
            file << doc.getTitle() << "\n\n";

            // 写入文档元素
            for (const auto& element : doc.getElements()) {
                switch (element->getType()) {
                    case ElementType::Heading: {
                        auto heading = std::dynamic_pointer_cast<HeadingElement>(element);
                        if (heading) {
                            file << std::string(heading->getLevel(), '#') << " "
                                 << heading->getText() << "\n\n";
                        }
                        break;
                    }
                    case ElementType::Paragraph: {
                        auto para = std::dynamic_pointer_cast<ParagraphElement>(element);
                        if (para) {
                            for (const auto& text : para->getTexts()) {
                                file << text->getText() << " ";
                            }
                            file << "\n\n";
                        }
                        break;
                    }
                    case ElementType::Text: {
                        auto text = std::dynamic_pointer_cast<TextElement>(element);
                        if (text) {
                            file << text->getText() << "\n\n";
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            return true;
        } catch (...) {
            return false;
        }
    }

    /**
     * @brief 获取转换器名称
     * @return string 转换器名称
     */
    std::string getName() const override {
        return name_;
    }

    /**
     * @brief 获取支持的格式列表
     * @return vector<string> 支持的格式列表
     */
    std::vector<std::string> getSupportedExtensions() const override {
        return supportedFormats_;
    }

private:
    std::string name_;                                      ///< 转换器名称
    std::vector<std::string> supportedFormats_;            ///< 支持的格式列表
};

} // namespace doc_converter 