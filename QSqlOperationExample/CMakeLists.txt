cmake_minimum_required(VERSION 3.10)
project(QSqlOperationExample)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Sql REQUIRED)

# release build
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release Build, add /Zi /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    # set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

add_executable(QSqlOperationExample)
file(GLOB_RECURSE SOURCES_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
target_sources(QSqlOperationExample PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/example/Example.cpp
    ${SOURCES_FILES}
)
set_target_properties(QSqlOperationExample PROPERTIES
    AUTOUIC ON
    AUTOMOC ON
    AUTORCC ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    INCLUDE_CURRENT_DIR ON
    OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(QSqlOperationExample PUBLIC 
    Qt5::Core 
    Qt5::Sql)
# target_include_directories(QSqlOperationExample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
# add_subdirectory(src)

if(QT_VERSION_MAJOR EQUAL 6)
qt_import_qml_plugins(QSqlOperationExample)
qt_finalize_executable(QSqlOperationExample)
endif()

# 添加测试子目录
add_subdirectory(test)
