/**
 * @file converter_factory.cpp
 * @brief 转换器工厂类的实现
 * 
 * 实现了ConverterFactory类的功能，包括：
 * - 注册转换器创建函数
 * - 创建转换器实例
 * 使用静态map存储转换器创建函数。
 */

#include "doc_converter/document.hpp"
#include <unordered_map>

namespace doc_converter {

namespace {
    /**
     * @brief 存储转换器创建函数的全局映射表
     * 
     * 键：转换器名称
     * 值：创建转换器的函数对象
     */
    std::unordered_map<std::string, std::function<std::shared_ptr<Converter>()>> g_converterCreators;
}

void ConverterFactory::registerConverter(
    const std::string& name,
    std::function<std::shared_ptr<Converter>()> creator) {
    // 将创建函数存储到映射表中
    g_converterCreators[name] = std::move(creator);
}

std::shared_ptr<Converter> ConverterFactory::createConverter(const std::string& name) {
    // 查找转换器创建函数
    auto it = g_converterCreators.find(name);
    if (it == g_converterCreators.end()) {
        // 如果找不到对应的转换器，返回nullptr
        return nullptr;
    }
    // 调用创建函数生成转换器实例
    return it->second();
}
} 