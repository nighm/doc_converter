# 文档转换工具

这是一个功能强大的文档转换工具，支持将Word文档转换为多种格式，包括HTML、PPT、Excel、Markdown等。

## 版本信息

当前版本：v1.1.0 (2024-03-31)

### 更新日志

#### v1.1.0 (2024-03-31)
- 改进了文件格式检查逻辑
- 完善了错误处理和日志记录
- 优化了测试用例实现
- 支持.doc格式文档解析
- 所有测试用例通过

#### v0.1.0 (2024-03-26)
- 创建了基本项目结构
- 实现了核心接口设计
- 配置了构建系统
- 添加了测试框架
- 实现了日志系统

## 功能特点

- 支持Word文档（.docx和.doc格式）转换为HTML
- 保持原文档的格式和样式
- 支持图片、表格等复杂元素
- 可扩展的插件系统
- 高性能的文档处理
- 完善的错误处理和日志记录

## 系统要求

- C++17兼容的编译器
- CMake 3.10或更高版本
- Google Test
- libxml2
- zlib

## 安装依赖

在Ubuntu/Debian系统上：

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libgtest-dev libxml2-dev zlib1g-dev
```

## 构建项目

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目
cmake ..

# 编译项目
make

# 运行测试
./tests/doc_converter_tests
```

## 使用方法

### 命令行工具

将Word文档转换为HTML：

```bash
./bin/word_to_html 输入文档.docx 输出文档.html
```

## 项目结构

```
doc_converter/
├── src/                    # 源代码目录
│   ├── word_document.cpp  # Word文档处理
│   └── logger.cpp         # 日志系统
├── include/               # 头文件目录
│   └── doc_converter/     # 公共头文件
├── tests/                 # 测试代码
└── docs/                  # 文档
```

## 开发计划

请查看 [开发计划.md](开发计划.md) 了解详细的开发进度和计划。

## 贡献指南

1. Fork 项目
2. 创建特性分支
3. 提交更改
4. 推送到分支
5. 创建 Pull Request

## 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。 