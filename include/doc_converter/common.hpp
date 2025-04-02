#pragma once

namespace doc_converter {

/**
 * @brief 文档元素类型枚举
 */
enum class ElementType {
    Text,       ///< 文本元素
    Paragraph,  ///< 段落元素
    Heading,    ///< 标题元素
    Table,      ///< 表格元素
    Image,      ///< 图片元素
    List        ///< 列表元素
};

/**
 * @brief 文档元素基类
 * 
 * 所有具体的文档元素类都必须继承自这个基类。
 */
class DocumentElement {
public:
    virtual ~DocumentElement() = default;
    
    /**
     * @brief 获取元素类型
     * @return ElementType 返回当前元素的类型
     */
    virtual ElementType getType() const = 0;
};

} // namespace doc_converter 