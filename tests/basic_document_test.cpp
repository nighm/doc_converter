/**
 * @file basic_document_test.cpp
 * @brief 基本文档类的单元测试
 * @author ChatGPT
 * @date 2024-03-26
 */

#include <gtest/gtest.h>
#include <fstream>
#include "doc_converter/basic_document.hpp"

using namespace doc_converter;

class BasicDocumentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建测试文件
        std::ofstream test_file("test_doc.txt");
        test_file << "Document Title\n";
        test_file << "First paragraph.\n";
        test_file << "\n";  // 空行
        test_file << "Second paragraph.\n";
        test_file.close();
    }

    void TearDown() override {
        // 删除测试文件
        std::remove("test_doc.txt");
    }
};

// 测试构造函数
TEST_F(BasicDocumentTest, Constructor) {
    BasicDocument doc("Test Title");
    EXPECT_EQ(doc.getTitle(), "Test Title");
    EXPECT_TRUE(doc.getElements().empty());
}

// 测试从文件加载
TEST_F(BasicDocumentTest, LoadFromFile) {
    BasicDocument doc;
    
    // 测试加载不存在的文件
    EXPECT_FALSE(doc.loadFromFile("nonexistent.txt"));
    
    // 测试加载存在的文件
    EXPECT_TRUE(doc.loadFromFile("test_doc.txt"));
    EXPECT_EQ(doc.getTitle(), "Document Title");
    
    // 检查文档元素
    const auto& elements = doc.getElements();
    ASSERT_EQ(elements.size(), 3);  // 标题 + 两个段落
    
    // 检查标题
    ASSERT_EQ(elements[0]->getType(), ElementType::Heading);
    auto heading = std::dynamic_pointer_cast<HeadingElement>(elements[0]);
    ASSERT_NE(heading, nullptr);
    EXPECT_EQ(heading->getText(), "Document Title");
    EXPECT_EQ(heading->getLevel(), 1);
    
    // 检查第一个段落
    ASSERT_EQ(elements[1]->getType(), ElementType::Paragraph);
    auto para1 = std::dynamic_pointer_cast<ParagraphElement>(elements[1]);
    ASSERT_NE(para1, nullptr);
    ASSERT_EQ(para1->getTexts().size(), 1);
    EXPECT_EQ(para1->getTexts()[0]->getText(), "First paragraph.");
    
    // 检查第二个段落
    ASSERT_EQ(elements[2]->getType(), ElementType::Paragraph);
    auto para2 = std::dynamic_pointer_cast<ParagraphElement>(elements[2]);
    ASSERT_NE(para2, nullptr);
    ASSERT_EQ(para2->getTexts().size(), 1);
    EXPECT_EQ(para2->getTexts()[0]->getText(), "Second paragraph.");
}

// 测试添加元素
TEST_F(BasicDocumentTest, AddElement) {
    BasicDocument doc("Test");
    
    // 添加标题
    auto heading = std::make_shared<HeadingElement>("Section 1", 2);
    doc.addElement(heading);
    
    // 添加段落
    auto para = std::make_shared<ParagraphElement>();
    para->addText("Test paragraph");
    doc.addElement(para);
    
    // 检查元素
    const auto& elements = doc.getElements();
    ASSERT_EQ(elements.size(), 2);
    EXPECT_EQ(elements[0]->getType(), ElementType::Heading);
    EXPECT_EQ(elements[1]->getType(), ElementType::Paragraph);
} 