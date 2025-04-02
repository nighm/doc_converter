/**
 * @file main_window.hpp
 * @brief 主窗口类的定义
 * @author ChatGPT
 * @date 2024-03-26
 */

#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include "doc_converter/document.hpp"
#include "doc_converter/document_elements.hpp"
#include "doc_converter/word_document.hpp"
#include "doc_converter/basic_converter.hpp"

namespace doc_converter {

/**
 * @brief 主窗口类
 * 
 * 提供文档转换器的主要用户界面，包括：
 * - 文件选择
 * - 文档预览
 * - 转换操作
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~MainWindow() override;

private slots:
    /**
     * @brief 打开文件按钮点击处理
     */
    void onOpenFile();

    /**
     * @brief 转换按钮点击处理
     */
    void onConvert();

private:
    /**
     * @brief 初始化UI
     */
    void initUI();

    /**
     * @brief 加载文档
     * @param filePath 文件路径
     * @return bool 是否加载成功
     */
    bool loadDocument(const std::string& filePath);

    /**
     * @brief 更新预览
     */
    void updatePreview();

    QPushButton* openButton_;      ///< 打开文件按钮
    QPushButton* convertButton_;   ///< 转换按钮
    QTextEdit* previewEdit_;       ///< 预览文本编辑框
    QLabel* statusLabel_;          ///< 状态标签
    std::shared_ptr<Document> doc_; ///< 当前文档
    std::shared_ptr<Converter> converter_; ///< 文档转换器
};

} // namespace doc_converter 