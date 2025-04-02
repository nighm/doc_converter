/**
 * @file word_document.cpp
 * @brief Word文档解析器的实现
 * @author ChatGPT
 * @date 2024-03-26
 */

#include "doc_converter/word_document.hpp"
#include "doc_converter/document_elements.hpp"
#include "doc_converter/logger.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>
#include <iostream>

namespace doc_converter {

WordDocument::WordDocument(const std::string& title) {
    title_ = title;
}

WordDocument::~WordDocument() {
}

bool WordDocument::loadFromFile(const std::string& filePath) {
    try {
        // 检查文件是否存在
        if (!std::filesystem::exists(filePath)) {
            throw std::runtime_error("File not found: " + filePath);
        }

        // 保存文件路径
        docxPath_ = filePath;

        // 根据文件扩展名选择解析方法
        if (filePath.length() >= 4 && 
            filePath.substr(filePath.length() - 4) == ".docx") {
            // 使用现有的.docx解析代码
            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filePath);
            }

            // 读取文件内容
            std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
            file.close();

            // 解析XML文档
            xmlDocPtr doc = xmlReadMemory(buffer.data(), buffer.size(), nullptr, nullptr, 0);
            if (!doc) {
                throw std::runtime_error("Failed to parse XML document");
            }

            // 解析文档内容
            parseDocument(doc);

            // 清理XML文档
            xmlFreeDoc(doc);
        } else if (filePath.length() >= 3 && 
                   filePath.substr(filePath.length() - 3) == ".doc") {
            // 使用antiword解析.doc文件
            parseDocDocument(filePath);
        } else {
            throw std::runtime_error("Unsupported file format: " + filePath);
        }

        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().error("Failed to load document: " + std::string(e.what()));
        return false;
    }
}

std::string WordDocument::getTitle() const {
    return title_;
}

const std::vector<std::shared_ptr<DocumentElement>>& WordDocument::getElements() const {
    return elements_;
}

void WordDocument::addElement(std::shared_ptr<DocumentElement> element) {
    elements_.push_back(element);
}

void WordDocument::parseDocument(xmlDocPtr xmlDoc) {
    Logger::getInstance().debug("开始解析文档");
    xmlNodePtr root = xmlDocGetRootElement(xmlDoc);
    if (!root) {
        Logger::getInstance().error("无法获取文档根节点");
        return;
    }

    // 遍历文档节点
    for (xmlNodePtr node = root->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(node->name, (const xmlChar*)"p") == 0) {
                parseParagraph(node);
            } else if (xmlStrcmp(node->name, (const xmlChar*)"tbl") == 0) {
                parseTable(node);
            } else if (xmlStrcmp(node->name, (const xmlChar*)"drawing") == 0) {
                parseImage(node);
            }
        }
    }
    Logger::getInstance().debug("文档解析完成");
}

void WordDocument::parseParagraph(xmlNodePtr node) {
    // 检查段落样式，判断是否为标题
    xmlChar* style = xmlGetProp(node, (const xmlChar*)"style");
    if (style && xmlStrstr(style, (const xmlChar*)"Heading")) {
        // 获取标题级别
        const char* levelStr = (const char*)xmlStrstr(style, (const xmlChar*)" ") + 1;
        int level = std::atoi(levelStr);
        xmlFree(style);

        // 创建标题元素
        auto heading = std::make_shared<HeadingElement>(
            getNodeText(node),
            level
        );
        addElement(heading);
        Logger::getInstance().debug("添加标题元素: " + heading->getText() + " (级别: " + std::to_string(level) + ")");
    } else {
        // 创建段落元素
        auto paragraph = std::make_shared<ParagraphElement>();
        std::string paragraphText;

        // 遍历段落中的文本
        for (xmlNodePtr child = node->children; child; child = child->next) {
            if (child->type == XML_TEXT_NODE && child->content) {
                std::string text = std::string((const char*)child->content);
                paragraphText += text;
                paragraph->addText(text);
            }
        }

        // 如果段落不为空，添加到文档
        if (!paragraph->getTexts().empty()) {
            addElement(paragraph);
            Logger::getInstance().debug("添加段落元素: " + paragraphText);
        }
    }
}

std::string WordDocument::getNodeText(xmlNodePtr node) {
    std::string text;
    xmlChar* content = xmlNodeGetContent(node);
    if (content) {
        text = (const char*)content;
        xmlFree(content);
    }
    return text;
}

void WordDocument::parseDocDocument(const std::string& filePath) {
    // 使用antiword命令解析.doc文件
    std::string cmd = "antiword -t " + filePath;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to execute antiword command");
    }

    // 读取antiword的输出
    std::string output;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }

    // 关闭管道
    int status = pclose(pipe);
    if (status != 0) {
        throw std::runtime_error("antiword command failed");
    }

    // 从输出中提取文本内容
    extractTextFromAntiwordOutput(output);
}

void WordDocument::extractTextFromAntiwordOutput(const std::string& output) {
    // 将输出按行分割
    std::istringstream stream(output);
    std::string line;
    std::string currentParagraph;

    while (std::getline(stream, line)) {
        // 跳过空行
        if (line.empty()) {
            if (!currentParagraph.empty()) {
                // 创建段落元素
                auto paragraph = std::make_shared<ParagraphElement>();
                paragraph->addText(currentParagraph);
                elements_.push_back(paragraph);
                currentParagraph.clear();
            }
            continue;
        }

        // 如果是新段落（以特定格式开头）
        if (line[0] == ' ' || line[0] == '\t') {
            if (!currentParagraph.empty()) {
                // 创建段落元素
                auto paragraph = std::make_shared<ParagraphElement>();
                paragraph->addText(currentParagraph);
                elements_.push_back(paragraph);
                currentParagraph.clear();
            }
            // 移除开头的空白字符
            line = line.substr(line.find_first_not_of(" \t"));
        }

        // 将当前行添加到段落中
        if (!currentParagraph.empty()) {
            currentParagraph += " ";
        }
        currentParagraph += line;
    }

    // 处理最后一个段落
    if (!currentParagraph.empty()) {
        auto paragraph = std::make_shared<ParagraphElement>();
        paragraph->addText(currentParagraph);
        elements_.push_back(paragraph);
    }

    Logger::getInstance().debug("从antiword输出中提取了 " + std::to_string(elements_.size()) + " 个段落");
}

void WordDocument::parseTable(xmlNodePtr node) {
    Logger::getInstance().debug("开始解析表格");
    auto table = std::make_shared<TableElement>();

    // 遍历表格行
    for (xmlNodePtr rowNode = node->children; rowNode; rowNode = rowNode->next) {
        if (rowNode->type == XML_ELEMENT_NODE && 
            xmlStrcmp(rowNode->name, (const xmlChar*)"tr") == 0) {
            TableRow row = parseTableRow(rowNode);
            table->addRow(row);
        }
    }

    addElement(table);
    Logger::getInstance().debug("表格解析完成");
}

TableRow WordDocument::parseTableRow(xmlNodePtr node) {
    TableRow row;
    Logger::getInstance().debug("开始解析表格行");

    // 遍历单元格
    for (xmlNodePtr cellNode = node->children; cellNode; cellNode = cellNode->next) {
        if (cellNode->type == XML_ELEMENT_NODE && 
            xmlStrcmp(cellNode->name, (const xmlChar*)"tc") == 0) {
            TableCell cell = parseTableCell(cellNode);
            row.addCell(cell);
        }
    }

    Logger::getInstance().debug("表格行解析完成，包含 " + std::to_string(row.getCells().size()) + " 个单元格");
    return row;
}

TableCell WordDocument::parseTableCell(xmlNodePtr node) {
    Logger::getInstance().debug("开始解析表格单元格");
    std::string cellText;

    // 遍历单元格内容
    for (xmlNodePtr contentNode = node->children; contentNode; contentNode = contentNode->next) {
        if (contentNode->type == XML_ELEMENT_NODE && 
            xmlStrcmp(contentNode->name, (const xmlChar*)"p") == 0) {
            // 获取段落文本
            std::string paragraphText = getNodeText(contentNode);
            if (!cellText.empty()) {
                cellText += "\n";
            }
            cellText += paragraphText;
        }
    }

    Logger::getInstance().debug("表格单元格解析完成: " + cellText);
    return TableCell(cellText);
}

void WordDocument::parseImage(xmlNodePtr node) {
    Logger::getInstance().debug("开始解析图片");
    
    // 查找图片ID
    xmlNodePtr blipNode = nullptr;
    for (xmlNodePtr child = node->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE && 
            xmlStrcmp(child->name, (const xmlChar*)"blip") == 0) {
            blipNode = child;
            break;
        }
    }

    if (!blipNode) {
        Logger::getInstance().error("未找到图片节点");
        return;
    }

    // 获取图片ID
    xmlChar* imageId = xmlGetProp(blipNode, (const xmlChar*)"embed");
    if (!imageId) {
        Logger::getInstance().error("未找到图片ID");
        return;
    }

    // 获取图片格式
    std::string format = "png";  // 默认格式
    xmlNodePtr formatNode = nullptr;
    for (xmlNodePtr child = node->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE && 
            xmlStrcmp(child->name, (const xmlChar*)"ext") == 0) {
            xmlChar* ext = xmlGetProp(child, (const xmlChar*)"uri");
            if (ext) {
                std::string uri = (const char*)ext;
                if (uri == "http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing") {
                    format = "png";
                } else if (uri == "http://schemas.openxmlformats.org/drawingml/2006/jpeg") {
                    format = "jpg";
                }
                xmlFree(ext);
            }
            break;
        }
    }

    // 获取图片尺寸
    int width = 0, height = 0;
    for (xmlNodePtr child = node->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE && 
            xmlStrcmp(child->name, (const xmlChar*)"extent") == 0) {
            xmlChar* cx = xmlGetProp(child, (const xmlChar*)"cx");
            xmlChar* cy = xmlGetProp(child, (const xmlChar*)"cy");
            if (cx && cy) {
                // 转换EMU到像素（1 EMU = 1/9525 像素）
                width = std::atoi((const char*)cx) / 9525;
                height = std::atoi((const char*)cy) / 9525;
                xmlFree(cx);
                xmlFree(cy);
            }
            break;
        }
    }

    // 提取图片数据
    std::vector<uint8_t> imageData = extractImageData((const char*)imageId, format);
    xmlFree(imageId);

    if (!imageData.empty()) {
        // 创建图片元素
        auto image = std::make_shared<ImageElement>(imageData, format, width, height);
        addElement(image);
        Logger::getInstance().debug("添加图片元素: " + std::to_string(width) + "x" + std::to_string(height));
    } else {
        Logger::getInstance().error("无法提取图片数据");
    }
}

std::vector<uint8_t> WordDocument::extractImageData(const std::string& imageId, const std::string& format) {
    // 构建图片文件路径
    std::string imagePath = docxPath_.substr(0, docxPath_.find_last_of("/") + 1) + 
                           "word/media/image" + imageId + "." + format;

    // 读取图片文件
    std::ifstream file(imagePath, std::ios::binary);
    if (!file.is_open()) {
        Logger::getInstance().error("无法打开图片文件: " + imagePath);
        return std::vector<uint8_t>();
    }

    // 读取图片数据
    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();

    return data;
}

} // namespace doc_converter 