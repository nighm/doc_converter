# FindLibreOffice.cmake
# 查找 LibreOffice 库

# 查找LibreOffice
#
# 定义以下变量:
#  LIBREOFFICE_FOUND - 是否找到LibreOffice
#  LIBREOFFICE_INCLUDE_DIRS - LibreOffice头文件目录
#  LIBREOFFICE_LIBRARIES - LibreOffice库文件

# 查找LibreOffice安装路径
find_path(LIBREOFFICE_ROOT_DIR
    NAMES
        program/soffice
        program/soffice.bin
    PATHS
        /usr/lib/libreoffice
        /usr/lib64/libreoffice
        /opt/libreoffice
        /opt/libreoffice*
        $ENV{LIBREOFFICE_ROOT}
    DOC "LibreOffice安装根目录"
)

if(NOT LIBREOFFICE_ROOT_DIR)
    # 尝试从soffice可执行文件位置推断
    find_program(SOFFICE_EXECUTABLE
        NAMES soffice
        PATHS
            /usr/bin
            /usr/local/bin
    )
    
    if(SOFFICE_EXECUTABLE)
        get_filename_component(SOFFICE_DIR "${SOFFICE_EXECUTABLE}" DIRECTORY)
        get_filename_component(LIBREOFFICE_ROOT_DIR "${SOFFICE_DIR}" DIRECTORY)
    else()
        message(FATAL_ERROR "未找到LibreOffice安装目录")
    endif()
endif()

# 设置包含目录
set(LIBREOFFICE_INCLUDE_DIRS
    /usr/include/libreoffice
    /usr/include/libreoffice/uno
    /usr/include/libreoffice/uno/cpp
    /usr/include/libreoffice/uno/cppu
    /usr/include/libreoffice/uno/cppuhelper
    /usr/include/libreoffice/uno/sal
    /usr/include/libreoffice/uno/salhelper
)

# 设置库目录
set(LIBREOFFICE_LIB_DIR
    /usr/lib/libreoffice/program
    /usr/lib/x86_64-linux-gnu
)

# 查找必要的库文件
find_library(LIBREOFFICE_CPPU_LIBRARY
    NAMES
        uno_cppu
        libuno_cppu.so.3
    PATHS
        ${LIBREOFFICE_LIB_DIR}
)

find_library(LIBREOFFICE_CPPUHELPER_LIBRARY
    NAMES
        uno_cppuhelpergcc3
        libuno_cppuhelpergcc3.so.3
    PATHS
        ${LIBREOFFICE_LIB_DIR}
)

find_library(LIBREOFFICE_SAL_LIBRARY
    NAMES
        uno_sal
        libuno_sal.so.3
    PATHS
        ${LIBREOFFICE_LIB_DIR}
)

find_library(LIBREOFFICE_SALHELPER_LIBRARY
    NAMES
        uno_salhelpergcc3
        libuno_salhelpergcc3.so.3
    PATHS
        ${LIBREOFFICE_LIB_DIR}
)

if(LIBREOFFICE_CPPU_LIBRARY AND LIBREOFFICE_CPPUHELPER_LIBRARY AND 
   LIBREOFFICE_SAL_LIBRARY AND LIBREOFFICE_SALHELPER_LIBRARY)
    set(LIBREOFFICE_LIBRARIES
        ${LIBREOFFICE_CPPU_LIBRARY}
        ${LIBREOFFICE_CPPUHELPER_LIBRARY}
        ${LIBREOFFICE_SAL_LIBRARY}
        ${LIBREOFFICE_SALHELPER_LIBRARY}
    )
    set(LIBREOFFICE_FOUND TRUE)
else()
    message(FATAL_ERROR "未找到LibreOffice库文件")
endif()

# 打印调试信息
if(NOT LIBREOFFICE_FIND_QUIETLY)
    message(STATUS "找到LibreOffice:")
    message(STATUS "  根目录: ${LIBREOFFICE_ROOT_DIR}")
    message(STATUS "  包含目录: ${LIBREOFFICE_INCLUDE_DIRS}")
    message(STATUS "  库文件: ${LIBREOFFICE_LIBRARIES}")
endif()

# 处理版本要求
if(LIBREOFFICE_FIND_VERSION)
    if(${LIBREOFFICE_VERSION} VERSION_LESS ${LIBREOFFICE_FIND_VERSION})
        message(FATAL_ERROR "LibreOffice版本 ${LIBREOFFICE_VERSION} 低于要求版本 ${LIBREOFFICE_FIND_VERSION}")
    endif()
endif()

# 设置缓存变量
set(LIBREOFFICE_ROOT_DIR ${LIBREOFFICE_ROOT_DIR} CACHE PATH "LibreOffice安装根目录")
set(LIBREOFFICE_INCLUDE_DIRS ${LIBREOFFICE_INCLUDE_DIRS} CACHE PATH "LibreOffice头文件目录")
set(LIBREOFFICE_LIBRARIES ${LIBREOFFICE_LIBRARIES} CACHE FILEPATH "LibreOffice库文件")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibreOffice
    DEFAULT_MSG
    LIBREOFFICE_ROOT_DIR
    LIBREOFFICE_INCLUDE_DIRS
    LIBREOFFICE_LIBRARIES
)

mark_as_advanced(
    LIBREOFFICE_ROOT_DIR
    LIBREOFFICE_INCLUDE_DIRS
    LIBREOFFICE_LIBRARIES
) 