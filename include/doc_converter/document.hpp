/**
 * @file document.hpp
 * @brief 定义文档转换工具的核心接口
 * 
 * 本文件包含以下主要组件：
 * - Document：文档的基类接口
 * - Converter：文档转换器的基类接口
 * - ConverterFactory：转换器工厂类
 */

#pragma once

#include "doc_converter/common.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace doc_converter {

/**
 * @brief 文档类
 * 
 * 表示一个完整的文档，包含多个文档元素。
 */
class Document {
public:
    /**
     * @brief 默认构造函数
     */
    Document() = default;

    /**
     * @brief 虚析构函数
     */
    virtual ~Document() = default;

    /**
     * @brief 添加文档元素
     * @param element 要添加的文档元素
     */
    virtual void addElement(std::shared_ptr<DocumentElement> element) = 0;

    /**
     * @brief 获取所有文档元素
     * @return const vector<shared_ptr<DocumentElement>>& 文档元素列表
     */
    virtual const std::vector<std::shared_ptr<DocumentElement>>& getElements() const = 0;

    /**
     * @brief 从文件加载文档
     * @param filePath 文档文件路径
     * @return bool 加载是否成功
     */
    virtual bool loadFromFile(const std::string& filePath) = 0;
    
    /**
     * @brief 获取文档标题
     * @return string 文档标题
     */
    virtual std::string getTitle() const = 0;
};

/**
 * @brief 转换器基类
 * 
 * 定义了文档转换器的基本操作接口，包括：
 * - 转换文档
 * - 获取转换器名称
 * - 获取支持的文件格式
 */
class Converter {
public:
    virtual ~Converter() = default;
    
    /**
     * @brief 转换文档
     * @param doc 要转换的文档
     * @param outputPath 输出文件路径
     * @return bool 转换是否成功
     */
    virtual bool convert(const Document& doc, const std::string& outputPath) = 0;
    
    /**
     * @brief 获取转换器名称
     * @return string 转换器的名称
     */
    virtual std::string getName() const = 0;
    
    /**
     * @brief 获取支持的文件扩展名列表
     * @return vector<string> 支持的文件扩展名列表
     */
    virtual std::vector<std::string> getSupportedExtensions() const = 0;
};

/**
 * @brief 转换器工厂类
 * 
 * 负责管理和创建各种文档转换器，使用工厂模式实现。
 * 提供了注册和创建转换器的静态方法。
 */
class ConverterFactory {
public:
    /**
     * @brief 注册转换器创建函数
     * @param name 转换器名称
     * @param creator 创建转换器的函数对象
     */
    static void registerConverter(
        const std::string& name,
        std::function<std::shared_ptr<Converter>()> creator);
    
    /**
     * @brief 创建转换器实例
     * @param name 转换器名称
     * @return shared_ptr<Converter> 创建的转换器实例
     */
    static std::shared_ptr<Converter> createConverter(const std::string& name);
};

} // namespace doc_converter 