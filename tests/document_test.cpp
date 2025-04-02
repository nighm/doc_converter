/**
 * @file document_test.cpp
 * @brief 文档转换接口的单元测试
 * 
 * 包含以下测试内容：
 * - 文档元素的类型获取
 * - 文档的加载和标题获取
 * - 文档元素的添加和访问
 * - 转换器的基本操作
 */

#include <gtest/gtest.h>
#include "doc_converter/document.hpp"

namespace doc_converter {
namespace testing {

/**
 * @brief 用于测试的文档元素实现
 * 
 * 实现了DocumentElement接口，用于测试文档元素的基本功能。
 */
class TestElement : public DocumentElement {
public:
    explicit TestElement(ElementType type) : type_(type) {}
    ElementType getType() const override { return type_; }
private:
    ElementType type_;
};

/**
 * @brief 用于测试的文档实现
 * 
 * 实现了Document接口，提供了：
 * - 基本的文件加载功能
 * - 固定的文档标题
 * - 文档元素的存储和访问
 */
class TestDocument : public Document {
public:
    TestDocument() = default;
    virtual ~TestDocument() = default;

    bool loadFromFile(const std::string& filePath) override {
        // 支持多种文档格式
        if (filePath == "test.doc" || filePath == "test.docx" || filePath == "test.docm") {
            m_title = "Test Document";
            return true;
        }
        return false;
    }

    std::string getTitle() const override {
        return m_title;
    }

    const std::vector<std::shared_ptr<DocumentElement>>& getElements() const override {
        return m_elements;
    }

    void addElement(std::shared_ptr<DocumentElement> element) override {
        m_elements.push_back(element);
    }

private:
    std::string m_title;
    std::vector<std::shared_ptr<DocumentElement>> m_elements;
};

/**
 * @brief 用于测试的转换器实现
 * 
 * 实现了Converter接口，提供了：
 * - 基本的转换功能（仅记录参数）
 * - 固定的转换器名称
 * - 固定的支持格式列表
 */
class TestConverter : public Converter {
public:
    bool convert(const Document& doc, const std::string& outputPath) override {
        lastDocument_ = &doc;
        lastOutputPath_ = outputPath;
        return true;  // 测试用，总是返回成功
    }
    
    std::string getName() const override {
        return "Test Converter";
    }
    
    std::vector<std::string> getSupportedExtensions() const override {
        return {".test"};
    }

    /**
     * @brief 获取最后一次转换的文档
     */
    const Document* getLastDocument() const { return lastDocument_; }

    /**
     * @brief 获取最后一次转换的输出路径
     */
    const std::string& getLastOutputPath() const { return lastOutputPath_; }

private:
    const Document* lastDocument_ = nullptr;
    std::string lastOutputPath_;
};

// 文档元素测试用例
TEST(DocumentElementTest, GetType) {
    TestElement textElement(ElementType::Text);
    EXPECT_EQ(textElement.getType(), ElementType::Text);

    TestElement tableElement(ElementType::Table);
    EXPECT_EQ(tableElement.getType(), ElementType::Table);
}

// 文档基本功能测试用例
TEST(DocumentTest, LoadAndGetTitle) {
    TestDocument doc;
    EXPECT_TRUE(doc.loadFromFile("test.doc"));
    EXPECT_EQ(doc.getTitle(), "Test Document");
}

// 测试不同文档格式的支持
TEST(DocumentTest, SupportedFormats) {
    TestDocument doc;
    EXPECT_TRUE(doc.loadFromFile("test.doc"));
    EXPECT_TRUE(doc.loadFromFile("test.docx"));
    EXPECT_TRUE(doc.loadFromFile("test.docm"));
    EXPECT_FALSE(doc.loadFromFile("test.txt"));  // 不支持的格式
    EXPECT_FALSE(doc.loadFromFile("invalid.doc"));  // 无效的文件名
}

// 文档元素管理测试用例
TEST(DocumentTest, Elements) {
    TestDocument doc;
    EXPECT_TRUE(doc.getElements().empty());

    // 添加文本元素
    auto textElement = std::make_shared<TestElement>(ElementType::Text);
    doc.addElement(textElement);
    EXPECT_EQ(doc.getElements().size(), 1);
    EXPECT_EQ(doc.getElements()[0]->getType(), ElementType::Text);

    // 添加表格元素
    auto tableElement = std::make_shared<TestElement>(ElementType::Table);
    doc.addElement(tableElement);
    EXPECT_EQ(doc.getElements().size(), 2);
    EXPECT_EQ(doc.getElements()[1]->getType(), ElementType::Table);
}

// 转换器基本功能测试用例
TEST(ConverterTest, BasicOperations) {
    TestConverter converter;
    TestDocument doc;
    
    EXPECT_EQ(converter.getName(), "Test Converter");
    EXPECT_EQ(converter.getSupportedExtensions(), std::vector<std::string>{".test"});
    
    EXPECT_TRUE(converter.convert(doc, "output.test"));
    EXPECT_EQ(converter.getLastDocument(), &doc);
    EXPECT_EQ(converter.getLastOutputPath(), "output.test");
}

} // namespace testing
} // namespace doc_converter 