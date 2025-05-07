cmake_minimum_required(VERSION 3.16)
project(QtCppDemo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 包
find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick LinguistTools)
# 如果需要其他 Qt 模块，可以在这里添加，例如：
# find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network)

# 定义构建信息变量
function(get_target_architecture target_architecture)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(${target_architecture} "x64" PARENT_SCOPE)
    else()
        set(${target_architecture} "x86" PARENT_SCOPE)
    endif()
endfunction()
get_target_architecture(ARCHITECTURE)
add_compile_definitions(
    BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    CMAKE_GENERATOR="${CMAKE_GENERATOR}"   
    ARCHITECTURE="${ARCHITECTURE}"
)

# 添加源文件
add_executable(QtCppDemo
    main.cpp
    ${CMAKE_SOURCE_DIR}/src/Test.h
    ${CMAKE_SOURCE_DIR}/src/Test.cpp
    ${CMAKE_SOURCE_DIR}/resource/resources.qrc
)

# 启用 Qt MOC 自动处理
set_target_properties(QtCppDemo PROPERTIES
AUTOMOC ON
AUTORCC ON
AUTOUIC ON
)

# 链接 Qt 库
target_link_libraries(QtCppDemo PRIVATE
Qt5::Core
Qt5::Gui
Qt5::Qml
)

set(TS_FILE ${CMAKE_SOURCE_DIR}/resource/Info_en_US.ts)
qt5_add_translation(QM_FILES ${TS_FILE})
add_custom_target(qmfiles ALL DEPENDS ${QM_FILES})
add_custom_command(TARGET QtCppDemo DEPENDS qmfiles
COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/translations"
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} "${CMAKE_CURRENT_BINARY_DIR}/translations"
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(QtCppDemo qmfiles)

