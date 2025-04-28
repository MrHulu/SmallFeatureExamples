cmake_minimum_required(VERSION 3.10)

# 查找Qt和测试组件
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Sql Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Sql Test REQUIRED)

# 查找所有测试文件
file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tst_*.cpp")

# 包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# 为每个测试文件创建单独的可执行文件
foreach(TEST_FILE ${TEST_FILES})
    # 获取文件名（不含路径和扩展名）作为测试名称
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    
    # 添加可执行文件
    add_executable(${TEST_NAME} ${TEST_FILE})
    
    # 设置可执行文件属性
    set_target_properties(${TEST_NAME} PROPERTIES
        AUTOUIC ON
        AUTOMOC ON
        AUTORCC ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        INCLUDE_CURRENT_DIR ON)
    
    # 链接Qt库
    target_link_libraries(${TEST_NAME} PRIVATE 
        Qt${QT_VERSION_MAJOR}::Core 
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Test)
    
    # 添加源文件
    target_sources(${TEST_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/Database.cpp)
    
    # 添加为CTest测试
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach() 