/**
 * @file document_elements_test.cpp
 * @brief 文档元素类的单元测试
 * @author ChatGPT
 * @date 2024-03-26
 */

#include <gtest/gtest.h>
#include "doc_converter/document_elements.hpp"

using namespace doc_converter;

// 测试文本元素
TEST(DocumentElementsTest, TextElement) {
    TextElement text("Hello, World!");
    
    // 测试元素类型
    EXPECT_EQ(text.getType(), ElementType::Text);
    
    // 测试文本内容
    EXPECT_EQ(text.getText(), "Hello, World!");
}

// 测试段落元素
TEST(DocumentElementsTest, ParagraphElement) {
    ParagraphElement para;
    
    // 测试元素类型
    EXPECT_EQ(para.getType(), ElementType::Paragraph);
    
    // 测试添加文本
    para.addText("First sentence.");
    para.addText("Second sentence.");
    
    // 测试获取文本
    auto texts = para.getTexts();
    ASSERT_EQ(texts.size(), 2);
    EXPECT_EQ(texts[0]->getText(), "First sentence.");
    EXPECT_EQ(texts[1]->getText(), "Second sentence.");
}

// 测试标题元素
TEST(DocumentElementsTest, HeadingElement) {
    // 测试正常标题级别
    HeadingElement h1("Title", 1);
    EXPECT_EQ(h1.getType(), ElementType::Heading);
    EXPECT_EQ(h1.getText(), "Title");
    EXPECT_EQ(h1.getLevel(), 1);
    
    // 测试标题级别限制（小于1）
    HeadingElement h_low("Low", 0);
    EXPECT_EQ(h_low.getLevel(), 1);
    
    // 测试标题级别限制（大于6）
    HeadingElement h_high("High", 7);
    EXPECT_EQ(h_high.getLevel(), 6);
}

// 表格元素测试
TEST(DocumentElementsTest, TableElement) {
    // 创建表格元素
    TableElement table;
    EXPECT_EQ(table.getType(), ElementType::Table);
    EXPECT_TRUE(table.getRows().empty());

    // 创建表格行和单元格
    TableRow row1;
    row1.addCell(TableCell("Cell 1,1"));
    row1.addCell(TableCell("Cell 1,2"));
    row1.addCell(TableCell("Cell 1,3"));

    TableRow row2;
    row2.addCell(TableCell("Cell 2,1"));
    row2.addCell(TableCell("Cell 2,2"));
    row2.addCell(TableCell("Cell 2,3"));

    // 添加行到表格
    table.addRow(row1);
    table.addRow(row2);

    // 验证表格内容
    EXPECT_EQ(table.getRows().size(), 2);
    EXPECT_EQ(table.getRows()[0].getCells().size(), 3);
    EXPECT_EQ(table.getRows()[1].getCells().size(), 3);

    // 验证单元格内容
    EXPECT_EQ(table.getRows()[0].getCells()[0].getText(), "Cell 1,1");
    EXPECT_EQ(table.getRows()[0].getCells()[1].getText(), "Cell 1,2");
    EXPECT_EQ(table.getRows()[0].getCells()[2].getText(), "Cell 1,3");
    EXPECT_EQ(table.getRows()[1].getCells()[0].getText(), "Cell 2,1");
    EXPECT_EQ(table.getRows()[1].getCells()[1].getText(), "Cell 2,2");
    EXPECT_EQ(table.getRows()[1].getCells()[2].getText(), "Cell 2,3");
} 