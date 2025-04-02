# 创建库目标
add_library(doc_converter_lib STATIC
    converter_factory.cpp
    word_document.cpp
    logger.cpp
)

# 设置库的包含目录
target_include_directories(doc_converter_lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${LIBXML2_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接依赖库
target_link_libraries(doc_converter_lib
    PRIVATE
        ${LIBXML2_LIBRARIES}
)

# 创建可执行文件
add_executable(doc_converter
    main.cpp
)

# 链接库
target_link_libraries(doc_converter
    PRIVATE
        doc_converter_lib
) 