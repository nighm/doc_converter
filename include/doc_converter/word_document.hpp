/**
 * @file word_document.hpp
 * @brief Word文档解析器的实现
 * @author ChatGPT
 * @date 2024-03-26
 * 
 * 本文件实现了Word文档的解析功能：
 * - 使用libxml2和zlib库解析.docx文件
 * - 使用antiword解析.doc文件
 * 主要功能：
 * - 读取.docx和.doc文件
 * - 解析文档结构（段落、标题等）
 * - 提取文档内容
 */

#pragma once

#include "doc_converter/common.hpp"
#include "doc_converter/document.hpp"
#include "doc_converter/document_elements.hpp"
#include <string>
#include <vector>
#include <memory>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <zlib.h>

namespace doc_converter {

/**
 * @brief Word文档类
 * 
 * 实现了Document接口的Word文档类，提供了.docx和.doc文件的读取和解析功能。
 * - .docx文件使用libxml2和zlib库解析
 * - .doc文件使用antiword工具解析
 */
class WordDocument : public Document {
public:
    /**
     * @brief 构造函数
     * @param title 文档标题
     */
    explicit WordDocument(const std::string& title = "");

    /**
     * @brief 析构函数
     */
    ~WordDocument();

    /**
     * @brief 从文件加载文档
     * @param filePath 文件路径
     * @return bool 是否加载成功
     */
    bool loadFromFile(const std::string& filePath) override;

    /**
     * @brief 获取文档标题
     * @return string 文档标题
     */
    std::string getTitle() const override;

    /**
     * @brief 获取文档元素列表
     * @return vector<shared_ptr<DocumentElement>> 文档元素列表
     */
    const std::vector<std::shared_ptr<DocumentElement>>& getElements() const override;

    /**
     * @brief 添加文档元素
     * @param element 要添加的元素
     */
    void addElement(std::shared_ptr<DocumentElement> element) override;

protected:
    std::string title_;  // 文档标题
    std::vector<std::shared_ptr<DocumentElement>> elements_;  // 文档元素列表

private:
    /**
     * @brief 解析.docx文档
     * @param xmlDoc XML文档对象
     */
    void parseDocument(xmlDocPtr xmlDoc);

    /**
     * @brief 解析段落
     * @param node XML节点
     */
    void parseParagraph(xmlNodePtr node);

    /**
     * @brief 解析表格
     * @param node XML节点
     */
    void parseTable(xmlNodePtr node);

    /**
     * @brief 解析表格行
     * @param node XML节点
     * @return TableRow 解析后的表格行
     */
    TableRow parseTableRow(xmlNodePtr node);

    /**
     * @brief 解析表格单元格
     * @param node XML节点
     * @return TableCell 解析后的表格单元格
     */
    TableCell parseTableCell(xmlNodePtr node);

    /**
     * @brief 解析图片
     * @param node XML节点
     */
    void parseImage(xmlNodePtr node);

    /**
     * @brief 从.docx文件中提取图片数据
     * @param imageId 图片ID
     * @param format 图片格式
     * @return vector<uint8_t> 图片数据
     */
    std::vector<uint8_t> extractImageData(const std::string& imageId, const std::string& format);

    /**
     * @brief 获取节点文本
     * @param node XML节点
     * @return string 节点文本
     */
    std::string getNodeText(xmlNodePtr node);

    /**
     * @brief 解析.doc文档
     * @param filePath 文件路径
     */
    void parseDocDocument(const std::string& filePath);

    /**
     * @brief 从antiword输出中提取文本内容
     * @param output antiword的输出内容
     */
    void extractTextFromAntiwordOutput(const std::string& output);

    std::string docxPath_;  // 当前打开的.docx文件路径
};

} // namespace doc_converter 