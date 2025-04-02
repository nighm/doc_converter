cmake_minimum_required(VERSION 3.10)
project(doc_converter VERSION 1.1.1 LANGUAGES CXX)

# 打印 CMake 变量
message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 添加cmake模块路径
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# 设置LibreOffice SDK环境变量
set(ENV{OFFICE_HOME} "/usr/lib/libreoffice")
set(ENV{SDK_HOME} "/usr/lib/libreoffice/sdk")
set(ENV{PATH} "$ENV{SDK_HOME}/bin:$ENV{PATH}")
set(ENV{CPPFLAGS} "-I$ENV{SDK_HOME}/include")
set(ENV{LDFLAGS} "-L$ENV{SDK_HOME}/lib")

# 设置平台定义
add_definitions(
    -DLINUX
    -DUNX
    -DCPPU_ENV=gcc3
    -DGCC
    -DLINUX_X86_64
)

# 查找依赖包
find_package(GTest REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 设置Qt自动MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 添加include目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LIBXML2_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

# 添加源文件
set(SOURCES
    src/word_document.cpp
    src/logger.cpp
    src/main.cpp
    src/main_window.cpp
)

# 添加头文件
set(HEADERS
    include/doc_converter/main_window.hpp
    include/doc_converter/document.hpp
    include/doc_converter/document_elements.hpp
    include/doc_converter/word_document.hpp
    include/doc_converter/basic_converter.hpp
    include/doc_converter/logger.hpp
)

# 创建库
add_library(doc_converter_lib SHARED ${SOURCES} ${HEADERS})

# 链接依赖库
target_link_libraries(doc_converter_lib
    PRIVATE
    ${LIBXML2_LIBRARIES}
    ${ZLIB_LIBRARIES}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

# 设置输出目录
set_target_properties(doc_converter_lib PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# 创建可执行文件
add_executable(doc_converter src/main.cpp)
target_link_libraries(doc_converter
    PRIVATE
    doc_converter_lib
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

# 安装规则
install(TARGETS doc_converter_lib doc_converter
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/doc_converter
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# 添加测试
enable_testing()
add_subdirectory(tests) 