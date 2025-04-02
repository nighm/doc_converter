/**
 * @file main_window.cpp
 * @brief 主窗口类的实现
 * @author ChatGPT
 * @date 2024-03-26
 */

#include "doc_converter/main_window.hpp"
#include "doc_converter/logger.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

namespace doc_converter {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , openButton_(nullptr)
    , convertButton_(nullptr)
    , previewEdit_(nullptr)
    , statusLabel_(nullptr)
    , doc_(nullptr)
{
    std::vector<std::string> formats = {"pdf", "txt"};
    converter_ = std::make_shared<BasicConverter>("Basic Converter", formats);
    initUI();
}

MainWindow::~MainWindow() {
}

void MainWindow::initUI() {
    // 创建中央部件
    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建主布局
    auto mainLayout = new QVBoxLayout(centralWidget);

    // 创建按钮布局
    auto buttonLayout = new QHBoxLayout();
    
    // 创建打开文件按钮
    openButton_ = new QPushButton("打开文件", this);
    connect(openButton_, &QPushButton::clicked, this, &MainWindow::onOpenFile);
    buttonLayout->addWidget(openButton_);

    // 创建转换按钮
    convertButton_ = new QPushButton("转换", this);
    connect(convertButton_, &QPushButton::clicked, this, &MainWindow::onConvert);
    convertButton_->setEnabled(false);
    buttonLayout->addWidget(convertButton_);

    mainLayout->addLayout(buttonLayout);

    // 创建预览区域
    previewEdit_ = new QTextEdit(this);
    previewEdit_->setReadOnly(true);
    mainLayout->addWidget(previewEdit_);

    // 创建状态标签
    statusLabel_ = new QLabel("就绪", this);
    mainLayout->addWidget(statusLabel_);

    // 设置窗口属性
    setWindowTitle("文档转换器");
    resize(800, 600);
}

void MainWindow::onOpenFile() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "选择文档",
        QString(),
        "Word文档 (*.docx *.doc);;所有文件 (*.*)"
    );

    if (!filePath.isEmpty()) {
        if (loadDocument(filePath.toStdString())) {
            statusLabel_->setText("文档加载成功");
            convertButton_->setEnabled(true);
            updatePreview();
        } else {
            statusLabel_->setText("文档加载失败");
            QMessageBox::warning(this, "错误", "无法加载文档");
        }
    }
}

void MainWindow::onConvert() {
    if (!doc_) {
        QMessageBox::warning(this, "错误", "没有加载文档");
        return;
    }

    QString savePath = QFileDialog::getSaveFileName(
        this,
        "保存转换后的文档",
        QString(),
        "PDF文档 (*.pdf);;所有文件 (*.*)"
    );

    if (!savePath.isEmpty()) {
        try {
            converter_->convert(*doc_, savePath.toStdString());
            statusLabel_->setText("转换成功");
            QMessageBox::information(this, "成功", "文档转换完成");
        } catch (const std::exception& e) {
            statusLabel_->setText("转换失败");
            QMessageBox::critical(this, "错误", QString("转换失败: %1").arg(e.what()));
        }
    }
}

bool MainWindow::loadDocument(const std::string& filePath) {
    try {
        doc_ = std::make_shared<WordDocument>();
        if (!doc_->loadFromFile(filePath)) {
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        Logger::getInstance().error("加载文档失败: " + std::string(e.what()));
        return false;
    }
}

void MainWindow::updatePreview() {
    if (!doc_) {
        previewEdit_->clear();
        return;
    }

    std::stringstream ss;
    const auto& elements = doc_->getElements();
    
    for (const auto& element : elements) {
        switch (element->getType()) {
            case ElementType::Text: {
                auto text = std::dynamic_pointer_cast<TextElement>(element);
                ss << text->getText() << "\n";
                break;
            }
            case ElementType::Paragraph: {
                auto para = std::dynamic_pointer_cast<ParagraphElement>(element);
                for (const auto& text : para->getTexts()) {
                    ss << text->getText() << " ";
                }
                ss << "\n\n";
                break;
            }
            case ElementType::Heading: {
                auto heading = std::dynamic_pointer_cast<HeadingElement>(element);
                ss << "#" << heading->getLevel() << " " << heading->getText() << "\n\n";
                break;
            }
            case ElementType::Table: {
                auto table = std::dynamic_pointer_cast<TableElement>(element);
                for (const auto& row : table->getRows()) {
                    for (const auto& cell : row.getCells()) {
                        ss << "|" << cell.getText() << "|";
                    }
                    ss << "\n";
                }
                ss << "\n";
                break;
            }
            case ElementType::Image: {
                auto image = std::dynamic_pointer_cast<ImageElement>(element);
                ss << "[图片: " << image->getWidth() << "x" << image->getHeight() 
                   << " (" << image->getFormat() << ")]\n\n";
                break;
            }
            default:
                break;
        }
    }

    previewEdit_->setPlainText(QString::fromStdString(ss.str()));
}

} // namespace doc_converter 