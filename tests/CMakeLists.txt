enable_testing()

# 创建测试可执行文件
add_executable(doc_converter_tests
    main_test.cpp
    document_test.cpp
    document_elements_test.cpp
    basic_document_test.cpp
    basic_converter_test.cpp
    word_document_test.cpp
)

# 链接Google Test和项目库
target_link_libraries(doc_converter_tests
    PRIVATE
    doc_converter_lib
    GTest::GTest
    GTest::Main
    ${LIBXML2_LIBRARIES}
)

# 添加测试
add_test(NAME doc_converter_tests COMMAND doc_converter_tests) 