/**
 * @file basic_converter_test.cpp
 * @brief 基本转换器的单元测试
 * @author ChatGPT
 * @date 2024-03-26
 */

#include <gtest/gtest.h>
#include <fstream>
#include "doc_converter/basic_converter.hpp"
#include "doc_converter/basic_document.hpp"

using namespace doc_converter;

class BasicConverterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 创建测试文档
        doc_ = std::make_shared<BasicDocument>("Test Document");
        
        // 添加标题
        auto heading = std::make_shared<HeadingElement>("Section 1", 2);
        doc_->addElement(heading);
        
        // 添加段落
        auto para = std::make_shared<ParagraphElement>();
        para->addText("This is a test paragraph.");
        doc_->addElement(para);
        
        // 创建转换器
        converter_ = std::make_shared<BasicConverter>("Test Converter", 
                                                    std::vector<std::string>{"txt"});
    }

    void TearDown() override {
        // 删除测试文件
        std::remove("test_output.txt");
    }

    std::shared_ptr<BasicDocument> doc_;
    std::shared_ptr<BasicConverter> converter_;
};

// 测试转换器名称
TEST_F(BasicConverterTest, GetName) {
    EXPECT_EQ(converter_->getName(), "Test Converter");
}

// 测试支持的格式
TEST_F(BasicConverterTest, GetSupportedExtensions) {
    const auto& formats = converter_->getSupportedExtensions();
    ASSERT_EQ(formats.size(), 1);
    EXPECT_EQ(formats[0], "txt");
}

// 测试文档转换
TEST_F(BasicConverterTest, Convert) {
    // 测试转换到不存在的目录
    EXPECT_FALSE(converter_->convert(*doc_, "/nonexistent/path/test.txt"));
    
    // 测试正常转换
    EXPECT_TRUE(converter_->convert(*doc_, "test_output.txt"));
    
    // 验证输出文件内容
    std::ifstream file("test_output.txt");
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    // 检查标题
    std::getline(file, line);
    EXPECT_EQ(line, "Test Document");
    std::getline(file, line); // 空行
    
    // 检查标题元素
    std::getline(file, line);
    EXPECT_EQ(line, "## Section 1");
    std::getline(file, line); // 空行
    
    // 检查段落
    std::getline(file, line);
    EXPECT_EQ(line, "This is a test paragraph. ");
    std::getline(file, line); // 空行
    
    file.close();
}

// 测试空文档转换
TEST_F(BasicConverterTest, ConvertEmptyDocument) {
    auto emptyDoc = std::make_shared<BasicDocument>("Empty Document");
    EXPECT_TRUE(converter_->convert(*emptyDoc, "test_output.txt"));
    
    std::ifstream file("test_output.txt");
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "Empty Document");
    
    file.close();
} 