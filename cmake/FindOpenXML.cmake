# FindOpenXML.cmake
# 查找OpenXML SDK库
#
# 定义以下变量：
# OPENXML_FOUND - 是否找到OpenXML SDK
# OPENXML_INCLUDE_DIRS - OpenXML SDK头文件目录
# OPENXML_LIBRARIES - OpenXML SDK库文件

include(FetchContent)

FetchContent_Declare(
    openxml
    GIT_REPOSITORY https://github.com/OfficeDev/Open-XML-SDK.git
    GIT_TAG v3.0.0
)

FetchContent_GetProperties(openxml)
if(NOT openxml_POPULATED)
    FetchContent_Populate(openxml)
    
    # 设置OpenXML SDK的构建选项
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
    set(BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
    
    # 添加OpenXML SDK的子目录
    add_subdirectory(${openxml_SOURCE_DIR} ${openxml_BINARY_DIR})
endif()

# 设置输出变量
set(OPENXML_FOUND TRUE)
set(OPENXML_INCLUDE_DIRS ${openxml_SOURCE_DIR}/include)
set(OPENXML_LIBRARIES openxml) 