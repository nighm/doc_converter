/**
 * @file document_elements.hpp
 * @brief 定义文档中各种元素的具体类
 * @author ChatGPT
 * @date 2024-03-26
 * 
 * 本文件包含以下组件：
 * - TextElement：文本元素
 * - ParagraphElement：段落元素
 * - TableElement：表格元素
 * - ImageElement：图片元素
 * - ListElement：列表元素
 * - HeadingElement：标题元素
 */

#pragma once

#include "doc_converter/common.hpp"
#include <string>
#include <vector>
#include <memory>

namespace doc_converter {

/**
 * @brief 文本元素类
 * 
 * 表示文档中的纯文本内容。
 */
class TextElement : public DocumentElement {
public:
    /**
     * @brief 构造函数
     * @param text 文本内容
     */
    explicit TextElement(const std::string& text) : text_(text) {}

    /**
     * @brief 获取元素类型
     * @return ElementType 返回 ElementType::Text
     */
    ElementType getType() const override { return ElementType::Text; }

    /**
     * @brief 获取文本内容
     * @return const string& 文本内容
     */
    const std::string& getText() const { return text_; }

private:
    std::string text_;  ///< 存储文本内容
};

/**
 * @brief 段落元素类
 * 
 * 表示文档中的段落，可以包含多个文本元素。
 */
class ParagraphElement : public DocumentElement {
public:
    /**
     * @brief 获取元素类型
     * @return ElementType 返回 ElementType::Paragraph
     */
    ElementType getType() const override { return ElementType::Paragraph; }

    /**
     * @brief 添加文本元素
     * @param text 要添加的文本
     */
    void addText(const std::string& text) {
        texts_.push_back(std::make_shared<TextElement>(text));
    }

    /**
     * @brief 获取段落中的所有文本元素
     * @return const vector<shared_ptr<TextElement>>& 文本元素列表
     */
    const std::vector<std::shared_ptr<TextElement>>& getTexts() const { return texts_; }

private:
    std::vector<std::shared_ptr<TextElement>> texts_;  ///< 存储段落中的文本元素
};

/**
 * @brief 标题元素类
 * 
 * 表示文档中的标题，包含级别信息。
 */
class HeadingElement : public DocumentElement {
public:
    /**
     * @brief 构造函数
     * @param text 标题文本
     * @param level 标题级别（1-6）
     */
    HeadingElement(const std::string& text, int level)
        : text_(text), level_(std::min(std::max(level, 1), 6)) {}

    /**
     * @brief 获取元素类型
     * @return ElementType 返回 ElementType::Heading
     */
    ElementType getType() const override { return ElementType::Heading; }

    /**
     * @brief 获取标题文本
     * @return const string& 标题文本
     */
    const std::string& getText() const { return text_; }

    /**
     * @brief 获取标题级别
     * @return int 标题级别（1-6）
     */
    int getLevel() const { return level_; }

private:
    std::string text_;  ///< 标题文本
    int level_;         ///< 标题级别（1-6）
};

/**
 * @brief 表格单元格类
 */
class TableCell {
public:
    /**
     * @brief 构造函数
     * @param text 单元格文本
     */
    explicit TableCell(const std::string& text = "") : text_(text) {}

    /**
     * @brief 获取单元格文本
     * @return string 单元格文本
     */
    std::string getText() const { return text_; }

    /**
     * @brief 设置单元格文本
     * @param text 单元格文本
     */
    void setText(const std::string& text) { text_ = text; }

private:
    std::string text_;  ///< 单元格文本
};

/**
 * @brief 表格行类
 */
class TableRow {
public:
    /**
     * @brief 添加单元格
     * @param cell 单元格
     */
    void addCell(const TableCell& cell) { cells_.push_back(cell); }

    /**
     * @brief 获取单元格列表
     * @return vector<TableCell>& 单元格列表的引用
     */
    const std::vector<TableCell>& getCells() const { return cells_; }

private:
    std::vector<TableCell> cells_;  ///< 单元格列表
};

/**
 * @brief 表格元素类
 */
class TableElement : public DocumentElement {
public:
    /**
     * @brief 构造函数
     */
    TableElement() = default;

    /**
     * @brief 获取元素类型
     * @return ElementType 元素类型
     */
    ElementType getType() const override { return ElementType::Table; }

    /**
     * @brief 添加表格行
     * @param row 表格行
     */
    void addRow(const TableRow& row) { rows_.push_back(row); }

    /**
     * @brief 获取表格行列表
     * @return vector<TableRow>& 表格行列表的引用
     */
    const std::vector<TableRow>& getRows() const { return rows_; }

private:
    std::vector<TableRow> rows_;  ///< 表格行列表
};

/**
 * @brief 图片元素类
 * 
 * 表示文档中的图片，包含图片数据和属性信息。
 */
class ImageElement : public DocumentElement {
public:
    /**
     * @brief 构造函数
     * @param imageData 图片数据
     * @param format 图片格式（如 "png", "jpg" 等）
     * @param width 图片宽度（像素）
     * @param height 图片高度（像素）
     */
    ImageElement(const std::vector<uint8_t>& imageData, 
                const std::string& format,
                int width,
                int height)
        : imageData_(imageData)
        , format_(format)
        , width_(width)
        , height_(height) {}

    /**
     * @brief 获取元素类型
     * @return ElementType 返回 ElementType::Image
     */
    ElementType getType() const override { return ElementType::Image; }

    /**
     * @brief 获取图片数据
     * @return const vector<uint8_t>& 图片数据
     */
    const std::vector<uint8_t>& getImageData() const { return imageData_; }

    /**
     * @brief 获取图片格式
     * @return const string& 图片格式
     */
    const std::string& getFormat() const { return format_; }

    /**
     * @brief 获取图片宽度
     * @return int 图片宽度（像素）
     */
    int getWidth() const { return width_; }

    /**
     * @brief 获取图片高度
     * @return int 图片高度（像素）
     */
    int getHeight() const { return height_; }

private:
    std::vector<uint8_t> imageData_;  ///< 图片数据
    std::string format_;              ///< 图片格式
    int width_;                       ///< 图片宽度
    int height_;                      ///< 图片高度
};

// TODO: 实现 ListElement

} // namespace doc_converter 