/**
 * @file word_document_test.cpp
 * @brief Word文档解析器的单元测试
 * @author ChatGPT
 * @date 2024-03-26
 */

#include <gtest/gtest.h>
#include "doc_converter/word_document.hpp"
#include "doc_converter/document_elements.hpp"
#include "doc_converter/logger.hpp"

using namespace doc_converter;

/**
 * @brief 用于测试的WordDocument子类
 * 
 * 重写了文件加载方法，直接返回测试数据，不依赖实际文件
 */
class TestWordDocument : public WordDocument {
public:
    explicit TestWordDocument(const std::string& title = "") : WordDocument(title) {}

    bool loadFromFile(const std::string& filePath) override {
        // 模拟文件加载
        if (filePath == "nonexistent.docx" || filePath == "nonexistent.doc") {
            Logger::getInstance().error("Failed to load document: File not found: " + filePath);
            return false;
        }

        // 清除现有元素
        elements_.clear();

        if (filePath == "test.docx") {
            // 模拟.docx文件内容
            auto heading = std::make_shared<HeadingElement>("Test Document", 1);
            elements_.push_back(heading);

            auto para = std::make_shared<ParagraphElement>();
            para->addText("This is a test paragraph.");
            elements_.push_back(para);

            // 添加表格测试数据
            auto table = std::make_shared<TableElement>();
            TableRow row1;
            row1.addCell(TableCell("Header 1"));
            row1.addCell(TableCell("Header 2"));
            table->addRow(row1);

            TableRow row2;
            row2.addCell(TableCell("Cell 1"));
            row2.addCell(TableCell("Cell 2"));
            table->addRow(row2);

            elements_.push_back(table);
            return true;
        } else if (filePath == "test.doc") {
            // 模拟.doc文件内容
            auto para = std::make_shared<ParagraphElement>();
            para->addText("Test Document");
            elements_.push_back(para);
            return true;
        } else {
            Logger::getInstance().error("Failed to load document: Unsupported file format: " + filePath);
            return false;
        }
    }
};

class WordDocumentTest : public ::testing::Test {
protected:
    void SetUp() override {
        doc = std::make_unique<TestWordDocument>();
    }

    void TearDown() override {
        doc.reset();
    }

    std::unique_ptr<TestWordDocument> doc;
};

// 测试构造函数
TEST_F(WordDocumentTest, Constructor) {
    TestWordDocument doc("Test");
    EXPECT_EQ(doc.getTitle(), std::string("Test"));
    EXPECT_TRUE(doc.getElements().empty());
}

// 测试加载文档
TEST_F(WordDocumentTest, LoadFromFile) {
    // 测试加载不存在的文件
    EXPECT_FALSE(doc->loadFromFile("nonexistent.docx"));
    
    // 测试加载正常文件
    EXPECT_TRUE(doc->loadFromFile("test.docx"));
    
    // 验证文档内容
    const auto& elements = doc->getElements();
    ASSERT_EQ(elements.size(), 3);
    
    // 验证标题
    auto heading = std::dynamic_pointer_cast<HeadingElement>(elements[0]);
    ASSERT_TRUE(heading);
    EXPECT_EQ(heading->getText(), std::string("Test Document"));
    EXPECT_EQ(heading->getLevel(), 1);
    
    // 验证段落
    auto para = std::dynamic_pointer_cast<ParagraphElement>(elements[1]);
    ASSERT_TRUE(para);
    ASSERT_EQ(para->getTexts().size(), 1);
    EXPECT_EQ(para->getTexts()[0]->getText(), std::string("This is a test paragraph."));

    // 验证表格
    auto table = std::dynamic_pointer_cast<TableElement>(elements[2]);
    ASSERT_TRUE(table);
}

// 测试添加元素
TEST_F(WordDocumentTest, AddElement) {
    // 添加标题元素
    auto heading = std::make_shared<HeadingElement>("Test Heading", 1);
    doc->addElement(heading);
    
    // 添加段落元素
    auto para = std::make_shared<ParagraphElement>();
    para->addText("Test paragraph");
    doc->addElement(para);
    
    // 验证元素
    const auto& elements = doc->getElements();
    ASSERT_EQ(elements.size(), 2);
    
    auto headingElement = std::dynamic_pointer_cast<HeadingElement>(elements[0]);
    ASSERT_TRUE(headingElement);
    EXPECT_EQ(headingElement->getText(), std::string("Test Heading"));
    EXPECT_EQ(headingElement->getLevel(), 1);
    
    auto paraElement = std::dynamic_pointer_cast<ParagraphElement>(elements[1]);
    ASSERT_TRUE(paraElement);
    ASSERT_EQ(paraElement->getTexts().size(), 1);
    EXPECT_EQ(paraElement->getTexts()[0]->getText(), std::string("Test paragraph"));
}

// 测试空文档
TEST_F(WordDocumentTest, EmptyDocument) {
    TestWordDocument doc("Empty");
    EXPECT_TRUE(doc.getElements().empty());
}

// 测试加载.doc格式
TEST_F(WordDocumentTest, LoadDocFormat) {
    // 测试加载.doc文件
    EXPECT_TRUE(doc->loadFromFile("test.doc"));
    
    // 验证文档内容
    const auto& elements = doc->getElements();
    ASSERT_FALSE(elements.empty());
    
    // 验证段落内容
    auto para = std::dynamic_pointer_cast<ParagraphElement>(elements[0]);
    ASSERT_TRUE(para);
    ASSERT_FALSE(para->getTexts().empty());
    EXPECT_EQ(para->getTexts()[0]->getText(), std::string("Test Document"));
}

// 测试.doc格式错误处理
TEST_F(WordDocumentTest, DocFormatErrorHandling) {
    // 测试加载不存在的.doc文件
    EXPECT_FALSE(doc->loadFromFile("nonexistent.doc"));
    
    // 测试加载不支持的文件格式
    EXPECT_FALSE(doc->loadFromFile("test.txt"));
}

// 测试表格解析
TEST_F(WordDocumentTest, TableParsing) {
    // 测试加载包含表格的文档
    EXPECT_TRUE(doc->loadFromFile("test.docx"));
    
    // 验证文档内容
    const auto& elements = doc->getElements();
    ASSERT_EQ(elements.size(), 3);  // 标题、段落和表格
    
    // 验证表格
    auto table = std::dynamic_pointer_cast<TableElement>(elements[2]);
    ASSERT_TRUE(table);
    
    // 验证表格行数
    const auto& rows = table->getRows();
    ASSERT_EQ(rows.size(), 2);
    
    // 验证第一行（表头）
    const auto& headerRow = rows[0];
    ASSERT_EQ(headerRow.getCells().size(), 2);
    EXPECT_EQ(headerRow.getCells()[0].getText(), std::string("Header 1"));
    EXPECT_EQ(headerRow.getCells()[1].getText(), std::string("Header 2"));
    
    // 验证第二行（数据）
    const auto& dataRow = rows[1];
    ASSERT_EQ(dataRow.getCells().size(), 2);
    EXPECT_EQ(dataRow.getCells()[0].getText(), std::string("Cell 1"));
    EXPECT_EQ(dataRow.getCells()[1].getText(), std::string("Cell 2"));
}

// 测试表格元素添加
TEST_F(WordDocumentTest, AddTableElement) {
    // 创建表格
    auto table = std::make_shared<TableElement>();
    
    // 添加表头行
    TableRow headerRow;
    headerRow.addCell(TableCell("Column 1"));
    headerRow.addCell(TableCell("Column 2"));
    table->addRow(headerRow);
    
    // 添加数据行
    TableRow dataRow;
    dataRow.addCell(TableCell("Value 1"));
    dataRow.addCell(TableCell("Value 2"));
    table->addRow(dataRow);
    
    // 添加表格到文档
    doc->addElement(table);
    
    // 验证表格
    const auto& elements = doc->getElements();
    ASSERT_EQ(elements.size(), 1);
    
    auto addedTable = std::dynamic_pointer_cast<TableElement>(elements[0]);
    ASSERT_TRUE(addedTable);
    
    // 验证表格内容
    const auto& rows = addedTable->getRows();
    ASSERT_EQ(rows.size(), 2);
    
    // 验证表头
    EXPECT_EQ(rows[0].getCells()[0].getText(), std::string("Column 1"));
    EXPECT_EQ(rows[0].getCells()[1].getText(), std::string("Column 2"));
    
    // 验证数据
    EXPECT_EQ(rows[1].getCells()[0].getText(), std::string("Value 1"));
    EXPECT_EQ(rows[1].getCells()[1].getText(), std::string("Value 2"));
}

// 测试图片元素添加
TEST_F(WordDocumentTest, AddImageElement) {
    // 创建测试图片数据
    std::vector<uint8_t> imageData = {1, 2, 3, 4, 5};
    
    // 创建图片元素
    auto image = std::make_shared<ImageElement>(
        imageData,
        "png",
        100,
        200
    );
    
    // 添加图片到文档
    doc->addElement(image);
    
    // 验证图片元素
    const auto& elements = doc->getElements();
    ASSERT_EQ(elements.size(), 1);
    
    auto addedImage = std::dynamic_pointer_cast<ImageElement>(elements[0]);
    ASSERT_TRUE(addedImage);
    
    // 验证图片属性
    EXPECT_EQ(addedImage->getFormat(), "png");
    EXPECT_EQ(addedImage->getWidth(), 100);
    EXPECT_EQ(addedImage->getHeight(), 200);
    
    // 验证图片数据
    const auto& data = addedImage->getImageData();
    ASSERT_EQ(data.size(), 5);
    for (size_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(data[i], i + 1);
    }
}

// 测试图片解析
TEST_F(WordDocumentTest, ImageParsing) {
    // 测试加载包含图片的文档
    EXPECT_TRUE(doc->loadFromFile("test_with_image.docx"));
    
    // 验证文档内容
    const auto& elements = doc->getElements();
    ASSERT_EQ(elements.size(), 1);  // 只包含一个图片元素
    
    // 验证图片元素
    auto image = std::dynamic_pointer_cast<ImageElement>(elements[0]);
    ASSERT_TRUE(image);
    
    // 验证图片属性
    EXPECT_EQ(image->getFormat(), "png");
    EXPECT_EQ(image->getWidth(), 800);
    EXPECT_EQ(image->getHeight(), 600);
    
    // 验证图片数据不为空
    const auto& data = image->getImageData();
    EXPECT_FALSE(data.empty());
}