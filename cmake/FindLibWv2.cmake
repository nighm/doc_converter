# FindLibWv2.cmake
# 查找 libwv2 库

find_path(LibWv2_INCLUDE_DIR
    NAMES wv2/parser.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        ${LibWv2_ROOT}/include
)

find_library(LibWv2_LIBRARY
    NAMES wv2 libwv2
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        ${LibWv2_ROOT}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibWv2
    DEFAULT_MSG
    LibWv2_LIBRARY
    LibWv2_INCLUDE_DIR
)

if(LibWv2_FOUND)
    set(LibWv2_LIBRARIES ${LibWv2_LIBRARY})
    set(LibWv2_INCLUDE_DIRS ${LibWv2_INCLUDE_DIR})
endif()

mark_as_advanced(LibWv2_INCLUDE_DIR LibWv2_LIBRARY) 